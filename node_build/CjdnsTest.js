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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
'use strict';
const Fs = require("fs");
const nThen = require("nthen");

const Fs_stat = function (file, callback) {
    Fs.stat(file, function (err, ret) {
        if (err === 'ENOENT') {
            console.log("Magical ENOENT on a file which definitely does exist, good job Linus");
            setTimeout(function () {
                Fs_stat(file, callback);
            }, 1000);
        } else {
            callback(err, ret);
        }
    });
};

const getTests = function (file, tests, isSubnode, callback) {
    if (/\/(.git|build_.*|node_build|contrib|target)\//.test(file)) { callback(); return; }
    if (isSubnode && /\/dht\//.test(file)) { callback(); return; }
    Fs_stat(file, function (err, stat) {
        if (err) { throw err; }
        if (stat.isDirectory()) {
            nThen(function (waitFor) {
                Fs.readdir(file, waitFor(function (err, list) {
                    if (err) { throw err; }
                    list.forEach(function (subFile) {
                        getTests(file + '/' + subFile, tests, isSubnode, waitFor());
                    });
                }));
            }).nThen(function (waitFor) {
                callback();
            });
            return;
        } else if (!/_test\.c$/.test(file)) {
            // not a test
        } else {
            // get rid of the annoying ./
            const f = file.replace(/^\.\//, '');
            if (tests.indexOf(f) === -1) { tests.push(f); }
        }
        callback();
    });
};

const rmFuzz = function (builder, cb) {
    const inputs = builder.config.buildDir + '/fuzz_inputs';
    Fs_stat(inputs, function (err, stat) {
        if (err && err.code === 'ENOENT') {
            Fs.mkdir(inputs, function (err, ret) {
                if (err) { throw err; }
                return void cb();
            });
            return;
        } else if (err) {
            throw err;
        }
        if (!stat.isDirectory()) {
            throw new Error(inputs + ' found, and it is a file, not a directory');
        }
        Fs.readdir(inputs, function (err, list) {
            if (err) { throw err; }
            let nt = nThen;
            list.forEach(function (f) {
                nt = nt(function (w) {
                    Fs.unlink(inputs + '/' + f, w(function (err) {
                        if (err) { throw err; }
                    }));
                }).nThen;
            });
            nt(function (w) { cb(); });
        });
    });
};

const mkFuzzCase = function (inFile, outPath, testId, cb) {
    Fs.readFile(inFile, 'utf8', function (err, ret) {
        if (err) { throw err; }
        ret = ret.replace(/#[^\n]*\n/g, '');
        ret = ret.replace(/[\s]*/g, '');
        const out = Buffer.from(ret, 'hex');
        const id = Buffer.alloc(4);
        id.writeInt32BE(testId, 0);
        Fs.writeFile(outPath, Buffer.concat([id, out]), function (err) {
            if (err) { throw err; }
            cb();
        });
    });
};

const mkFuzz = function (builder, testPath, testId, fuzzCases, cb) {
    const inputs = builder.config.buildDir + '/fuzz_inputs';
    nThen(function (w) {
        const casesPath = testPath.replace(/\.c$/, '_cases');
        Fs.readdir(casesPath, w(function (err, list) {
            if (err && err.code === 'ENOENT') {
                return void console.log("Fuzz test [" + testPath + "] has no test cases");
            }
            if (err) { throw err; }
            let nt = nThen;
            list.forEach(function (f) {
                nt = nt(function (w) {
                    const fNoExt = f.replace(/\..*$/, '');
                    const outPath = inputs + '/' + (testPath + fNoExt).replace(/[^a-zA-Z0-9_-]/g, '_');
                    fuzzCases.push('"' + outPath + '",');
                    const inFile = casesPath + '/' + f;
                    mkFuzzCase(inFile, outPath, testId, w());
                }).nThen;
            });
            nt(w());
        }));
    }).nThen(function (w) {
        cb();
    });
};

module.exports.generate = function (builder, isSubnode, callback) {
    const tests = [];
    const fuzzCases = [];
    const prototypes = [];
    const listContent = [];
    const fuzzTests = [];
    builder.config.cjdnsTest_files = builder.config.cjdnsTest_files || [];

    nThen(function (w) {
        getTests('.', tests, isSubnode, w());
        rmFuzz(builder, w());
    }).nThen(function (w) {
        tests.forEach(function (test) {
            //console.log(test);
            const testProto = /^.*\/([^\/]+)\.c$/.exec(test)[1];
            builder.config.cjdnsTest_files.push(test);
            const cflags = (builder.config.fileCflags[test] = builder.config.fileCflags[test] || []);
            if (test.indexOf('_fuzz_test') > -1) {
                cflags.push(
                    '-D', 'CJDNS_FUZZ_INIT='+testProto+'_FUZZ_INIT',
                    '-D', 'CJDNS_FUZZ_MAIN='+testProto+'_FUZZ_MAIN'
                );
                prototypes.push(
                    'void* '+testProto+'_FUZZ_INIT(struct Allocator* alloc, struct Random* rand);',
                    'void '+testProto+'_FUZZ_MAIN(void* ctx, struct Message* fuzz);'
                );
                mkFuzz(builder, test, fuzzTests.length, fuzzCases, w());
                fuzzTests.push('{' +
                    '.init = '+testProto+'_FUZZ_INIT, ' +
                    '.name = "'+test.replace(/^.*\/|.c$/g, '')+'", ' +
                    '.fuzz = '+testProto+'_FUZZ_MAIN, ' +
                '},');
            } else {
                var main = testProto + '_main';
                cflags.push(
                    '-D', 'main='+main,
                    '-D', main+'(...)='+main+'(int argc, char** argv);int '+main+'(int argc, char** argv)'
                );
                prototypes.push('int '+main+'(int argc, char** argv);');
                listContent.push('{ .func = '+main+', .name = "'+test.replace(/^.*\/|.c$/g, '')+'" },');
            }
        });
    }).nThen(function (w) {
        builder.config.cjdnsTest_fuzzCases = fuzzCases.join(' ');
        builder.config.cjdnsTest_tests = listContent.join(' ');
        builder.config.cjdnsTest_fuzzTests = fuzzTests.join(' ');
        builder.config.cjdnsTest_prototypes = prototypes.join(' ');
        callback();
    });
};
