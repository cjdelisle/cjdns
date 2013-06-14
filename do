#!/bin/sh
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

[ `dirname $0` ] && cd `dirname $0`

# get a sha256sum implementation.
getsha256sum() {
    expected=4ee73c05d5158b0fdfec9f5e52cab3fa85b98d6992a221bbff28fdbd935e8afc
    testFile=test/$expected
    for hasher in sha256sum gsha256sum 'shasum -a 256' 'openssl sha256'
    do
        #echo "trying ${hasher} ${testFile}"
        ${hasher} ${testFile} 2>/dev/null | grep -q ${expected} && SHA256SUM=${hasher} && break
    done
    [ ! "${SHA256SUM}" ] && echo "couldn't find working sha256 hasher." && exit 1
}

getsha256sum

if [ ! -d build ]; then
    mkdir build;
fi
cd build

CMAKE=`which cmake`
if [ -f cmake-build/bin/cmake ]; then
    CMAKE=`pwd`/cmake-build/bin/cmake
fi

[ ${CMAKE} ] && ${CMAKE} --version | grep '2.8.\([2-9]\|10\)' ||
while true; do
    [ -d cmake-build ] && rm -r cmake-build
    mkdir cmake-build
    cd cmake-build

    APP=`which wget || which curl || echo 'none'`
    [ "$APP" = 'none' ] && echo 'Need wget curl' && exit 1;
    [ "$APP" = `which wget` ] && $APP ${CMAKE_DOWNLOAD}
    [ "$APP" = `which curl` ] && $APP ${CMAKE_DOWNLOAD} > cmake.tar.gz

    ${SHA256SUM} ./*.tar.gz | grep ${CMAKE_SHA256} || exit 1
    tar -xzf *.tar.gz
    find ./ -mindepth 1 -maxdepth 1 -type d -exec mv {} build \;
    ./build/configure && make || exit 1
    CMAKE=`pwd`/bin/cmake
    cd ..
    break
done

(
    ${CMAKE} .. && make || exit 1;
    make test || [ "${FORCE}" != "" ] || exit 1;
    [ -f admin/angel/cjdroute2 ] && [ -f admin/angel/cjdns ] || exit 1;
    [ ! -f ../cjdroute ] || rm ../cjdroute || exit 1;
    [ ! -f ../cjdns ] || rm ../cjdns || exit 1;
    cp admin/angel/cjdroute2 ../cjdroute || exit 1;
    cp admin/angel/cjdns ../ || exit 1;
    echo -e "\033[1;32mBuild completed successfully, type ./cjdroute to begin setup.\033[0m"
) || echo -e "\033[1;31mFailed to build cjdns.\033[0m"
