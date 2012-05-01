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
find_package(Libevent2 REQUIRED)
include_directories(${LIBEVENT2_INCLUDE_DIRS})

add_library(crypto Crypto.c CryptoAuth.c AddressCalc.c)

# This is needed because NACL must be built first because header files are generated.
add_dependencies(crypto NACL)

target_link_libraries(crypto
    cjdbenc
    cjdmemory
    ${NACL_LIBRARIES}
    ${LIBEVENT2_LIBRARIES}
    util
)

add_library(crypto_benchmark CryptoAuth_benchmark.c)
target_link_libraries(crypto_benchmark crypto)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
