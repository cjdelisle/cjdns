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
        flags.push.apply(flags, ["-x", "c", "-o", outputFile, file]);
        flags.push.apply(flags, ldflags);

        builder.cc(flags, waitFor(function (ret, out, err) {
            if (ret && /undefined reference/.test(err)) {
                callback(undefined, false);
            } else if (ret) {
                callback(new Error(err));
            } else {
                callback(undefined, true);
            }
        }));

    });
};
