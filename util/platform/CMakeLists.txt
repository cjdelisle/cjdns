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

add_subdirectory(netdev)

find_package(Libuv REQUIRED)
include_directories(${LIBUV_INCLUDE_DIRS})

if (Windows)
    # uv.h
    remove_definitions(-pedantic)
endif ()

add_library(cjdns-util-platform-socket
    Socket.c
    Sockaddr.c
)
target_link_libraries(cjdns-util-platform-socket
    ${LIBUV_LIBRARIES}
    util
    cjdbenc
)

# Force libuv to build first
add_dependencies(cjdns-util-platform-socket ${LIBUV_LIBRARIES})

enable_testing()
add_subdirectory(test)
