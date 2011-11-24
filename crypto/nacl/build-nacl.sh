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
find ./ -name '*.h' -exec mv {} ./include/ \;
mkdir lib
find ./ -name '*.a' -exec mv {} ./lib/ \;
