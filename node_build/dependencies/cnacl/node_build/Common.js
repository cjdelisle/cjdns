var Fs = require('fs');

var done = function(workers, onComplete) {
  if (!workers[0]) { throw new Error(); }
  if (!--workers[0]) { onComplete(); }
};

var BUILD_DIR = 'jsbuild';
var INCLUDE = BUILD_DIR + '/include';
var INCLUDE_INTERNAL = BUILD_DIR + '/include_internal';
var OBJECTS_INTERNAL = BUILD_DIR + '/objects_internal';
var TESTS = BUILD_DIR + '/tests';


var mkdir = function(dir) {
  if (Fs.existsSync(dir)) { return; }
  Fs.mkdirSync(dir);
};

var init = function() {
  mkdir(BUILD_DIR);
  mkdir(INCLUDE);
  mkdir(INCLUDE_INTERNAL);
  mkdir(OBJECTS_INTERNAL);
  mkdir(TESTS);
};

module.exports = {
  TESTS:TESTS,
  OBJECTS_INTERNAL:OBJECTS_INTERNAL,
  INCLUDE_INTERNAL:INCLUDE_INTERNAL,
  INCLUDE:INCLUDE,
  BUILD_DIR:BUILD_DIR,

  done:done,
  init:init,
};
