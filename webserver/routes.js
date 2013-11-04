exports.index = function(req, res, reg_ids) {
    res.render('index', {
        title: 'Welcome!',
        header: 'Mobile Connections',
        registration_ids: reg_ids,
        });
};