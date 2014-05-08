/*
 * Caleb James DeLisle
 * Sat Mar 23 01:42:29 EDT 2013
 * Public Domain
 */
;(function() {
var nThen = function(next) {
    var funcs = [];
    var timeouts = [];
    var calls = 0;
    var abort;
    var waitFor = function(func) {
        calls++;
        return function() {
            if (func) {
                func.apply(null, arguments);
            }
            calls = (calls || 1) - 1;
            while (!calls && funcs.length && !abort) {
                funcs.shift()(waitFor);
            }
        };
    };
    waitFor.abort = function () {
        timeouts.forEach(clearTimeout);
        abort = 1;
    };
    var ret = {
        nThen: function(next) {
            if (!abort) {
                if (!calls) {
                    next(waitFor);
                } else {
                    funcs.push(next);
                }
            }
            return ret;
        },
        orTimeout: function(func, milliseconds) {
            if (abort) { return ret; }
            if (!milliseconds) { throw Error("Must specify milliseconds to orTimeout()"); }
            var cto;
            var timeout = setTimeout(function() {
                while (funcs.shift() !== cto) ;
                func(waitFor);
                calls = (calls || 1) - 1;
                while (!calls && funcs.length) { funcs.shift()(waitFor); }
            }, milliseconds);
            funcs.push(cto = function() {
                for (var i = 0; i < timeouts.length; i++) {
                    if (timeouts[i] === timeout) {
                        timeouts.splice(i, 1);
                        clearTimeout(timeout);
                        return;
                    }
                }
                throw new Error('timeout not listed in array');
            });
            timeouts.push(timeout);
            return ret;
        }
    };
    return ret.nThen(next);
};

if (typeof(define) == 'function') {
    // AMD (require.js etc)
    define([], function() { return nThen; });
} else if (typeof(window) !== 'undefined') {
    // Browser global var nThen
    window.nThen = nThen;
}
if (typeof(module) !== 'undefined' && module.exports) {
    // Node.js
    module.exports = nThen;
}

})();
