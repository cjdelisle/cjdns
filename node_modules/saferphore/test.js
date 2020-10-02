'use strict';
var Saferphore = require('./index');
var assert = function (x) { if (!x) { throw new Error(); } };

var basicTest = function () {
    var sem = Saferphore.create(4);
    var out = [];
    (new Array(10)).join().split(',').forEach(function (x,y) {
        sem.take(function (returnAfter) {
            out.push(y);
            setTimeout(returnAfter(function () {
                if (out.length === 10) {
                    if (out.join() !== "0,1,2,3,4,5,6,7,8,9") { throw new Error(); }
                }
            }));
        });
    });
    if (out.join() !== "0,1,2,3") {
        throw new Error(out.join());
    }
};

var doubleCallTest = function () {
    var sem = Saferphore.create(4);
    var catcher = setTimeout(function () {
        throw new Error();
    }, 1000);
    sem.take(function (returnAfter) {
        setTimeout(returnAfter(function () {
            setTimeout(function () {
                clearTimeout(catcher);
                try { returnAfter()(); } catch (e) { return; }
                throw new Error();
            });
        }));
    });
};

var doubleCallbackTest = function () {
    var sem = Saferphore.create(4);
    var catcher = setTimeout(function () {
        throw new Error();
    }, 1000);
    sem.take(function (returnAfter) {
        var wrapped = returnAfter();
        wrapped();
        try { wrapped(); } catch (e) { clearTimeout(catcher); return; }
        throw new Error();
    });
};

basicTest();
doubleCallTest();
doubleCallbackTest();
