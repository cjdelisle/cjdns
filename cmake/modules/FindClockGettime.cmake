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

if(NOT CLOCK_GETTIME_FOUND AND NOT NO_CLOCK_GETTIME)
    check_function_exists(clock_gettime HAVE_GETTING)
    if(HAVE_GETTING)
        set(CLOCK_GETTIME_FOUND TRUE)
        set(CLOCK_GETTIME_LIBRARIES "")
    else()
        message("clock_gettime() not found, searching for library")
    endif()

    if(NOT CLOCK_GETTIME_FOUND)
        find_library(CLOCK_GETTIME_LIBRARIES
            NAMES
                rt
            PATHS
                ${CLOCK_GETTIME_PREFIX}/lib/
                /usr/local/lib64/
                /usr/local/lib/
                /usr/lib/i386-linux-gnu/
                /usr/lib/x86_64-linux-gnu/
                /usr/lib/powerpc-linux-gnu/
                /usr/lib64/
                /usr/lib/
                /lib/x86_64-linux-gnu/
        )

        if(CLOCK_GETTIME_LIBRARIES)
            message("Yay clock_gettime() was found at [${CLOCK_GETTIME_LIBRARIES}]")
            set(CLOCK_GETTIME_FOUND TRUE)
        else()
            message("Could not find clock_gettime()")
            if(ClockGettime_FIND_REQUIRED)
                message(FATAL_ERROR "Try to setup CLOCK_GETTIME_PREFIX accordingly")
            else()
                message("Skipping because ClockGettime_FIND_REQUIRED is not set")
                set(CLOCK_GETTIME_LIBRARIES "")
            endif()
        endif()
    endif()
endif()
