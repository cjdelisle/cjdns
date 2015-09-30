var Fs = require('fs');
var Common = require('./Common');

module.exports.run = function(cc, config, onComplete) {

    var randomBytesImp = 'devurandom';

    var args = [];
    args.push(config.flag.compileOnly, config.flag.outputObj + Common.OBJECTS_INTERNAL + '/randombytes' + config.ext.obj);
    args.push('randombytes/' + randomBytesImp + '.c');

    Fs.readFile('randombytes/' + randomBytesImp + '.h', function(err, content) {
        if (err) {
            throw err;
        }
        Fs.writeFile(Common.INCLUDE_INTERNAL + '/randombytes.h', content, function(err) {
            if (err) {
                throw err;
            }
            cc(args, function(retCode, stdout, stderr) {
                if (stdout !== '') {
                    console.log(stdout);
                }
                if (stderr !== '') {
                    console.log(stderr);
                }
                if (retCode) {
                    throw new Error('failed to compile');
                }
                onComplete();
            });
        });
    });
};
