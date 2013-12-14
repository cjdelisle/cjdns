var nThen = require("nthen");
var Fs = require("fs");

module.exports.check = function (builder, func, ldflags, callback) {

    var file = builder.tmpFile();
    var outputFile = builder.tmpFile();

    nThen(function (waitFor) {

        Fs.writeFile(file, "int main() { " + func + "(); }", waitFor(function (err, ret) {
            if (err) {
                waitFor.abort();
                callback(err);
            }
        }));

    }).nThen(function (waitFor) {

        var flags = [];
        flags.push.apply(flags, ldflags);
        flags.push.apply(flags, ["-x", "c", "-o", outputFile, file]);

        builder.cc(flags, waitFor(function (err, ret) {
            if (err && /undefined reference/.test(err.message)) {
                callback(undefined, false);
            } else if (err) {
                callback(err);
            } else {
                callback(undefined, true);
            }
        }));

    });
};
