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

#if(COMMAND cmake_policy)
#    cmake_policy(SET CMP0003 NEW)
#endif(COMMAND cmake_policy)

add_library(cjdns-util-events-libuv
    EventBase.c
    Event.c
    Time.c
    Timeout.c
)
target_link_libraries(cjdns-util-events-libuv
    cjdmemory
    ${LIBUV_LIBRARIES}
)

set_target_properties(cjdns-util-events-libuv
        PROPERTIES COMPILE_FLAGS "-D _GNU_SOURCE")
