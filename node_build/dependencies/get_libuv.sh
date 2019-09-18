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
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# script meant to be run manually, for getting a deterministic copy of libuv for cjdns.
EXPECT_SUM=b5d78ff51506a633847b2546802bf9b5945a2a19cc318e43f57fa5096beb57b8

die() {
    echo "ERROR: $1";
    exit 100;
}

git clone https://github.com/libuv/libuv || die 'clone libuv'
cd libuv || die 'cd libuv'
git checkout 1551969c84c2f546a429dac169c7fdac3e38115e || die 'checkout revision of libuv'
rm -rf ./.git || die 'remove libuv/.git'
mkdir -p build || die 'mkdir -p build'
git clone https://chromium.googlesource.com/external/gyp build/gyp || die 'failed checkout gyp'
cd build/gyp || die 'failed cd build/gyp'
git checkout fcd686f1880fa52a1ee78d3e98af1b88cb334528 || die 'checkout revision of gyp'
rm -rf ./.git || die 'remove gyp/.git'
cd ../..
find ./ -type f -exec sha256sum {} \; | sort | sha256sum | grep $EXPECT_SUM || die 'did not get the expected file shasum'
echo "Looks good!"
