var http = require('http');
var assert = require('assert');
var mysql = require('mysql');
var config = require('./config');

// Settings for HTTP requests to the server
var opts = {
    host: config.ip,
    port: config.port,
    path: '/',
    method: 'POST',
    headers: {'content-type':'application/json'}
};

/*
*               MISSING DATA
*/
// Meant to fail, no data provided
var missing_data_test1 = http.request(opts, function(res) {
    res.setEncoding('utf8')
    var data = "";
    res.on('data', function(d) {
        data += d
    })

    res.on('end', function() {
        assert.deepEqual(JSON.parse(data),
            {error: 'No registration ID/ cart ID/ membership ID received'},
            console.log('Test 1 Passed'))
    })
});

missing_data_test1.write(JSON.stringify({}));
missing_data_test1.end();


// Meant to fail, no registration ID provided
var missing_data_test2 = http.request(opts, function(res) {
    res.setEncoding('utf8')
    var data = "";
    res.on('data', function(d) {
        data += d
    })

    res.on('end', function() {
        assert.deepEqual(JSON.parse(data),
            {error: 'No registration ID/ cart ID/ membership ID received'},
            console.log('Test 2 Passed'))
    })
});

var no_reg = {trolleyId: 1111, memId: 2222};
missing_data_test2.write(JSON.stringify(no_reg));
missing_data_test2.end();


// Meant to fail, no cart ID provided
var missing_data_test3 = http.request(opts, function(res) {
    res.setEncoding('utf8')
    var data = "";
    res.on('data', function(d) {
        data += d
    })

    res.on('end', function() {
        assert.deepEqual(JSON.parse(data),
            {error: 'No registration ID/ cart ID/ membership ID received'},
            console.log('Test 3 Passed'))
    })
});

var no_trolley = {regId: 1111, memId: 2222};
missing_data_test3.write(JSON.stringify(no_trolley));
missing_data_test3.end();


// Meant to fail, no membership ID provided
var missing_data_test4 = http.request(opts, function(res) {
    res.setEncoding('utf8')
    var data = "";
    res.on('data', function(d) {
        data += d
    })

    res.on('end', function() {
        assert.deepEqual(JSON.parse(data),
            {error: 'No registration ID/ cart ID/ membership ID received'},
            console.log('Test 4 Passed'))
    })
});

var no_mem = {regId: 1111, memId: 2222};
missing_data_test4.write(JSON.stringify(no_mem));
missing_data_test4.end();


/*
*               INCORRECT DATA
*/
// Meant to fail, non integer cart ID received
var incorrect_data_test1 = http.request(opts, function(res) {
    res.setEncoding('utf8')
    var data = "";
    res.on('data', function(d) {
        data += d
    })

    res.on('end', function() {
        assert.deepEqual(JSON.parse(data),
            {error: 'Trolley and Member ID must be integers!'},
            console.log('Test 5 Passed'))
    })
});

var bad_trolley = {regId: 1111, trolleyId: 'trollOnARoll', memId: 2222};
incorrect_data_test1.write(JSON.stringify(bad_trolley));
incorrect_data_test1.end();


// Meant to fail, non integer membership ID received
var incorrect_data_test2 = http.request(opts, function(res) {
    res.setEncoding('utf8')
    var data = "";
    res.on('data', function(d) {
        data += d
    })

    res.on('end', function() {
        assert.deepEqual(JSON.parse(data),
        {error: 'Trolley and Member ID must be integers!'},
        console.log('Test 6 Passed'))
    })
});

var bad_mem = {regId: 1111, trolleyId: 11111, memId: 'monkeys!!!'};
incorrect_data_test2.write(JSON.stringify(bad_mem));
incorrect_data_test2.end();


/*
*               CORRECT DATA
*/
// Meant to pass, all correct data received
var correct_data_test1 = http.request(opts, function(res) {
    res.setEncoding('utf8')
    var data = "";
    res.on('data', function(d) {
        data += d
    })

    res.on('end', function() {
        assert.deepEqual(JSON.parse(data),
        {status: 'connected'},
        console.log('Test 7 Passed'))
    })
});

var ns_mem = {regId: '111dfsdafsfdf184', trolleyId: 487784654, memId: 18784254};
correct_data_test1.write(JSON.stringify(ns_mem));
correct_data_test1.end();


/*
*               NONSTANDARD DATA THAT PASSES
*/
// Meant to fail, non integer membership ID received
var nonstandard_data_test1 = http.request(opts, function(res) {
    res.setEncoding('utf8')
    var data = "";
    res.on('data', function(d) {
        data += d
    })

    res.on('end', function() {
        assert.deepEqual(JSON.parse(data),
        {status: 'connected'},
        console.log('Test 8 Passed'))
    })
});

var ns_mem = {regId: 1111, trolleyId: 11111, memId: 33.321};
nonstandard_data_test1.write(JSON.stringify(ns_mem));
nonstandard_data_test1.end();