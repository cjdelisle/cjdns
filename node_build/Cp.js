var Fs = require("fs");
var Semaphore = require('./Semaphore');
var nThen = require('nthen');

var throwIfErr = function(err) {
    if (err) {
        throw err;
    }
};

var sema = Semaphore.create(64);
var cp = module.exports = function(src, dest, callback) {
    Fs.stat(src, function(err, stat) {
        throwIfErr(err);
        if (stat.isDirectory()) {
            var subFiles;
            nThen(function(waitFor) {
                Fs.mkdir(dest, waitFor(function(err) {
                    throwIfErr(err);
                }));
                Fs.readdir(src, waitFor(function(err, list) {
                    throwIfErr(err);
                    subFiles = list;
                }));
            }).nThen(function(waitFor) {
                subFiles.forEach(function(file) {
                    cp(src + '/' + file, dest + '/' + file, waitFor());
                });
            }).nThen(function(waitFor) {
                callback();
            });
        } else {
            sema.take(function(returnAfter) {
                Fs.readFile(src, function(err, content) {
                    throwIfErr(err);
                    Fs.writeFile(dest, content, returnAfter(function(err) {
                        throwIfErr(err);
                        callback();
                    }));
                });
            });
        }
    });
};
