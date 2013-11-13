var config = {};

// Server settings
config.dev_ip = process.env.IP;
config.dev_port = 8000
config.ip = process.env.IP;
config.port = process.env.PORT;

// Template settings
config.view_dir =  __dirname + '/views';
config.template_engine = 'ejs';

// GCM settings
config.gcm_api_key = 'AIzaSyCBas1G4aI6k_hCSgFF0n8vcTtmVN5NFEA';

// Database settings
config.sql_host = '172.20.160.156'; // Will change periodically
config.sql_port = '3306';
config.sql_user = 'pasha';
config.sql_password = 'password';
config.database_name = 'SLIP_DB';

module.exports = config;
