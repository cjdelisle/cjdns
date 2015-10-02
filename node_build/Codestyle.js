/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

var Fs = require('fs');
var nThen = require('nthen');
var Semaphore = require('../tools/lib/Semaphore');
var Child = require('child_process');

var headerLines = [
    '/* vim: set expandtab ts=4 sw=4: */',
    '/*',
    ' * You may redistribute this program and/or modify it under the terms of',
    ' * the GNU General Public License as published by the Free Software Foundation,',
    ' * either version 3 of the License, or (at your option) any later version.',
    ' *',
    ' * This program is distributed in the hope that it will be useful,',
    ' * but WITHOUT ANY WARRANTY; without even the implied warranty of',
    ' * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the',
    ' * GNU General Public License for more details.',
    ' *',
    ' * You should have received a copy of the GNU General Public License',
    ' * along with this program.  If not, see <http://www.gnu.org/licenses/>.',
    ' */'
];

var parseFile = function (fileName, fileContent) {
    var output = '';
    var parenthCount = 0;
    var functionParenthCount = 0;
    var expectBracket = 0;
    var name = fileName.replace(/^.*\//, '').replace(/\..*$/,'');

    var lines = fileContent.split('\n');

    var lineInfo = '';
    var ignore = false;

    var error = function(msg) {
        if (!ignore) {
            output += lineInfo + '  ' + msg + '\n';
        }
    };

    for (var lineNum = 0; lineNum < lines.length; lineNum++) {
        var line = lines[lineNum];

        // switch to 1 indexing for human readability
        lineInfo = fileName + ":" + (lineNum+1);
        ignore = false;

        if (lineNum < headerLines.length) {
            var expectedLine = headerLines[lineNum];
            if (line !== headerLines[lineNum]) {
                error("missing header\n" + expectedLine + "\n" + line);
            }
        } else if (/\.h$/.test(fileName) && lineNum < headerLines.length + 1) {
            if (line !== '#ifndef ' + name + "_H") {
                error("expected #ifndef " + name + "_H found " + line);
            }
        } else if (/\.h$/.test(fileName) && lineNum < headerLines.length + 2) {
            if (line !== '#define ' + name + "_H") {
                error("expected #define " + name + "_H found " + line);
            }
        }

        ignore = /CHECKFILES_IGNORE/.test(line);

        if (expectBracket === 1) {
            expectBracket = 0;
            if (!(/^[\s]*{/.test(line))) {
                error("expecting a { bracket " + line);
            }
        }


        // implementations..  TUNConfigurator_Linux contains TUNConfigurator_doStuff...
        var n = name.replace(/_.*/, '');
        if ((/^\w+\s.*\(/).test(line)) {
            if (!(/^int main\(/.test(line)
                || line.indexOf(' '+n) > -1
                || /^[ ]?static /.test(line)
                || /^typedef /.test(line)))
            {
                error("all globally visible functions must begin with the name of the file.");
            }
        }

        var matches;
        if (functionParenthCount === 0) {
            matches = /^\w+\s.*(\(.*)$/.exec(line);
        }
        if (functionParenthCount > 0 || matches) {
            var txt = (functionParenthCount > 0) ? line : matches[1];
            functionParenthCount += (txt.match(/\(/g)||[]).length;
            functionParenthCount -= (txt.match(/\)/g)||[]).length;
            if (functionParenthCount === 0) {
                txt = txt.substring(txt.lastIndexOf(')') + 1);
                if (/{/.test(txt)) {
                    error("please put the opening bracket on the next line.");
                }
            }
        }

        if (/[\w]*int[\w]*\s+\*+\w/.test(line) || /[\w]*struct\s+[\w]+\s+\*+\w/.test(line)) {
            error("int* blah; means int pointer named blah, int *blah; means int names splatblah");
        }

        if (line.length > 100) {
            error("cjd's editor window is only 100 characters wide");
        }

        if (/\.h$/.test(fileName) && fileName.indexOf('util/platform/libc/') === -1) {

            // If the name is CryptoAuth_pvt.h, it's ok to make a structure called CryptoAuth
            var nameRe = name.replace(/_pvt$/, '').replace(/_impl$/, '');

            if (/^struct /.test(line) && line.indexOf('struct ' + nameRe) !== 0 && !(/\(/.test(line))) {
                error("all structures must begin with the name of the file.");
            }

            if (/#define /.test(line) && line.indexOf('#define ' + nameRe) === -1) {
                error("all defines must begin with the name of the file.");
            }
        }
        if (/\t/.test(line)) {
            error("tabs are not allowed, use 4 spaces.");
        }

        if (/\s$/.test(line)) {
            error("trailing whitespace.");
        }

        if (/[^A-Z](TODO|FIXME|XXX)[^A-Z]/.test(line)) {
            if (/[^A-Z](TODO|FIXME|XXX)[^\(A-Z]/.test(line)) {
                error("Please take responsibility for your TODO: eg: // TODO(cjd): make this work");
            } else {
                console.log(lineInfo + ' ' + line.replace(/[ \/]*/, ''));
            }
        }

        if (/(if|for|while)\(/.test(line)) {
            error("If/for/while statements must be followed by whitespace.");
        }

        matches = null;
        if (parenthCount === 0) {
            matches = /[^\w#](if|for|while) (\(.*$)/.exec(line);
        }
        if (parenthCount > 0 || matches) {
            var txt1 = (parenthCount > 0) ? line : matches[2];
            parenthCount += (txt1.match(/\(/g)||[]).length;
            parenthCount -= (txt1.match(/\)/g)||[]).length;
            if (parenthCount === 0) {
                txt1 = txt1.substring(txt1.lastIndexOf(')') + 1);
                // for (x; y; z) ;         <-- ok
                // for (x; y; z) {         <-- ok
                // for (x; y; z) {   \     <-- ok (in preprocessor macro)
                // for (x; y; z)           <-- ok but you better put a bracket on the next line
                // for (x; y; z) { j++; }  <-- ok
                // for (x; y; z) j++;      <-- BZZZZZZZZZZT
                if (!(/^[\s]*[;{].*$/.test(txt1)) && !(/^[\s]+{[\s]*\\$/).test(txt1)) {
                    if (/[\s]*$/.test(txt1)) {
                        expectBracket = 1;
                    } else {
                        error(parenthCount + '  ' + line);
                    }
                }
            }
        }
    }
    return output;
};

var checkFile = module.exports.checkFile = function (file, callback) {
    Fs.readFile(file, function (err, ret) {
        if (err) { throw err; }
        callback(parseFile(file, ret.toString()));
    });
};

var lint = module.exports.lint = function (fileName, fileContent, callback) {
    var out = parseFile(fileName, fileContent);
    callback(out, !!out);
};

var checkFiles = module.exports.checkFiles = function (files, callback) {
    var sema = Semaphore.create(64);
    var errors = '';
    nThen(function (waitFor) {
        files.forEach(function (file) {
            sema.take(waitFor(function (returnAfter) {
                checkFile(file, waitFor(returnAfter(function (err) {
                    if (err) {
                        errors += file + '\n' + err + '\n';
                    }
                })));
            }));
        });
    }).nThen(function (waitFor) {
        callback(errors);
    });
};

var checkDir = module.exports.checkDir = function (dir, runInFork, callback) {

    var gitIgnoreLines;

    if (runInFork) {
        var err = '';
        var out = '';
        var proc = Child.spawn(process.execPath, [__filename]);
        proc.stdout.on('data', function (data) { err += data.toString('utf8'); });
        proc.stderr.on('data', function (data) { err += data.toString('utf8'); });
        proc.on('close', function (ret) {
            out += err;
            var error;
            if (ret !== 0) { error = new Error(out); }
            callback(error, out);
        });
        return;
    }

    var output = '';
    nThen(function (waitFor) {

        Fs.readFile('.gitignore', waitFor(function (err, ret) {
            if (err) { throw err; }
            gitIgnoreLines = ret.toString('utf8').split('\n');
        }));

    }).nThen(function (waitFor) {

        var addDir = function (dir) {
            Fs.readdir(dir, waitFor(function (err, files) {
                if (err) { throw err; }
                files.forEach(function (file) {
                    Fs.stat(dir + '/' + file, waitFor(function (err, stat) {
                        if (err) { throw err; }
                        if (file === '.git') {
                        } else if (file === 'contrib') {
                        } else if (file === 'dependencies') {
                        } else if (gitIgnoreLines.indexOf(file) !== -1) {
                        } else {
                            if (stat.isDirectory()) {
                                addDir(dir + '/' + file);
                            } else if (/.*\.[ch]$/.test(file)) {
                                checkFile(dir + '/' + file, waitFor(function (ret) {
                                    output += ret;
                                }));
                            }
                        }
                    }));
                });
            }));
        };
        addDir(dir);

    }).nThen(function (waitFor) {

        callback(output);

    });
};

if (module.parent === null) {
    checkDir('.', false, function(output) {
        if (output !== '') {
            console.log(output);
            process.exit(1);
        }
    });
}
