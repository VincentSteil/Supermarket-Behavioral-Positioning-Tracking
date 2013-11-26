exports.is_valid = function(ids) {
    for (var i = 0; i < ids.length; i++) {
        if (isNaN(ids[i])) {
            return false;
            console.log('An ID was incorrect');
        }
    }
    return true;
};