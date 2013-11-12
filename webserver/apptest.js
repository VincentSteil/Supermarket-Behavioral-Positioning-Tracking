var http = require('http');
var assert = require('assert');
var mysql = require('mysql');

var db_config = {
        host: config.sql_host,
        port: config.sql_port,
        user: config.sql_user,
        password: config.sql_password
};