exports.index = function(req, res, reg_ids) {
    res.render('index', {
        title: 'SLIP Supermarket Server',
        header: 'Mobile Connections',
        registration_ids: reg_ids,
        });
};

exports.about = function(req, res) {
    res.render('about', {
        title: 'SLIP Supermarket Server - About',
        header: 'About'
    })
};
