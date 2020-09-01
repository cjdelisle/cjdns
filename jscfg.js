var preprocess = require('./jsmacro.js').preprocess,
    child_process = require('child_process'), fs = require('fs'),
    os = require('os'), subnode = !!process.env['SUBNODE'],
    testc = 'test/testcjdroute.c', signc = 'crypto/Sign.c', mains = [
        ['client/cjdroute2.c', 'cjdroute'],
        ['crypto/random/randombytes.c'],
        ['contrib/c/publictoip6.c'],
        ['contrib/c/privatetopublic.c'],
        ['contrib/c/sybilsim.c'],
        ['contrib/c/makekeys.c'],
        ['contrib/c/mkpasswd.c'],
        ['test/testcjdroute.c']
    ], tests = child_process.execSync(
        'find . -name "*_test.c" | ' + (subnode ? 'grep -v dht | ' : '') +
            'sed "s/^..//" | sort', { encoding: 'utf-8' }
    ).split(/\s+/).slice(0, -1);

var deps = {}, x_cppflags = {}, testlinks = [],
    testcfg = { tests: [], fuzzes: [], cases: [] },
    jscfg = { links: {}, uvFiles: [], systemName: process.platform },
    env = { CC: 'cc', X_CFLAGS: ['-pedantic'], X_CPPFLAGS: ['-I.'],
        CPPFLAGS: ['-std=c99', '-D_FORTIFY_SOURCE=2',
            '-I./build_linux/dependencies/libuv/include',
            '-I./build_linux/dependencies/cnacl/jsbuild/include',
            '-D' + jscfg.systemName + '=1', '-DNumberCompress_TYPE=v3x5x8',
            '-DIdentity_CHECK=1', '-DAllocator_USE_CANARIES=1', '-DPARANOIA=1',
            '-DLog_DEBUG', '-DCJD_PACKAGE_VERSION="unknown"'] };

var ext_sub = function (filename, ext) {
    return filename.replace(/\.[^.]+$/, ext);
};

