Supermarket Web Server
===================

This is the web server that bridges the gap between the Android phone clients
and the information in the database. It maintains a consistent connection with
all the nodes - updating the database when a customer appears and continuously
updating the customer on deals based on their x and y values.



Installing Dependencies
--------------------
`cd webserver`
`npm install`


Server Configurations
------------------

Before the web server works, you must create a config.js file with the required
fields:

### Server settings
*config.dev_ip
*config.dev_port
*config.ip
*config.port

### Template settings
*config.view_dir =  __dirname + '/views';
*config.template_engine = 'ejs';

### GCM settings
*config.gcm_api_key

### Database settings
*config.sql_host = '172.20.161.9'; // Will change periodically
*config.sql_port = '3306';
*config.sql_user = 'pasha';
*config.sql_password = 'password';

And don't forget to include this line of code at the end!
`module.exports = config;`

