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

if (NOT LIBUV_FOUND)
    include(LibuvHelpers)

    find_path(LIBUV_INCLUDE_DIRS
        NAMES
	    uv.h
	PATHS
	    ${LIBUV_PREFIX}/include
	    /usr/include
	    /usr/local/include
	    /opt/local/include
	    ${CMAKE_BINARY_DIR}/libuv/include
	NO_DEFAULT_PATH)

    find_library(LIBUV_LIBRARIES
        NAMES
	    libuv.a
	PATHS
	    ${LIBUV_INCLUDE_DIRS}/../.libs
	    ${LIBUV_PREFIX}/lib
	    /usr/lib
	    /usr/local/lib
	    /opt/local/lib
	NO_DEFAULT_PATH)

    # May need to test linkage here (similarly to libevent), but I'll hold
    # off on that until it's needed.

    if (LIBUV_INCLUDE_DIRS AND LIBUV_LIBRARIES)
        set(LIBUV_FOUND TRUE)

        if("$ENV{NO_STATIC}" STREQUAL "")
            add_library(uv STATIC IMPORTED)
        else()
            add_library(uv SHARED IMPORTED)
        endif()
	set_property(TARGET uv PROPERTY IMPORTED_LOCATION ${LIBUV_LIBRARIES})
	addDependencies()
	set(LIBUV_LIBRARIES uv)
    endif()

endif()
