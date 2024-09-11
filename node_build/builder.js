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
/*@flow*/
'use strict';
const Os = require('os');
const Fs = require('fs');
const Spawn = require('child_process').spawn;
const nThen = require('nthen');
const Crypto = require('crypto');
const Saferphore = require('saferphore');

var seed = process.env.SOURCE_DATE_EPOCH || Crypto.randomBytes(32).toString('hex');

/*::
export type Builder_File_t = {|
    includes: string[],
    links: string[],
    cflags: string[],
    ldflags: string[],
    mtime: number,
|};
export type Builder_Hfile_t = {|
    mtime: number,
|};
export type Builder_Compiler_t = {|
    isLLVM: bool,
    isClang: bool,
    isGCC: bool,
    version: string
|};
export type Builder_State_t = {|
    compilerType: Builder_Compiler_t,
    cFiles: { [string]: Builder_File_t },
    hFiles: { [string]: Builder_Hfile_t },
|};
export type Builder_Linter_t = (string, string, (string, bool)=>void)=>void;
export type Builder_TestRunnerCb_t = (string, bool)=>void;
export type Builder_t = {|
    cc: (string[], (number, string, string)=>void)=>void,
    buildLibrary: (string)=>void,
    buildExecutable: (string, ?string)=>void,
    buildTest: (string)=>string,
    runTest: (string, (string, Builder_TestRunnerCb_t)=>void)=>void,
    lintFiles: (Builder_Linter_t)=>void,
    config: Builder_Config_t,
    tmpFile: (?string)=>string,
    compilerType: () => Builder_Compiler_t,
    fileCflags: {[string]: string[]},
|};
export type Builder_BaseConfig_t = {|
    systemName?: ?string,
    gcc?: ?string,
    buildDir?: ?string,
|};
export type Builder_Config_t = {
    systemName: string,
    gcc: string,
    buildDir: string,
    includeDirs: string[],
    cflags: string[],
    ldflags: string[],
    libs: string[],
    jobs: number,
} & {[string]:any};
import type { Nthen_WaitFor_t } from 'nthen';
import type { Saferphore_t } from 'saferphore';
export type Builder_Stage_t = (Builder_t, Nthen_WaitFor_t)=>void;
export type Builder_CompileJob_t = {
    cFile: string,
    outputFile: ?string,
    type: 'exe'|'lib'
};
export type Builder_Pub_t = {|
    build: (Builder_Stage_t)=>Builder_Pub_t,
    test: (Builder_Stage_t)=>Builder_Pub_t,
    pack: (Builder_Stage_t)=>Builder_Pub_t,
    failure: (Builder_Stage_t)=>Builder_Pub_t,
    success: (Builder_Stage_t)=>Builder_Pub_t,
    complete: (Builder_Stage_t)=>Builder_Pub_t,
|}
export type Builder_PreCtx_t = {
    buildStage: Builder_Stage_t,
    testStage: Builder_Stage_t,
    packStage: Builder_Stage_t,
    failureStage: Builder_Stage_t,
    successStage: Builder_Stage_t,
    completeStage: Builder_Stage_t,

    failure: bool,
    linters: Builder_Linter_t[],
    executables: Array<Builder_CompileJob_t>,
    tests: Array<(Builder_TestRunnerCb_t)=>void>,
    toCompile: { [string]: Builder_File_t },
    config: Builder_Config_t,
    sema: Saferphore_t,
};
export type Builder_Ctx_t = Builder_PreCtx_t & {
    builder: Builder_t,
    state: Builder_State_t,
};
*/

const error = function (message) /*:Error*/ {
    try {
        throw new Error(message);
    } catch (e) {
        return e;
    }
};

const expandArgs = function (args) {
    const out = [];
    for (let i = 0; i < args.length; i++) {
        if (typeof(args[i]) === 'object') {
            if (Array.isArray(args[i])) {
                out.push.apply(out, expandArgs(args[i]));
            } else {
                throw new Error("object in arguments [" + args.join() + "]");
            }
        } else {
            out.push(args[i]);
        }
    }
    return out;
};

