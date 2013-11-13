var express = require('express');
var gcm = require('node-gcm');
var mysql = require('mysql');
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
        database: config.database_name
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
});

app.listen(config.dev_port, config.ip);

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
    if(req.body && req.body.regId && req.body.trolleyId) {
        // Retrieve JSON data about customer
        var reg_id = req.body.regId;
        var cart_id = parseInt(req.body.trolleyId);
        var customer_id = parseInt(req.body.memId);
        var shopping_list = null;
        if (req.body.shoppingList) {
            shopping_list = req.body.shoppingList;
        }
        var time_connected = new Date().getTime();

        // Add to the list of customers present for later push notifications
        var user = {
            'customer_id': customer_id,
            'reg_id': reg_id
        };
        registration_ids.push(user);

        // Send confirmation of connection and list of offers (currently null)
        // will get offers from DB
        res.json({
            'status': 'connected',
            'offers': null
        });

        // Signal to database that customer is active
        var values = {Customer_ID : customer_id, Cart_ID: cart_id, Time_Connected: time_connected};
        var sql = 'INSERT INTO Active_Customers SET ?';
        var query = connection.query(sql, values,
            function(err, result) {
                if (err) {
                    throw err;
                }
                res.send(query.sql);
            });
    }
    else {
        console.log('Incorrect data sent');
    }
});

setInterval(function() {
    console.log('Check DB');
    var message;
    var query = connection.query('SELECT * FROM Cart_Positioning', function(err, rows, fields) {
        if (err) {
            throw new Error('Failed');
        }
        console.log('# of rows in db ' + rows.length);
        message = new gcm.Message();
        for (var i = 0; i < rows.length; i++) {
            var c_id = rows[i].Customer_Id;
            console.log(c_id);
            for (var customer_data in registration_ids) {
                if (customer_data['customer_id'] === c_id) {
                    message.addDataWithObject({
                        title: 'Position',
                        x: rows[i].X,
                        y: rows[i].Y
                    });

                    sender.send(message, [customer_data['reg_id']], 4, function(result) {
                        console.log(result);
                    });

                    break;
                }
            }
        }
    });

}, 5000);