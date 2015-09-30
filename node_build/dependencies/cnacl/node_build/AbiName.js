var getAbiName = function(cc, config, onComplete) {

    var args = [];
    args.push(config.flag.compileOnly, config.flag.outputObj + config.devNull, 'okcompilers/abiname_xcompile.c');

    cc(args, function(retcode, out, err) {
        if (!retcode) {
            throw new Error("expected error from abiname_xcompile.c!");
        }
        try {
            err = err.split('ABI_IS_')[1];
            err = err.split('__')[0];
        } catch (e) {
            throw new Error("no matching ABI in [" + err + "]");
        }
        onComplete(err);
    });
};

module.exports = {
    get: getAbiName
};
