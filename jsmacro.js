var execJs = function (js, file, jscfg) {
    var qs = js.replace(/\n#.*\n/g, '').split('Js_Q');
    if (qs.length % 2 === 0) throw new Error(js);
    for (let i = 1; i < qs.length; i += 2) {
        qs[i] = qs[i].replace(/\'/g, '\\u0027');
    }
    js = qs.join("'");

    var func = jscfg ? (new Function('fileName', 'jscfg', js)) :
        (new Function('file', 'request', js));
    return jscfg ? func.call(func, file, jscfg) :
        func.call(func, file, function (str, path) {
            if (!file[str]) file[str] = require(path);
        });
};

var preprocess = function (content, filename, jscfg) {
    content = content.replace(/__Js_Shorten__ "[^"]*?([^"\/]+)"/g, '"$1"');
    var elems = [], array = content.split(/(<\?js|\?>|<\$js|\$>)/);
    var delims = [['<?js', '?>'], ['<$js', '$>']];
    if (jscfg) delims = [delims[1], delims[0]];
    for (var i = 0; i < array.length; i++) {
        if (array[i] === delims[0][0]) {
            i++; elems.push([array[i++]]);
            if (array[i] !== (delims[0][1])) throw new Error(filename);
        } else if (array[i] === delims[1][0]) {
            i += 2;
            if (array[i] !== (delims[1][1])) throw new Error(filename);
        } else elems.push(array[i]);
    }

    var file = jscfg ? filename : { name: filename };
    elems.forEach(function (elem, i) {
        if (typeof(elem) === 'string') { return; }
        try { elems[i] = execJs(elem[0], file, jscfg); }
        catch (e) {
            console.log('Error in file ' + filename + ':\n' + elem[0]);
            throw e;
        }
    });
    return elems.join('');
};

if (require.main === module) {
    var fs = require("fs"), filename = process.argv[2],
        js = fs.readFileSync(filename, 'utf-8');
    fs.writeFileSync(process.argv[3], preprocess(js, filename));
} else module.exports.preprocess = preprocess;

