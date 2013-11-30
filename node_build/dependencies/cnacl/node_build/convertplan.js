var Fs = require('fs');

Fs.readFile(process.argv[process.argv.length-1], function (err, content) {
    if (err) { throw err; }

    var out = {
        PLAN_IMPLEMENTATIONS: [],
        PLAN_TYPES: [],
    };
    var lines = content.toString('utf8').split('\n');
    if (!(/PLAN_IMPLEMENTATIONS/.test(lines[0]))) { throw new Error(); }
    for (var i = 1; i < lines.length; i++) {
        var matches;
        if ((matches = /\"(crypto_[^\/]+)\/([^\/]+)\/([^\"]+)\"/.exec(lines[i]))) {
            out.PLAN_IMPLEMENTATIONS.push([matches[1], matches[2], matches[3]]);
        } else if ((matches = /\"(typedef [^\"]+)\"/.exec(lines[i]))) {
            out.PLAN_TYPES.push(matches[1]);
        }
    }
    console.log(JSON.stringify(out, null, '  '));
});
