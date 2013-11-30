var getAbiName = function(cc, onComplete)
{
  cc(['-o', '/dev/null', '-c', 'okcompilers/abiname_xcompile.c'], function(retcode, out, err) {
    if (!retcode) { throw new Error("expected error from abiname_xcompile.c!"); }
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
