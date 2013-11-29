var Fs = require('fs');

module.exports.get = function (callback) {
    Fs.readFile('.git/logs/HEAD', function (err, ret) {
        if (err) { throw err; }
        var lines = ret.toString('utf8').split('\n');
        var hashes = lines[lines.length-2].split(' ');
        var head = hashes[1];
        if (!(/^[a-f0-9]{40}$/.test(head))) {
            throw new Error(head + ' does not look like a git hash');
        }
        callback(head);
    });
};
