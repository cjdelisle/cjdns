#!/usr/bin/env bash
# You may redistribute this program and/or modify it under the terms of
# the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CMAKE_DOWNLOAD=http://www.cmake.org/files/v2.8/cmake-2.8.8.tar.gz
CMAKE_SHA256=2b59897864d6220ff20aa8eac64cac8994e004898a1c0f899c8cb4d7b7570b46

# get a sha256sum implementation.
getsha256sum() {
    local testhash=4ee73c05d5158b0fdfec9f5e52cab3fa85b98d6992a221bbff28fdbd935e8afc
    local testfile=test/$testhash
    local program
    for program in 'sha256sum' 'gsha256sum' 'shasum -a 256' 'openssl sha256'; do
        if $program $testfile 2>/dev/null | grep -qs "$testhash"; then
            SHA256SUM=$program
            return 0
        fi
    done
    echo "Couldn't find a working sha256 tool."
    return 1
}

cd "$(dirname "$0")"
mkdir -p build
cd build

CMAKE=$PWD/cmake-build/bin/cmake

if [ ! -x "$CMAKE" ]; then
    CMAKE=$(which cmake)
fi

if ! { [ -x "$CMAKE" ] && "$CMAKE" --version |
        egrep 'version 2\.8\.([2-9]|[1-9][0-9]+)(\.|$)'; }; then

    set -e # exit if a command fails
    rm -rf cmake-build
    mkdir cmake-build
    cd cmake-build

    getsha256sum

    if which wget >&/dev/null; then
        wget "$CMAKE_DOWNLOAD" -O cmake.tar.gz
    elif which curl >&/dev/null; then
        curl "$CMAKE_DOWNLOAD" -o cmake.tar.gz
    else
        echo "Please download the following file to 'cmake.tar.gz'"
        echo "  $CMAKE_DOWNLOAD"
        exit 1
    fi

    if ! $SHA256SUM cmake.tar.gz | grep -qs "$CMAKE_SHA256"; then
        echo "Checksum of cmake.tar.gz incorrect"
        exit 1
    fi

    tar -xf cmake.tar.gz
    find . -mindepth 1 -maxdepth 1 -type d -exec mv {} build \;
    build/configure
    make
    CMAKE="$PWD/bin/cmake"

    cd ..
    set +e
fi

set -e

$CMAKE ..

make

# I'm not drunk enough to understand this.
make test || rm cjdroute &&
[ -f admin/angel/cjdroute2 ] && [ ! -f ../cjdroute ] || rm ../cjdroute && cp admin/angel/cjdroute2 ../cjdroute &&
[ -f admin/angel/cjdns ] && [ ! -f ../cjdns ] || rm ../cjdns && cp admin/angel/cjdns ../ &&
echo -e "\033[1;32mBuild completed successfully, type ./cjdroute to begin setup.\033[0m"
