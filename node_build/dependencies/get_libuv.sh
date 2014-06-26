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

# script meant to be run manually, for getting a deterministic copy of libuv for cjdns.
# After running:
# cd libuv
# $ find ./ -type f -exec sha256sum {} \; | sort | sha256sum
#   5cbd29365b03eb0655822f008115cf92bc1b9c19007ecff637585ba7eb91dafd  -


die() {
    echo "ERROR: $1";
    exit 100;
}

git clone http://github.com/joyent/libuv.git || die 'clone libuv'
cd libuv || die 'cd libuv'
git checkout 336a1825309744f920230ec3e427e78571772347 || die 'checkout revision of libuv'
rm -rf ./.git || die 'rm -rf ./.git'
mkdir -p build || die 'mkdir -p build'
svn co https://gyp.googlecode.com/svn/trunk build/gyp || die 'failed checkout gyp'
cd build/gyp || die 'failed cd build/gyp'
svn up 1857 || die 'failed to checkout revision 1857'
rm -rf ./.svn || die 'failed to remove .svn'
