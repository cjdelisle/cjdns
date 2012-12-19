#!/bin/bash
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
let line=0
for x in `curl --data-binary @__TEST_FILE__ __REMOTE_TEST_IP_PORT__ 2>>/dev/null`
do
    [ "${line}" == "0" ] && echo "$x" | grep '4:codei0e' >/dev/null && echo 'Passed' && exit 0;
    echo 'Failed'
    echo "$x";
    let "line = line + 1"
done && exit 1;

