var nThen = require('nthen');
var Spawn = require('child_process').spawn;

var getversion = module.exports = function (callback) {
    nThen(function (waitFor) {
        var child = Spawn('git', ['describe', '--always', '--dirty', '--tags']);
        child.stdout.on('data', function(data) {
            callback(data);
        });
        child.stderr.on('data', function(data) {
            throw new Error("git error: " + data);
        });
        child.on('close', function(code) {
        });
        child.on('error', function(err) {
            throw new Error("git error: " + err);
        });
    });
};
