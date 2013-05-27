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

include_directories(${LIBUV_INCLUDE_DIRS})

if (Windows)
    # uv.h
    remove_definitions(-pedantic)
endif ()

add_library(cjdns-util-events-libuv-time
    Time.c
    Timeout.c
)
set_target_properties(cjdns-util-events-libuv-time
        PROPERTIES COMPILE_FLAGS "-D _GNU_SOURCE")

add_library(cjdns-util-events-libuv
    EventBase.c
    Event.c
    Pipe.c
    Process.c
    UDPAddrInterface.c
)

target_link_libraries(cjdns-util-events-libuv
    cjdns-util-events-libuv-time
    cjdmemory
    ${LIBUV_LIBRARIES}
)

set_target_properties(cjdns-util-events-libuv
        PROPERTIES COMPILE_FLAGS "-D _GNU_SOURCE")
