var Fs = require("fs");
var Semaphore = require('../tools/lib/Semaphore');
var nThen = require('nthen');

var sema = Semaphore.create(64);
var cp = module.exports = function (src, dest, callback) {
    Fs.stat(src, function (err, stat) {
        if (err) { throw err; }
        if (stat.isDirectory()) {
            var subFiles;
            nThen(function (waitFor) {
                Fs.mkdir(dest, waitFor(function (err) {
                    if (err) { throw err; }
                }));
                Fs.readdir(src, waitFor(function (err, list) {
                    if (err) { throw err; }
                    subFiles = list;
                }));
            }).nThen(function (waitFor) {
                subFiles.forEach(function (file) {
                    cp(src + '/' + file, dest + '/' + file, waitFor());
                });
            }).nThen(function (waitFor) {
                callback();
            });
        } else {
            sema.take(function (returnAfter) {
                Fs.readFile(src, function (err, content) {
                    if (err) { throw err; }
                    Fs.writeFile(dest, content, returnAfter(function (err) {
                        if (err) { throw err; }
                        callback();
                    }));
                });
            });
        }
    });
};
