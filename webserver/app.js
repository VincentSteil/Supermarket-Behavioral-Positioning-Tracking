var express = require('express');
var gcm = require('node-gcm');
var routes = require('./routes');
var mysql = require('mysql');
var connection;

// Create an express app
var app = express();

app.configure(function() {
    // Set location of views and template engine
    app.set('views', __dirname + '/views');
    app.set('view engine', 'ejs');
    // To handle post data
    app.use(express.bodyParser());
    // Set up the database server connection
    connection = mysql.createConnection({
        host: process.env.IP,
        port: process.env.PORT,
        user: 'pavel',
        password: 'pavel\'s password'
    });
});

app.listen(process.env.PORT, process.env.IP);

// Set the  server/api key for GCM
var sender = new gcm.Sender('AIzaSyCBas1G4aI6k_hCSgFF0n8vcTtmVN5NFEA');

// Store of registration IDs - unique to every Android phone
var registration_ids = [];

// Function for the main administrator's view
//app.get('/', routes.index);
app.get('/', function(req, res) {
    routes.index(req, res, registration_ids);
});

// Function that gets post data from Google upon device registration
app.post('/', function(req, res) {
    if(req.body && req.body.regId) {
        // Retrieve JSON data about customer
        var reg_id = req.body.regId;
        var trolley_id = req.body.name;
        var customer_id = req.body.email;
        var time_connected = req.body.time;

        // Add to the list of customers present
        registration_ids.push(reg_id);
        console.log('New user: ' + customer_id + ' using trolley: ' + trolley_id
        + ' connected at: ' + time_connected);

        // Signal to database that customer is active
        var query = connection.query('INSERT INTO Active_Customers SET ?', req.body,
            function(err, result) {
                if (err) {
                    throw err;
                }
                console.log('Query succesful!');
                console.log(query.sql);
            });
    }
});