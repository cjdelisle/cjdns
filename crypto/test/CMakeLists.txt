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

#allow us to test internal CryptoAuth.c functions.
add_library(exports Exports.c)
target_link_libraries(exports crypto util)

set(Test_FILES
    CryptoAuth_test.c
    CryptoAuth_unit_test.c
    ReplayProtector_test.c
)
set(Test_LIBRARIES
    crypto
    util
    exports
    cjdns-crypto-random
    cjdns-util-events-libuv
    cjdns-memory-canary
)
include(${CMAKE_SOURCE_DIR}/cmake/modules/Test.cmake)
