var nThen = require('nthen');
var Exec = require('child_process').exec;

var getversion = module.exports = function (callback) {
    nThen(function (waitFor) {
        var child = Exec('git describe --always --dirty --tags', callback);
    });
};
