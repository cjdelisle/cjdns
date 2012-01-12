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
if (LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
    set(LIBEVENT2_FOUND TRUE)
else (LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
    find_path(LIBEVENT2_INCLUDE_DIRS
        NAMES
            event2/dns.h
        PATHS
            ${LIBEVENT2_PREFIX}/include
            /usr/include
            /usr/local/include
            /opt/local/include
        NO_DEFAULT_PATH
    )

    find_library(LIBEVENT2_LIBRARIES
        NAMES
            event
        PATHS
            ${LIBEVENT2_INCLUDE_DIRS}/../lib
            ${LIBEVENT2_PREFIX}/lib
            /usr/lib
            /usr/local/lib
            /opt/local/lib
        NO_DEFAULT_PATH
    )
    if(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
       set(LIBEVENT2_FOUND TRUE)
    endif(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
endif (LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)

if (LIBEVENT2_FOUND)
    message(STATUS "Found libevent2: includes ${LIBEVENT2_INCLUDE_DIRS} libs ${LIBEVENT2_LIBRARIES}")
else (LIBEVENT2_FOUND)
    if (Libevent2_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find libevent2, try to setup LIBEVENT2_PREFIX accordingly")
    endif (Libevent2_FIND_REQUIRED)
endif (LIBEVENT2_FOUND)
