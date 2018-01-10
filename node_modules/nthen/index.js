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
    var calls = 0;
    var waitFor = function(func) {
        calls++;
        return function() {
            if (func) {
                func.apply(null, arguments);
            }
            calls = (calls || 1) - 1;
            while (!calls && funcs.length) {
                funcs.shift()(waitFor);
            }
        };
    };
    var ret = {
        nThen: function(next) {
            if (!calls) {
                next(waitFor);
            } else {
                funcs.push(next);
            }
            return ret;
        },
        orTimeout: function(func, milliseconds) {
            if (!milliseconds) { throw Error("Must specify milliseconds to orTimeout()"); }
            var cto;
            var timeout = setTimeout(function() {
                while (funcs.shift() !== cto) { }
                func(waitFor);
                calls = (calls || 1) - 1;
                while (!calls && funcs.length) { funcs.shift()(waitFor); }
            }, milliseconds);
            funcs.push(cto = function() { clearTimeout(timeout); });
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
