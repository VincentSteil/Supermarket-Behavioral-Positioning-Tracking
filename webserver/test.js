var gcm = require('node-gcm');

var message = new gcm.Message();

message.addDataWithObject({
    title: 'Hola',
    message: 'Works :D'
});
message.addData('msgcnt','3');

console.log(message);

// Server key :D
var sender = new gcm.Sender('AIzaSyCBas1G4aI6k_hCSgFF0n8vcTtmVN5NFEA');
//var sender = new gcm.Sender('AIzaSyAMdRf70s69B1J-rUSIxSHGUiKfpLSETs8');

// maintain a list of the apps connected via their unique registration ID
var registrationIds = ['APA91bGnkQOMFCVHBBYEYajpp8o03q1fCuiks7xdMvdiBrRSYq5R51OlQpymwpzlfzcjRmmXgUxd1z7oEr8Z5611UDGJv1VrTE9fq6HaRmIkLln5eGZMy_0_W_tUH89qirwQvX-TvtrTSPobmF1FFcon03nc_uRcxoNpS-5FvvAKDOUmgXGG9b8'];

/**
 * Parameters: message-literal, registrationIds-array, No. of retries, callback-function
 */
sender.send(message, registrationIds, 4, function (result) {
    console.log(result);
});