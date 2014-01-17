var express = require('express');
var gcm = require('node-gcm');
var mysql = require('mysql');
var request = require('request');
var routes = require('./routes');
var config = require('./config');
var connection;

// Create an express app
var app = express();

// Store the db configuration
var db_config = {
    host: config.sql_host,
    port: config.sql_port,
    user: config.sql_user,
    password: config.sql_password,
    database: config.database_name,
    insecureAuth: config.insecure_auth
};

app.configure(function() {
    // Set location of views and template engine
    app.set('views', config.view_dir);
    app.set('view engine', config.template_engine);
    // To get access to public files such as stylesheets, images etc.
    app.use(express.static(__dirname + '/public'));
    app.use('public/style', express.static(__dirname + '/public/style'));
    app.use('public/style/css', express.static(__dirname + '/public/style/css'));
    app.use('public/style/font', express.static(__dirname + '/public/style/font'));
    app.use('public/style/img', express.static(__dirname + '/public/style/img'));
    // To handle post data
    app.use(express.bodyParser());
    // Set up the database server connection
    connection = mysql.createConnection(db_config);
    connection.connect(function(error) {
        if (!error) {
            console.log('Connected to database');
        }
        else {
            throw error;
        }
    });
});

app.listen(config.port, config.ip);

// Set the  server/api key for GCM
var sender = new gcm.Sender(config.gcm_api_key);

// Store of registration IDs - unique to every Android phone
var registration_ids = [];

// Handle various views
app.get('/', function(req, res) {
    routes.index(req, res, registration_ids);
});

app.get('/about', function(req, res) {
    routes.about(req, res);
});

app.get('/contact', function(req, res) {
    routes.index(req, res, registration_ids);
});

app.get('/admin', function(req, res) {
    routes.index(req, res, registration_ids);
});

// Function that gets post data from Google upon device registration
app.post('/', function(req, res) {
    if(req.body && req.body.regId && req.body.trolleyId && req.body.memId) {
        // Retrieve JSON data about customer
        var reg_id = req.body.regId;
        var cart_id = parseInt(req.body.trolleyId, 10);
        var customer_id = parseInt(req.body.memId, 10);
        var time_connected = new Date().getTime();

        // Do not add user if garbage data is sent
        if (isNaN(cart_id) || isNaN(customer_id)) {
            res.json({'error': 'Trolley and Member ID must be integers!'});
            console.log('error, user didn\'t connect');
        }
        // If valid data was sent
        else {
            // Add to the list of customers present for later push notifications
            var user = {
                'customer_id': customer_id,
                'ip' : req.ip,
                'reg_id': reg_id,
                'last_movement': time_connected
            };
            registration_ids.push(user);
            console.log(user);
            // Return a success message
            res.json({'status': 'connected'});


            // For SQL query
            var values = {};
            var sql = '';
            var insert = true;

            // Check if the customer is in the database before
            var sql_check = 'SELECT * FROM Active_Customers WHERE Customer_ID = ' + customer_id;
            var check = connection.query(sql_check, function(err, rows, fields) {
                for (var i = 0; i < rows.length; rows++) {
                    if (rows[i].Customer_ID === customer_id) {
                        values = {Cart_ID: cart_id, Time_Connected: time_connected};
                        sql = 'UPDATE Active_Customers SET Cart_ID = ' + cart_id + ', Time_Connected = ' + time_connected + ' WHERE Customer_ID = ' + customer_id + ';';
                        insert = false;
                    }
                }
            });

            // Set up to insert values if the user is not in the db
            if (insert) {
                values = {Customer_ID : customer_id, Cart_ID: cart_id, Time_Connected: time_connected};
                sql = 'INSERT INTO Active_Customers SET ?;';
            }

            // Run the query to update or insert the user
            var query = connection.query(sql, values,
                function(error, rows, fields) {
                    if (error) {
                        console.log(sql);
                        throw error;
                    }
                }
            );
        } // end else when input is valid
    }
    else {
        console.log('No registration ID/ cart ID/ membership ID received');
        res.json({'error': 'No registration ID/ cart ID/ membership ID received'});
    }
});

var recursive_db_check = function() {
    console.log('Database polling');

    // Get all the customers currently in the database
    var customers = connection.query('SELECT * FROM Active_Customers;', function(error, rows, fields) {
        if (error) {
            throw error;
        }
        // Retrieve all the customer ids
        rows = rows.map(function(row) {
            return row.Customer_ID;
        })
        // Clean up list of users to send messages to
        // Return list of customers who are also present in the database
        registration_ids = registration_ids.filter(function(user) {
            return rows.indexOf(user.customer_id) !== -1
        });
    });

    // Get the positions of the members and send it to the phones
    var query = connection.query('SELECT * FROM Cart_Positioning ORDER BY Time DESC;', function(error, rows, fields) {
        if (error) {
            throw new Error('Failed');
        }
        message = new gcm.Message();
        for (var i = 0; i < rows.length; i++) {
            var c_id = rows[i].Customer_ID;
            var time = rows[i].Time;
            console.log('From DB: ' + c_id);

            for (var j = 0; j < registration_ids.length; j++) {
                customer_data = registration_ids[j];
                if (customer_data['customer_id'] === c_id && customer_data['last_movement'] < time) {
                    // Send a request so that GCM messages can be utilised
                    customer_data['last_movement'] = time;
                    message.addDataWithObject({
 -                        title: 'Position',
 -                        x: rows[i].X,
 -                        y: rows[i].Y
 -                    });
 -
 -                    sender.send(message, [customer_data['reg_id']], 4, function(result) {
 -                        console.log(result);
 -                    });
                }
            }
        }
    });
    setTimeout(recursive_db_check, config.TIMEOUT);
};

recursive_db_check();
