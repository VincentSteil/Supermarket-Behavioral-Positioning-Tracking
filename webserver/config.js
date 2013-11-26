var config = {};

// Set for test environment
config.test = true;

// Server settings
config.ip = process.env.IP;
config.port = config.test ? 8000 : process.env.PORT;

// Template settings
config.view_dir =  __dirname + '/views';
config.template_engine = 'ejs';

// GCM settings
config.gcm_api_key = 'AIzaSyCBas1G4aI6k_hCSgFF0n8vcTtmVN5NFEA';

// Database settings
config.sql_host = config.test ? process.env.PORT : '192.185.21.164';
config.sql_port = '3306';
config.sql_user = config.test ? 'msanatan' : 'msanatan_slip';
config.sql_password = config.test ? 'password' : 'slipdemo';
config.database_name = config.test ? 'SLIP_DB' : 'msanatan_SLIP_DB';
config.insecure_auth = true;

// For polling
config.TIMEOUT = 5000;

module.exports = config;