var maybe_quote = function (ss) {
    var ret = '';
    ss.forEach(function (s) {
        ret += (/["'\\]|\s/.test(s) ? (
            "'" + s.replace(/'/g, "'\\''") + "'"
        ) : s) + ' ';
    });
    return ret.slice(0, -1);
};

var detect_seccomp = function () {
    if (jscfg.systemName !== 'linux') return false;
    var release = os.release().replace(/[^.0-9].*/g, '')
        .replace(/\./g, '').split('.');
    if (1000 * Number(release[0]) + Number(release[1]) < 3005) return false;
    return child_process.spawnSync(
        env.CC, ['-o', '/dev/null', 'node_build/try_seccomp.c']
    ).status === 0;
};

var detect_system = function () {
    var ret = '', yesno = function (s, x)
        { ret += s + (x ? 'yes' : 'no'); return x; };
    if (yesno('ethiface ', ['linux', 'darwin'].indexOf(jscfg.systemName) >= 0))
        env.CPPFLAGS.push('-DHAS_ETH_INTERFACE=1');
    jscfg.setuidImpl = 'util/Setuid_' + jscfg.systemName + '.c';
    if (yesno(', setuid ', fs.existsSync(jscfg.setuidImpl)))
        jscfg.setuidImpl = 'util/Setuid_dummy.c';
    jscfg.seccompImpl = yesno(', seccomp ', detect_seccomp()) ?
        'util/Seccomp.c' : 'util/Seccomp_dummy.c';
    if (yesno(', subnode ', subnode)) env.CPPFLAGS.push('-DSUBNODE=1');
    return ret;
};

var cases_add = function (filename) {
    var idx = Buffer.alloc(4);
    idx.writeInt32BE(testcfg.fuzzes.length, 0);
    child_process.execSync(
        'ls ' + ext_sub(filename, '_cases/*.hex | sort'), { encoding: 'utf-8' }
    ).split(/\s+/).slice(0, -1).forEach(function (_case) {
        var out = ext_sub(_case, '.bin');
        fs.writeFileSync(out, Buffer.concat([
            idx, Buffer.from(fs.readFileSync(_case, 'utf-8')
                .replace(/#.*\n/g, '').replace(/\s+/g, ''), 'hex')
        ]));
        testcfg.cases.push(out);
    });
};

var test_add = function (filename) {
    var base = ext_sub(filename, '').replace(/.*\//, '');
    testlinks.push(filename);
    if (/_fuzz_test/.test(filename)) {
        x_cppflags[filename] = [
            '-I.', '-DCJDNS_FUZZ_INIT=' + base + '_FUZZ_INIT',
            '-DCJDNS_FUZZ_MAIN=' + base + '_FUZZ_MAIN'
        ];
        cases_add(filename);
        testcfg.fuzzes.push(base);
    } else {
        var _main = base + '_main';
        x_cppflags[filename] = [
            '-I.', '-Dmain=' + _main, '-D' + _main + '(...)=' +
                _main + '(int argc, char **argv); int ' +
                _main + '(int argc, char **argv)'
        ];
        testcfg.tests.push(base);
    }
};

var cpp = function (filename) {
    if (!/\.c$/.test(filename)) throw new Error(filename);
    if (jscfg.links[filename]) return;
    jscfg.links[filename] = filename === testc ? testlinks : [];
    var proc = child_process.spawnSync(
        env.CC, ['-E'].concat(env.CPPFLAGS)
            .concat(x_cppflags[filename] || env.X_CPPFLAGS)
            .concat([filename]), { encoding: 'utf-8' }
    );
    if (proc.status !== 0) throw new Error(proc.stderr);
    fs.writeFileSync(ext_sub(filename, '.ii'), proc.stdout);
    preprocess(proc.stdout, filename, jscfg);
};

var dep_get = function (l) {
    var ret = [l[0]];
    if (jscfg.links[l[0]]) throw new Error(l[0]);
    for (var i = 0; i < ret.length; i++) {
        cpp(ret[i]);
        jscfg.links[ret[i]].forEach(function (req) {
            if (ret.indexOf(req) < 0) ret.push(req);
        });
    }
    deps[l[1]] = ret;
};

var fmt_tests = function () {
    var ret = '#ifndef TestCjdroute_Gen_H\n#define TestCjdroute_Gen_H\n\n';
    testcfg.tests.forEach(function (e) { ret += 'Test ' + e + '_main;\n'; });
    testcfg.fuzzes.forEach(function (e) { ret += 'Fuzz_proto(' + e + ')\n'; });
    ret += '\nstatic const struct { char *name; Test *func; } TESTS[] = {\n'
    testcfg.tests.forEach(function (e) { ret += 'Test_elem(' + e + '),\n'; });
    ret += '};\n\nstatic const struct { char *name; ' +
        'FuzzTestInit *init; FuzzTest *fuzz; } FUZZ_TESTS[] = {\n';
    testcfg.fuzzes.forEach(function (e) { ret += 'Fuzz_elem(' + e + '),\n'; });
    ret += '};\n\nstatic const char *FUZZ_CASES[] = {\n';
    testcfg.cases.forEach(function (e) { ret += '"' + e + '",\n'; });
    ret += '};\n\n#endif\n';
    return ret;
};

var fmt_mk = function () {
    var all = [];
    mains.forEach(function (e) { all.push(e[1]) });
    var ret = 'CC = ' + env.CC + '\nCPPFLAGS = ' + maybe_quote(env.CPPFLAGS) +
        '\nX_CFLAGS = ' + maybe_quote(env.X_CFLAGS) + '\nX_CPPFLAGS = ' +
        maybe_quote(env.X_CPPFLAGS) + '\nall: ' + all.join(' ') + '\n' +
        all.join(' ') + ':\n\t$(LINK.o) $^ $(LDLIBS) -o $@\n';
    all.forEach(function (k) {
        ret += k + ':';
        deps[k].forEach
            (function (filename) { ret += ' ' + ext_sub(filename, '.o'); });
        ret += '\n';
    });
    if (jscfg.uvFiles.length) {
        jscfg.uvFiles.forEach
            (function (filename) { ret += ext_sub(filename, '.o') + ' '; });
        ret = ret.slice(0, -1) + ': X_CFLAGS =\n';
    }
    Object.keys(x_cppflags).forEach(function (k) {
        ret += ext_sub(k, '.ii') + ': X_CPPFLAGS = ' +
            maybe_quote(x_cppflags[k]) + '\n';
    });
    return ret;
};

var main = function (argv) {
    console.log('Detecting system:');
    console.log('    ' + detect_system());
    console.log('Formatting tests');
    tests.forEach(test_add);
    fs.writeFileSync(ext_sub(testc, '_gen.h'), fmt_tests());
    console.log('Computing dependencies:');
    x_cppflags[signc] =
        ['-I.', '-I./build_linux/dependencies/cnacl/jsbuild/include_internal'];
    mains.forEach(function (e) {
        if (!e[1]) e[1] = ext_sub(e[0], '').replace(/.*\//, '');
        dep_get(e);
        console.log('    ' + e[0] + ' done');
    });
    console.log('Formatting configuration');
    fs.writeFileSync('jscfg.mk', fmt_mk());
};

main(process.argv);

