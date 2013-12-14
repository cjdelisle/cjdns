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

CMAKE_DOWNLOAD=http://www.cmake.org/files/v2.8/cmake-2.8.11.2.tar.gz
CMAKE_SHA256=b32acb483afdd14339941c6e4ec25f633d916a7a472653a0b00838771a6c0562

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

NODE=`which node`
if [ -f buildjs/dependencies/node/bin/node ]; then
    CMAKE=`pwd`/buildjs/dependencies/node/bin/node
fi

$NODE ./node_build/make.js $@
