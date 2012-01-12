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
find_package(NACL REQUIRED)

include_directories(${NACL_INCLUDE_DIRS})

# Libevent is used to get random numbers.
include_directories(${LIBEVENT2_INCLUDE_DIRS})

add_library(crypto Crypto.c CryptoAuth.c HashFunctions.c AddressCalc.c sha1/sha1.c)
target_link_libraries(crypto
    benc
    MemAllocator
    ${NACL_LIBRARIES}
    ${LIBEVENT2_LIBRARIES}
    util
)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
