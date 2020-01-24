/*@flow*/
/*
 * Caleb James DeLisle
 * Sat Mar 23 01:42:29 EDT 2013
 * Public Domain or MIT License
 */
/*::
type WaitFor_t = (?(...any)=>void)=>((...any)=>void);
type NthenRet_t = { nThen: Nthen_t, orTimeout:((WaitFor_t)=>void, number)=>NthenRet_t };
type Nthen_t = ((WaitFor_t)=>void)=>NthenRet_t;
module.exports = */ (function() {
var nThen /*:Nthen_t*/ = function(next) {
    var funcs /*:Array<(WaitFor_t)=>void>*/ = [];
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
                while (funcs.shift() !== cto) { }
                func(waitFor);
                calls = (calls || 1) - 1;
                while (!calls && funcs.length) { funcs.shift()(waitFor); }
            }, milliseconds);
            funcs.push(cto = function() {
                var idx = timeouts.indexOf(timeout);
                if (idx > -1) {
                    timeouts.splice(idx, 1);
                    clearTimeout(timeout);
                    return;
                }
                throw new Error('timeout not listed in array');
            });
            timeouts.push(timeout);
            return ret;
        }
    };
    return ret.nThen(next);
};

if (typeof(module) !== 'undefined') { module.exports = nThen; }
else if (typeof(window.define) === 'function' && window.define.amd) { window.define(function () { return nThen; }); }
else { window.nThen = nThen; }
/*:: return nThen; */
})();