const compiler = function (
    ctx /*:Builder_Ctx_t*/,
    args /*:string[]*/,
    callback /*:(number, string, string)=>bool|void*/,
    content /*:string*/
) {
    let stop = false;
    args = expandArgs(args);
    ctx.sema.take(function (returnAfter) {
        if (stop) {
            return void returnAfter(function (ret) {
                callback(1, '', 'interrupted');
            });
        }
        if (process.env.VERBOSE) {
            console.log(ctx.config.gcc + ' ' + args.join(' '));
        }
        const gcc = Spawn(ctx.config.gcc, args);
        let err = '';
        let out = '';

        gcc.stdout.on('data', function (dat) { out += dat.toString(); });
        gcc.stderr.on('data', function (dat) { err += dat.toString(); });
        gcc.on('close', returnAfter(function (ret) {
            if (callback(ret, out, err)) { stop = true; }
        }));

        gcc.on('error', function (err) {
            if (err.code === 'ENOENT') {
                console.error('\x1b[1;31mError: ' + ctx.config.gcc + ' is required!\x1b[0m');
            } else {
                console.error(
                    '\x1b[1;31mFail run ' + process.cwd() + ': ' + ctx.config.gcc + ' '
                    + args.join(' ') + '\x1b[0m'
                );
                console.error('Message:' + err);
            }

            // handle the error safely
            console.log(args);
        });

        if (content) {
            gcc.stdin.write(content, function (err) {
                if (err) { throw err; }
                gcc.stdin.end();
            });
        }
    });
};

const cc = function (
    ctx /*:Builder_Ctx_t*/,
    args /*:string[]*/,
    callback /*:(?Error, ?string)=>bool|void*/,
    content /*:string*/
) {
    compiler(ctx, args, function (ret, out, err) {
        if (ret) {
            return callback(error(ctx.config.gcc + " " + args.map(String).join(' ') + "\n\n" + err));
        }

        if (err !== '') {
            //process.stdout.write(err);
        }

        return callback(undefined, out);
    }, content);
};

const getStatePrototype = function () /*:Builder_State_t*/ {
    return {
        compilerType: {
            isLLVM: false,
            isClang: false,
            isGCC: false,
            version: ''
        },

        cFiles: {},
        hFiles: {},
    };
};

const tmpFile = function (ctx /*:Builder_Ctx_t*/, name) {
    name = name || '';
    return ctx.config.buildDir + '/tmp/' + name + Crypto.pseudoRandomBytes(10).toString('hex');
};

const finalizeCtx = function (
    state /*:Builder_State_t*/,
    pctx /*:Builder_PreCtx_t*/
) /*:Builder_Ctx_t*/ {
    const ctx = ((pctx /*:any*/) /*:Builder_Ctx_t*/);
    ctx.state = state;
    ctx.builder = (Object.freeze({
        cc: function (args, callback) {
            compiler(ctx, args, callback, '');
        },

        buildLibrary: function (cFile) {
            ctx.executables.push({ cFile, outputFile: null, type: 'lib' });
        },

        buildExecutable: function (cFile, outputFile) {
            ctx.executables.push({ cFile, outputFile, type: 'exe' });
        },

        buildTest: function (cFile) {
            const outputFile = getTempExe(ctx, cFile);
            ctx.executables.push({ cFile, outputFile, type: 'exe' });
            return outputFile;
        },

        runTest: function (outFile, testRunner) {
            ctx.tests.push(function (cb) { testRunner(outFile, cb); });
        },

        lintFiles: function (linter) {
            ctx.linters.push(linter);
        },

        config: ctx.config,

        tmpFile: function (name) {
            return tmpFile(ctx, name);
        },

        compilerType: () => JSON.parse(JSON.stringify(ctx.state.compilerType)),

        fileCflags: {},

    }) /*:Builder_t*/);
    return ctx;
};

