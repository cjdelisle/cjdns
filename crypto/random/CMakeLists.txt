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
add_library(cjdns-crypto-random
    Random.c
)

# This is needed because NACL must be built first because header files are generated.
add_dependencies(cjdns-crypto-random NACL)

target_link_libraries(cjdns-crypto-random
    ${NACL_LIBRARIES}
    cjdns-crypto-random-seed
)

add_executable(randombytes
    randombytes.c
)
target_link_libraries(randombytes
    cjdns-crypto-random
)

add_subdirectory(seed)
add_subdirectory(libuv)

enable_testing()
add_subdirectory(test)
