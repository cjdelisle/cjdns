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

# This is for finding berkley socket functions
# it specifically looks for socket()

include(CheckFunctionExists)

if(NOT SOCKET_FOUND AND NOT NO_SOCKET)
    check_function_exists(socket HAVE_SOCKET)
    if(HAVE_SOCKET)
        set(SOCKET_FOUND TRUE)
        set(SOCKET_LIBRARIES "")
    else()
        message("socket() not found, searching for library")
    endif()

    # mingw32 uses ws2_32
    # illumos uses socket
    if(NOT SOCKET_FOUND)
        find_library(SOCKET_LIBRARIES
            NAMES
                ws2_32
                socket
            PATHS
                ${SOCKET_PREFIX}/
                /lib/
        )

        if(SOCKET_LIBRARIES)
            message("Yay socket() was found at [${SOCKET_LIBRARIES}]")
            set(SOCKET_FOUND TRUE)
        elseif(Socket_FIND_REQUIRED)
            message(FATAL_ERROR "Could not find socket(), try to setup SOCKET_PREFIX accordingly")
        else()
            message("Could not find socket(), skipping because Socket_FIND_REQUIRED is not set")
        endif()
    endif()
endif()
