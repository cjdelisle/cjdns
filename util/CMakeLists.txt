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
add_subdirectory(log)
add_subdirectory(version)
add_subdirectory(events)
add_subdirectory(platform)

find_package(Socket REQUIRED)

add_library(cjdns-errno Errno.c)

if(WIN32)
    set(sec Security_W32.c)
    target_link_libraries(cjdns-errno ${SOCKET_LIBRARIES})
else()
    set(sec Security.c)
endif()


add_library(cjdns-security
    ${sec}
)

target_link_libraries(cjdns-security
    cjdns-except
    cjdns-errno
)

add_library(cjdns-process
    Process_${SYSTEM}.c
)

add_library(util
    AverageRoller.c
    Hex.c
    Pinger.c
    Pipe.c
    Security_admin.c
)
target_link_libraries(util
    cjdmemory
    cjdns-security
    cjdns-util-log-writer
    cjdns-errno
)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
