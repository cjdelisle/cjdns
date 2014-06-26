var Fs = require('fs');
var Common = require('./Common');

module.exports.run = function(cc, onComplete) {
  var args = ['-o',Common.OBJECTS_INTERNAL+'/randombytes.o','-c','randombytes/devurandom.c'];
  Fs.readFile('randombytes/devurandom.h', function(err, content) {
    if (err) { throw err; }
    Fs.writeFile(Common.INCLUDE_INTERNAL + '/randombytes.h', content, function(err) {
      if (err) { throw err; }
      cc(args, function(retCode, stdout, stderr) {
        if (stdout !== '') { console.log(stdout); }
        if (stderr !== '') { console.log(stderr); }
        if (retCode) { throw new Error('failed to compile'); }
        onComplete();
      });
    });
  });
};
