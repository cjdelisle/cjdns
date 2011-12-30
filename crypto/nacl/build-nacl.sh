#!/bin/bash
cd nacl-*
cat ./do | sed \
  -e '/^exec >"$top\/log"$/d' \
 > do.loudly
sh ./do.loudly
rm -f ./do.loudly

mv ./build ../../../build/nacl-build
cd ../../../build/nacl-build

mkdir include
mkdir lib

ARCH=`uname -m`
HOST=`uname -n`
SDIR=''

case $ARCH in
    x86_64) [[ -d "$HOST/include/amd64" ]] && SDIR="amd64"
	;;
    i686) [[ -d "$HOST/include/x86" ]] && SDIR="x86"
	;;
esac

mv $HOST/include/*.h ./include/
find ./$HOST/include/$SDIR -name '*.h' -exec mv {} ./include/ \;
find ./$HOST/lib/$SDIR -name '*.a' -exec mv {} ./lib/ \;
