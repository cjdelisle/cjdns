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

add_library(cjdns-crypto-random-test
    DeterminentRandomSeed.c
)

set(Test_FILES
    Random_test.c
)
set(Test_LIBRARIES
    cjdns-crypto-random
    cjdns-crypto-random-test
    cjdio
)
include(${CMAKE_SOURCE_DIR}/cmake/modules/Test.cmake)
