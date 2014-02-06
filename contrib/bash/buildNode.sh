#!/bin/bash
cd "$BASEDIR"
echo "$BASEDIR"
cd ${BUILDDIR}
[ -d "nodejs" ] || mkdir nodejs
cd nodejs
CORES=$(grep -c ^processor /proc/cpuinfo 2>/dev/null || sysctl -n hw.ncpu)
echo "Attempting to build node.js from source..."
echo "Downloading source package..."
if wget --version > /dev/null 2>&1; then
    wget -O - http://nodejs.org/dist/v0.10.25/node-v0.10.25.tar.gz > node.tar.gz
elif curl --version > /dev/null 2>&1; then
    curl http://nodejs.org/dist/v0.10.25/node-v0.10.25.tar.gz > node.tar.gz
else
    echo 'wget or curl is required download node.js but you have neither!'
    return 1
fi
tar -xzf node.tar.gz
cd node-v0.10.25
./configure --prefix="../node" && make -j $CORES && make install
cd ..
cd ..
cd ..