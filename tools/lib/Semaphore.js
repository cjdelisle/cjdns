module.exports.create = function (resourceCount) {
    var queue = [];
    var returnAfter = function (func) {
        var called = 0;
        return function () {
            if (called++) { throw new Error("Function called multiple times"); }
            if (func) { func.apply(null, arguments); }
            resourceCount++;
            check();
        };
    };
    var check = function () {
        if (resourceCount < 0) { throw new Error("(resourceCount < 0) should never happen"); }
        if (resourceCount === 0 || queue.length === 0) { return; }
        resourceCount--;
        queue.shift()(returnAfter);
    };
    return {
        take: function (func) {
            queue.push(func);
            check();
        }
    };
};
