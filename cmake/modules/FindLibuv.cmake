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

    include(CheckLibraryExists)
    include(CheckFunctionExists)
    include(FindThreads)
    include(FindClockGettime)

    find_package(Socket REQUIRED)
    find_package(ClockGettime)
    find_package(Threads)

    function(addDependencies)
        if (APPLE)
            find_library(CORE_SERVICES_LIB CoreServices)
            set_property(TARGET uv PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES
                ${SOCKET_LIBRARIES}
                ${CLOCK_GETTIME_LIBRARIES}
                ${CMAKE_THREAD_LIBS_INIT}
                ${CORE_SERVICES_LIB}
            )
        else ()
            set_property(TARGET uv PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES
                ${SOCKET_LIBRARIES}
                ${CLOCK_GETTIME_LIBRARIES}
                ${CMAKE_THREAD_LIBS_INIT}
            )
        endif ()
    endFunction()

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
	    uv
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
    endif()

    if (LIBUV_FOUND)
	    if ("${LIBUV_INCLUDE_DIRS}" STREQUAL "${CMAKE_BINARY_DIR}/libuv/include")
	        add_library(uv STATIC IMPORTED)
	        set_property(TARGET uv PROPERTY IMPORTED_LOCATION ${LIBUV_LIBRARIES})
	        addDependencies()
	        set(LIBUV_LIBRARIES uv)
	    endif()
    endif()

endif()

if (NOT LIBUV_FOUND AND "$ENV{NO_STATIC}" STREQUAL "")
    include(ExternalProject)

    # Without this, the build doesn't happen until link time.
    include_directories(${UV_USE_FILES})

    set(url "${CMAKE_SOURCE_DIR}/cmake/externals/node-v0.9.7.tar.gz")
    if (NOT EXISTS "${url}")
        set(url "https://github.com/joyent/libuv/archive/node-v0.9.7.tar.gz")
    endif()

    if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        separate_arguments(MAKE_COMMAND UNIX_COMMAND "CFLAGS=-fPIC make")
    else ()
        set(MAKE_COMMAND "make")
    endif ()

    ExternalProject_Add(libuv
        URL ${url}
	URL_MD5 "caa8135e6f74628fbd7cf789e9c432e5"
	SOURCE_DIR "${CMAKE_BINARY_DIR}/libuv"
	BINARY_DIR "${CMAKE_BINARY_DIR}/libuv"
	CONFIGURE_COMMAND ""
        BUILD_COMMAND ${MAKE_COMMAND}
	INSTALL_COMMAND ""
	UPDATE_COMMAND ""
	PATCH_COMMAND "")

    set(LIBUV_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/libuv/include")

    add_library(uv STATIC IMPORTED)

    set_property(TARGET uv
        PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/libuv/libuv.a)

    addDependencies()

    set(LIBUV_LIBRARIES uv)
    set(LIBUV_FOUND TRUE)

endif()
