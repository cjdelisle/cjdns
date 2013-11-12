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

if(Windows)
    set(sec Security_Windows.c)
else()
    set(sec Security.c)
endif()


add_library(cjdns-security
    ${sec}
)

target_link_libraries(cjdns-security
    cjdns-except
)

add_library(util
    AverageRoller.c
    Hex.c
    Pinger.c
    Security_admin.c
    Assert.c
    Order.c
)
target_link_libraries(util
    cjdmemory
    cjdns-security
    cjdns-util-log-writer
    cjdns-util-platform-socket
)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