// You Were Warned
const execJs = function (js, ctx, file, fileName, callback, thisObj) {
    let res;
    let x;
    let err;

    // # 74 "./wire/Message.h"
    js = js.replace(/\n#.*\n/g, '');

    // Js_SQ Js_DQ
    const qs = js.split('Js_Q');
    if (qs.length && (qs.length % 2) === 0) {
        throw new Error("Uneven number of Js_Q, content: [" + js + "]");
    }
    for (let i = 1; i < qs.length; i += 2) {
        // escape nested quotes, they'll come back out in the final .i file
        qs[i] = qs[i].replace(/\'/g, '\\u0027');
    }
    js = '"use strict";' + qs.join("'");

    const to = setTimeout(function () {
        throw new Error("Inline JS did not return after 120 seconds [" + js + "]");
    }, 120000);

    nThen(function (waitFor) {

        try {
            /* jshint -W054 */ // Suppress jshint warning on Function being a form of eval
            const func = new Function('require', 'js', 'console', 'builder', js);

            const jsObj = Object.freeze({
                async: function () {
                    return waitFor(function (result) {
                        res = result;
                    });
                },
                linkerDependency: (cFile) => file.links.push(cFile),
                currentFile: fileName,
            });

            x = func.call(thisObj,
                          require,
                          jsObj,
                          console,
                          ctx.builder);
        } catch (e) {
            clearTimeout(to);
            console.error("Error executing: [" + js + "] in File [" + fileName + "]");
            throw e;
        }

    }).nThen(function (waitFor) {

        if (err) { return; }
        res = res || x || '';
        clearTimeout(to);
        process.nextTick(function () { callback(undefined, res); });

    });
};

const debug = console.log;

const preprocessBlock = function (block, ctx, fileObj, fileName, callback, thisObj) {
    // a block is an array of strings and arrays, any inside arrays must be
    // preprocessed first. deep first top to bottom.

    let nt = nThen;

    block.forEach(function (elem, i) {
        if (typeof(elem) === 'string') { return; }

        nt = nt(function (waitFor) {
            preprocessBlock(elem, ctx, fileObj, fileName, waitFor(function (err, ret) {
                if (err) { throw err; }

                block[i] = ret;
            }), thisObj);
        }).nThen;
    });

    nt(function (waitFor) {
        const capture = block.join('');
        execJs(capture, ctx, fileObj, fileName, waitFor(function (err, ret) {
            if (err) { throw err; }
            callback(undefined, ret);
        }), thisObj);
    });
};

const preprocess = function (content /*:string*/, ctx, fileObj, fileName, callback) {
    // <?js file.Test_mainFunc = "<?js return 'RootTest_'+file.RootTest_mainFunc; ?>" ?>
    // worse:
    // <?js file.Test_mainFunc = "<?js const done = this.async(); process.nextTick(done); ?>" ?>

    const flatArray = content.split(/(<\?js|\?>)/);
    const elems = [];
    const unflatten = function (array, startAt, out) {
        let i = startAt;
        for (; i < array.length; i++) {
            if (((i - startAt) % 2) === 0) {
                out.push(array[i]);
            } else if (array[i] === '<?js') {
                const next = [];
                out.push(next);
                i = unflatten(array, i+1, next);
            } else if (array[i] === '?>') {
                return i;
            }
        }

        return i;
    };

    if (unflatten(flatArray, 0, elems) !== flatArray.length) {
        throw new Error();
    }

    const thisObj = {};

    let nt = nThen;
    elems.forEach(function (elem, i) {
        if (typeof(elem) === 'string') { return; }

        nt = nt(function (waitFor) {
            preprocessBlock(elem, ctx, fileObj, fileName, waitFor(function (err, ret) {
                if (err) { throw err; }

                elems[i] = ret;
            }), thisObj);
        }).nThen;
    });

    nt(function (waitFor) {
        callback(undefined, elems.join(''));
    });
};

const mkFile = function () /*:Builder_File_t*/ {
    return {
        includes: [],
        links: [],
        cflags: [],
        ldflags: [],
        mtime: 0,
    };
};

const getOFile = function (ctx, cFile) {
    return ctx.config.buildDir + '/' + cFile.replace(/[^a-zA-Z0-9_-]/g, '_') + '.o';
};
const getIFile = function (ctx, cFile) {
    return ctx.config.buildDir + '/' + cFile.replace(/[^a-zA-Z0-9_-]/g, '_') + '.i';
};
const getTempExe = function (ctx, cFile) {
    return ctx.config.buildDir + '/' + cFile.replace(/[^a-zA-Z0-9_-]/g, '_');
};
const getExeFile = function (ctx, exe /*:Builder_CompileJob_t*/) {
    let outputFile = exe.outputFile;
    if (!outputFile) {
        outputFile = exe.cFile.replace(/^.*\/([^\/\.]*).*$/, (a, b) => b);
    }
    if (ctx.config.systemName === 'win32' && !(/\.exe$/.test(outputFile))) {
        outputFile += '.exe';
    }
    return outputFile;
};

var randomHex = function (bytes, fileName) {
    var material = new Crypto.Hash('sha512').update(seed).update(fileName).digest();
    if (bytes > 64) { throw new Error("meh, randomHex of over 64 bytes is unimplemented"); }
    return material.slice(0, bytes).toString('hex');
};

const getFlags = function (ctx, cFile, includeDirs) {
    const flags = [];
    if (cFile.indexOf('node_build/dependencies/libuv') > -1) {
        //console.log('cargo:warning=' + cFile);
        for (const f of ctx.config.cflags) {
            if (f !== '-Werror') {
                flags.push(f);
            }
        }
    } else {
        flags.push.apply(flags, ctx.config.cflags);
    }
    flags.push.apply(flags, ctx.builder.fileCflags[cFile] || []);
    flags.push('-DCJDNS_RAND_U64_PER_FILE=0x' + randomHex(8, cFile) + 'ull');
    if (includeDirs) {
        for (let i = 0; i < ctx.config.includeDirs.length; i++) {
            if (flags[flags.indexOf(ctx.config.includeDirs[i])-1] === '-I') {
                continue;
            }
            flags.push('-I');
            flags.push(ctx.config.includeDirs[i]);
        }
    }
    return flags;
};

const preprocessFile = function (cFile, ctx, callback)
{
    if (ctx.state.cFiles[cFile]) {
        return void callback();
    }
    const state = ctx.state;
    //debug('  preprocessing ' + cFile);

    //debug('\x1b[2;32mCompiling ' + cFile + '\x1b[0m');

    const fileObj = mkFile();
    let fileContent = '';
    const cflags = getFlags(ctx, cFile, true);
    fileObj.cflags = getFlags(ctx, cFile, false);

    nThen((w) => {
        //debug("CPP");
        cc(ctx, ['-E', ...cflags, cFile], w(function (err, output) {
            if (err) { throw err; }
            fileContent = output;
            return false;
        }), '');

        // Stat the C file
        Fs.stat(cFile, w(function (err, st) {
            if (err) { throw err; }
            fileObj.mtime = st.mtime.getTime();
        }));
    }).nThen((w) => {
        //debug("Preprocess");
        preprocess(fileContent, ctx, fileObj, cFile, w(function (err, output) {
            if (err) { throw err; }
            Fs.writeFile(getIFile(ctx, cFile), output, w(function (err) {
                if (err) { throw err; }
            }));
        }));

        // Also snatch the local includes
        const includes = fileContent.match(/# [0-9]+ "\.\/[^"]*"/g) || [];
        const uniqIncl = {};
        for (const incl of includes) {
            uniqIncl[incl.replace(/^.* "\.\//, '').slice(0,-1)] = 1;
        }
        fileObj.includes = Object.keys(uniqIncl);
        fileObj.includes.forEach((incl) => {
            if (ctx.state.hFiles[incl]) { return; }
            Fs.stat(incl, w((err, st) => {
                if (err) { throw err; }
                ctx.state.hFiles[incl] = {
                    mtime: st.mtime.getTime()
                };
            }));
        });

    }).nThen(function (_) {
        debug('\x1b[2;36mPreprocessing ' + cFile + ' complete\x1b[0m');
        state.cFiles[cFile] = fileObj;
        ctx.toCompile[cFile] = fileObj;
        callback();
    });
};

const preprocessFiles = function (ctx, files, callback) {
    const added = {};
    for (const f of files) { added[f] = 1; }
    const doMore = () => {
        if (files.length === 0) {
            return void callback();
        }
        const filez = files;
        files = [];
        nThen((w) => {
            filez.forEach((file) => {
                preprocessFile(file, ctx, w(() => {
                    ctx.state.cFiles[file].links.forEach(function (link) {
                        if (link === file || added[link]) {
                            return;
                        }
                        added[link] = 1;
                        files.push(link);
                    });
                }));
            });
        }).nThen((w) => {
            doMore();
        });
    };
    doMore();
};

const getLinkOrder = function (cFile, files) /*:string[]*/ {
    const completeFiles = [];

    const getFile = function (name) {
        const f = files[name];
        //debug('Resolving links for ' + name);

        for (let i = 0; i < f.links.length; i++) {
            if (f.links[i] === name) {
                continue;
            }

            if (completeFiles.indexOf(f.links[i]) > -1) {
                continue;
            }

            getFile(f.links[i]);
        }

        completeFiles.push(name);
    };

    getFile(cFile);

    return completeFiles;
};

// Called on the .c file with a main() function which corrisponds to
// an executable.
// We kick the file entries right out of the state object when they
// or an #include get dirty, so we just need to traverse links to
// make sure everything is present.
const needsToLink = function (ctx, cFile) {
    const nlCache = {};
    const nll = [];
    const nl = (cFile) => {
        if (nlCache[cFile]) { return false; }
        if (nll.indexOf(cFile) > -1) {
            return false;
            //throw new Error(`File ${cFile} is self-referencial:\n${nll.join('\n')}\n\n`);
        }
        nll.push(cFile);
        const out = (() => {
            //debug('  ' + cFile);
            if (typeof(ctx.state.cFiles[cFile]) !== 'object') {
                return true;
            }
            for (const l of ctx.state.cFiles[cFile].links) {
                if (l !== cFile && nl(l)) {
                    return true;
                }
            }
            nlCache[cFile] = true;
            return false;
        })();
        if (nll.pop() !== cFile) { throw new Error(); }
        return out;
    };
    return nl(cFile);
};

const makeTime = function () {
    let time = 0;
    return function () {
        const oldTime = time;
        time = new Date().getTime();
        return time - oldTime;
    };
};

const link = function (cFile, callback, ctx /*:Builder_Ctx_t*/) {
    const state = ctx.state;
    const temp = getTempExe(ctx, cFile);
    nThen((waitFor) => {
        const linkOrder = getLinkOrder(cFile, state.cFiles);
        for (let i = 0; i < linkOrder.length; i++) {
            linkOrder[i] = getOFile(ctx, linkOrder[i]);
        }
        const fileObj = state.cFiles[cFile];
        const ldArgs = []
            .concat(ctx.config.ldflags)
            .concat(fileObj.ldflags)
            .concat(['-o', temp])
            .concat(linkOrder)
            .concat(ctx.config.libs);
        debug('\x1b[1;31mLinking C executable ' + cFile + '\x1b[0m');
        cc(ctx, ldArgs, waitFor(function (err, ret) {
            if (err) { throw err; }
            return false;
        }), '');
    }).nThen((_) => callback());
};

const compile = function (ctx, cFile, done) {
    //debug("CC");
    const file = ctx.state.cFiles[cFile];
    const oFile = getOFile(ctx, cFile);
    const iFile = getIFile(ctx, cFile);
    cc(ctx, ['-c', '-x', 'cpp-output', '-o', oFile, ...file.cflags, iFile], (err) => {
        done(err);
        return typeof(err) !== 'undefined';
    }, '');
};

/**
 * Get a copy of process.env with a few entries which are constantly changing removed.
 * This prevents isStaleState from returning true every time one builds in a different
 * window.
 */
const normalizedProcessEnv = function () {
    const out = process.env;
    delete out.WINDOWID;
    delete out.OLDPWD;
    return out;
};

const getRebuildIfChangesHash = function (rebuildIfChanges, callback) {
    const hash = Crypto.createHash('sha256');
    const rebIfChg = [];

    nThen(function (waitFor) {

        rebuildIfChanges.forEach(function (fileName, i) {
            Fs.readFile(fileName, waitFor(function (err, ret) {
                if (err) { throw err; }

                rebIfChg[i] = ret;
            }));
        });

        hash.update(JSON.stringify(normalizedProcessEnv()));

    }).nThen(function (waitFor) {

        rebIfChg.forEach(function (data) {
            hash.update(data);
        });

        callback(hash.digest('hex'));

    });
};

const probeCompiler = function (ctx /*:Builder_Ctx_t*/, callback) {
    nThen(function (waitFor) {
        const compilerType = ctx.state.compilerType = {
            isLLVM: false,
            isClang: false,
            isGCC: false,
            version: ''
        };
        compiler(ctx, ['-v'], waitFor(function (ret, out, err) {
            // TODO(cjd): afl-clang-fast errors when called with -v
            //if (ret !== 0) { throw new Error("Failed to probe compiler ret[" + ret + "]\n" + err); }
            if (/Apple LLVM version /.test(err)) {
                compilerType.isLLVM = true;
                if (/clang/.test(err)) {
                    // Apple LLVM version 6.1.0 (clang-602.0.53) (based on LLVM 3.6.0svn)
                    // Target: x86_64-apple-darwin14.4.0
                    // Thread model: posix
                    compilerType.isClang = true;
                    compilerType.version = err.match(/Apple LLVM version ([^ ]+) /)[1];
                } else if (/gcc version /.test(err)) {
                    // Using built-in specs.
                    // Target: i686-apple-darwin11
                    // Configured with: /private/const/tmp/llvmgcc42/llvmgcc42.......
                    // Thread model: posix
                    // gcc version 4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2336.11.00)
                    compilerType.isGCC = true;
                    compilerType.version = err.match(/gcc version ([^ ]+) /)[1];
                }
            } else if (/clang version /.test(err)) {
                // FreeBSD clang version 3.0 (tags/RELEASE_30/final 145349) 20111210
                // Target: x86_64-unknown-freebsd10.0
                // Thread model: posix

                // clang version 3.2 (trunk)
                // Target: x86_64-unknown-linux-gnu
                // Thread model: posix
                compilerType.isLLVM = true;
                compilerType.isClang = true;
                compilerType.version = err.match(/clang version ([^ ]+) /)[1];
            } else if (/gcc version /.test(err)) {
                compilerType.isGCC = true;
                compilerType.version = err.match(/gcc version ([^ ]+) /)[1];
            }
            //console.log(JSON.stringify(compilerType));
        }), '');
    }).nThen(callback);
};

process.on('exit', function () {
    console.log("Total build time: " + Math.floor(process.uptime() * 1000) + "ms.");
});

const deepFreeze = (obj) => {
    Object.freeze(obj);
    for (const k in obj) {
        if (typeof(obj[k]) === 'object') { deepFreeze(obj[k]); }
    }
};

const sweep = (path, done) => {
    let files = [];
    nThen((w) => {
        Fs.readdir(path, w((err, fls) => {
            if (err) { throw err; }
            files = fls;
        }));
    }).nThen((w) => {
        files.forEach((f) => {
            const file = path + '/' + f;
            Fs.stat(file, w((err, st) => {
                if (err) { throw err; }
                if (st.isDirectory()) {
                    sweep(file, w(() => {
                        Fs.rmdir(file, w((err) => {
                            if (err) { throw err; }
                        }));
                    }));
                } else {
                    Fs.unlink(file, w((err) => {
                        if (err) { throw err; }
                    }));
                }
            }));
        });
    }).nThen((_) => done());
};

module.exports.configure = function (
    params /*:Builder_BaseConfig_t*/,
    configFunc /*:(Builder_t, Nthen_WaitFor_t)=>void*/
) /*:Builder_Pub_t*/ {

    // Track time taken for various steps
    const time = makeTime();
    time();

    const systemName = params.systemName || process.platform;
    const buildDir = params.buildDir || 'build_' + systemName;

    let gcc;
    if (params.gcc) {
        gcc = params.gcc;
    } else if (systemName === 'openbsd') {
        gcc = 'egcc';
    } else if (systemName === 'freebsd') {
        gcc = 'clang';
    } else {
        gcc = 'gcc';
    }

    // Since many of the compile operations are short, the best
    // performance seems to be when running 1.25x the number of jobs as
    // cpu cores. On BSD and iphone systems, os.cpus() is not reliable so
    // if it returns undefined let's just assume 1
    // workaround, nodejs seems to be broken on openbsd (undefined result after second call)
    const cpus = Os.cpus();
    const jobs = Math.floor((typeof cpus === 'undefined' ? 1 : cpus.length) * 1.25);

    const pctx /*:Builder_PreCtx_t*/ = {
        buildStage: (_x,_y)=>{},
        testStage: (_x,_y)=>{},
        packStage: (_x,_y)=>{},
        failureStage: (_x,_y)=>{},
        successStage: (_x,_y)=>{},
        completeStage: (_x,_y)=>{},

        failure: false,
        linters: [],
        executables: [],
        tests: [],
        toCompile: {},
        sema: Saferphore.create(1),

        config: {
            buildDir,
            gcc,
            systemName,

            version: '',
            includeDirs: ['.'],
            cflags: [],
            ldflags: [],
            libs: [],
            jobs,
        },
    };
    let state = getStatePrototype();
    let ctx;
    let hasState = false;

    nThen(function (waitFor) {

        // make the build directory
        Fs.exists(buildDir, waitFor(function (exists) {
            if (exists) { return; }

            Fs.mkdir(buildDir, {}, waitFor(function (err) {
                if (err) { throw err; }
            }));
        }));

    }).nThen(function (waitFor) {

        Fs.exists(buildDir + '/tmp', waitFor(function (exists) {
            if (exists) {
                sweep(buildDir + '/tmp', waitFor());
            } else {
                Fs.mkdir(buildDir + '/tmp', {}, waitFor(function (err) {
                    if (err) { throw err; }
                }));
            }
        }));

    }).nThen(function (waitFor) {

        if (process.env['CJDNS_FULL_REBUILD']) {
            debug("CJDNS_FULL_REBUILD set, non-incremental build");
            return;
        }

        // read out the state if it exists
        Fs.exists(buildDir + '/state.json', waitFor(function (exists) {
            if (!exists) { return; }
            Fs.readFile(buildDir + '/state.json', waitFor(function (err, ret) {
                if (err) { throw err; }
                state = ( JSON.parse(ret) /*:Builder_State_t*/ );
                hasState = true;
                debug("Loaded state file");
            }));
        }));

    }).nThen(function (waitFor) {

        debug("Initialize " + time() + "ms");

        // Do the configuration step
        if (hasState) {
            ctx = finalizeCtx(state, pctx);
            return;
        }

        state = getStatePrototype();
        ctx = finalizeCtx(state, pctx);
        probeCompiler(ctx, waitFor());

    }).nThen(function (waitFor) {
        //if (!ctx.builder) { throw new Error(); }
        configFunc(ctx.builder, waitFor);

    }).nThen(function (_) {
        ctx.sema = Saferphore.create(ctx.config.jobs);

        if (ctx.config.systemName !== systemName) {
            throw new Error("systemName cannot be changed in configure phase " +
                "it must be specified in the initial configuration " +
                `initial systemName = ${systemName}, changed to ${ctx.config.systemName}`);
        }

        if (ctx.config.gcc !== gcc) {
            throw new Error("gcc cannot be changed in configure phase " +
                "it must be specified in the initial configuration " +
                `initial gcc = ${gcc}, changed to ${ctx.config.gcc}`);
        }

        deepFreeze(ctx.config);

        debug("Configure " + time() + "ms");

        if (!ctx) { throw new Error(); }
        postConfigure(ctx, time);
    });

    const out = Object.freeze({
        build:    function (x /*:Builder_Stage_t*/) { pctx.buildStage = x;    return out; },
        test:     function (x /*:Builder_Stage_t*/) { pctx.testStage = x;     return out; },
        pack:     function (x /*:Builder_Stage_t*/) { pctx.packStage = x;     return out; },
        failure:  function (x /*:Builder_Stage_t*/) { pctx.failureStage = x;  return out; },
        success:  function (x /*:Builder_Stage_t*/) { pctx.successStage = x;  return out; },
        complete: function (x /*:Builder_Stage_t*/) { pctx.completeStage = x; return out; },
    });
    return out;
};

const checkFileMtime = (fileName, done) => {
    Fs.stat(fileName, function (err, stat) {
        if (err) {
            if (err.code === 'ENOENT') {
                done(-1);
            } else {
                throw err;
            }
        } else {
            done(stat.mtime.getTime());
        }
    });
};

const removeStaleFiles = (ctx, done) => {
    const stales = {};
    // Transient dependencies are provided by gcc -MM so there's no need to resolve them
    const dependents = {};
    nThen((w) => {
        Object.keys(ctx.state.cFiles).forEach(function (cFile) {
            const file = ctx.state.cFiles[cFile];
            for (const incl of file.includes) {
                if (!ctx.state.hFiles[incl]) {
                    // Missing the header entirely, definitely stale
                    debug(`\x1b[1;34m${cFile} stale (header ${incl} deleted)\x1b[0m`);
                    stales[cFile] = 1;
                    return;
                }
                (dependents[incl] = dependents[incl] || []).push(cFile);
            }
            const cflags = getFlags(ctx, cFile, false);
            if (JSON.stringify(cflags) !== JSON.stringify(file.cflags)) {
                debug(`\x1b[1;34m${cFile} stale (change of cflags)\x1b[0m`);
                stales[cFile] = 1;
                return;
            }
            checkFileMtime(cFile, w((mtime) => {
                if (mtime !== file.mtime) {
                    debug(`\x1b[1;34m${cFile} stale\x1b[0m`);
                    stales[cFile] = 1;
                } else {
                    Fs.access(getOFile(ctx, cFile), Fs.constants.F_OK, w((err) => {
                        if (err && err.code !== 'ENOENT') {
                            throw err;
                        } else if (err) {
                            // Not stale but needs to be compiled
                            ctx.toCompile[cFile] = file;
                        }
                    }));
                }
            }));
        });
    }).nThen((w) => {
        Object.keys(ctx.state.hFiles).forEach(function (hFile) {
            const file = ctx.state.hFiles[hFile];
            checkFileMtime(hFile, w((mtime) => {
                if (mtime === file.mtime) {
                    return;
                } else if (mtime === -1) {
                    debug(`\x1b[1;34m${hFile} stale (deleted)\x1b[0m`);
                    delete ctx.state.hFiles[hFile];
                } else {
                    debug(`\x1b[1;34m${hFile} stale\x1b[0m`);
                    file.mtime = mtime;
                }
                for (const cFile of (dependents[hFile] || [])) {
                    debug(`\x1b[1;34m${cFile} stale (includes ${hFile})\x1b[0m`);
                    stales[cFile] = 1;
                }
            }));
        });
    }).nThen((w) => {
        Object.keys(stales).forEach((cFile) => {
            const file = ctx.state.cFiles[cFile];
            if (typeof(file) === 'undefined') { return; }
            delete ctx.state.cFiles[cFile];
            // Sweep up relevant files
            [getIFile(ctx, cFile), getOFile(ctx, cFile)].forEach((deleteMe) => {
                if (!deleteMe) { return; }
                Fs.unlink(deleteMe, w(function (err) {
                    if (err && err.code !== 'ENOENT') {
                        throw err;
                    }
                }));
            });
        });
    }).nThen((w) => {
        done();
    });
};

const postConfigure = (ctx /*:Builder_Ctx_t*/, time) => {
    const state = ctx.state;
    nThen((waitFor) => {

        removeStaleFiles(ctx, waitFor());

    }).nThen(function (waitFor) {

        debug("Scan for out of date files " + time() + "ms");

        ctx.buildStage(ctx.builder, waitFor);

    }).nThen(function (waitFor) {
        ctx.executables = ctx.executables.filter((exe) => {
            if (!needsToLink(ctx, exe.cFile)) {
                debug(`\x1b[1;31m${getExeFile(ctx, exe)} up to date\x1b[0m`);
                return false;
            }
            return true;
        });
        preprocessFiles(ctx, ctx.executables.map(exe => exe.cFile), waitFor());
    }).nThen(function (w) {
        debug("Preprocess " + time() + "ms");
        // save state
        const stateJson = JSON.stringify(state, null, '\t');
        Fs.writeFile(ctx.config.buildDir + '/state.json', stateJson, w(function (err) {
            if (err) { throw err; }
            //debug("Saved state " + time() + "ms");
            deepFreeze(state);
        }));
    }).nThen(function (w) {
        Object.keys(ctx.toCompile).forEach((cFile) => {
            compile(ctx, cFile, w((err) => {
                if (err) {
                    throw err;
                }
                debug('\x1b[2;32mCompiling ' + cFile + ' complete\x1b[0m');
            }));
        });
    }).nThen(function (waitFor) {
        debug("Compile " + time() + "ms");
        for (const exe of ctx.executables) {
            if (exe.type === 'exe') {
                link(exe.cFile, waitFor(), ctx);
            }
        }
    }).nThen((w) => {
        debug("Link " + time() + "ms");

        ctx.tests.forEach(function (test) {
            test(w(function (output, failure) {
                debug(output);

                if (failure) {
                    ctx.failure = true;
                }
            }));
        });

    }).nThen(function (waitFor) {

        if (ctx.linters.length === 0) {
            return;
        }

        debug("Checking codestyle");

        const sema = Saferphore.create(64);

        Object.keys(ctx.toCompile).forEach(function (cFile) {
            sema.take(waitFor(function (returnAfter) {
                Fs.readFile(cFile, waitFor(function (err, ret) {
                    if (err) { throw err; }

                    ret = ret.toString('utf8');
                    nThen(function (waitFor) {

                        ctx.linters.forEach(function (linter) {
                            linter(cFile, ret, waitFor(function (out, isErr) {
                                if (isErr) {
                                    debug("\x1b[1;31m" + out + "\x1b[0m");
                                    ctx.failure = true;
                                }
                            }));
                        });

                    }).nThen(returnAfter(waitFor()));
                }));
            }));
        });

    }).nThen(function (waitFor) {

        ctx.testStage(ctx.builder, waitFor);

    }).nThen(function (waitFor) {

        if (ctx.failure) { return; }

        debug("Test " + time() + "ms");

        ctx.executables.forEach(function (exe) {
            const temp = getTempExe(ctx, exe.cFile);
            if (exe.outputFile === temp) { return; }
            const outputFile = getExeFile(ctx, exe);
            Fs.rename(temp, outputFile, waitFor(function (err) {
                // TODO(cjd): It would be better to know in advance whether to expect the file.
                if (err && err.code !== 'ENOENT') {
                    throw err;
                }
            }));
        });

    }).nThen(function (waitFor) {

        if (ctx.failure) { return; }
        ctx.packStage(ctx.builder, waitFor);

    }).nThen(function (waitFor) {

        if (ctx.failure) { return; }
        debug("Pack " + time() + "ms");

    }).nThen(function (waitFor) {

        if (ctx.failure) { return; }

        ctx.successStage(ctx.builder, waitFor);

    }).nThen(function (waitFor) {

        if (!ctx.failure) { return; }

        ctx.failureStage(ctx.builder, waitFor);

    }).nThen(function (waitFor) {

        ctx.completeStage(ctx.builder, waitFor);

        sweep(ctx.config.buildDir + '/tmp', waitFor());
    });
};
