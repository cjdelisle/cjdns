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
        elseif (FREEBSD)
            set_property(TARGET uv PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES
                ${SOCKET_LIBRARIES}
                ${CLOCK_GETTIME_LIBRARIES}
                ${CMAKE_THREAD_LIBS_INIT}
                kvm
            )
        elseif (WIN32)
            set_property(TARGET uv PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES
                ${SOCKET_LIBRARIES}
                ${CLOCK_GETTIME_LIBRARIES}
                ${CMAKE_THREAD_LIBS_INIT}
                psapi # GetProcessMemoryInfo()
                iphlpapi # GetAdapterAddresses()
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

    set(url "${CMAKE_SOURCE_DIR}/cmake/externals/libuv-v0.10.7.tar.gz")

    set(PATCH_LIBUV ${CMAKE_SOURCE_DIR}/cmake/patches/patch.sh ${CMAKE_SOURCE_DIR}/cmake/patches/)
    if (APPLE)
        set(MAKE_COMMAND "make")
    elseif (WIN32)
        set(MAKE_COMMAND make -f Makefile PLATFORM=mingw PREFIX=${TOOLCHAIN_PREFIX}-)
    else ()
        if (FREEBSD OR OPENBSD)
            set(make gmake)
						separate_arguments(MAKE_COMMAND UNIX_COMMAND "CFLAGS=-fPIC ${make} HAVE_DTRACE=0")
        else()
            set(make make)
						separate_arguments(MAKE_COMMAND UNIX_COMMAND "CFLAGS=-fPIC ${make}")
        endif ()
    endif ()

    ExternalProject_Add(libuv_ep
        URL ${url}
        SOURCE_DIR "${CMAKE_BINARY_DIR}/libuv"
        BINARY_DIR "${CMAKE_BINARY_DIR}/libuv"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ${MAKE_COMMAND}
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        PATCH_COMMAND ${PATCH_LIBUV}
    )

    set(LIBUV_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/libuv/include")

    add_library(uv STATIC IMPORTED)

    set_property(TARGET uv
        PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/libuv/libuv.a)

    # The source of this uglyness is a limit on adding dependencies to imported libraries.
    # see: http://www.cmake.org/Bug/print_bug_page.php?bug_id=10395
    # It's fixed in cmake 2.8.4 but it would be nice to continue supporting 2.8.2 and 2.8.3
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/DoNothing_libuv.c)
        file(WRITE ${CMAKE_BINARY_DIR}/DoNothing_libuv.c "int DoNothing() { return 0; }\n")
    endif()
    add_library(libuv_dep ${CMAKE_BINARY_DIR}/DoNothing_libuv.c)
    add_dependencies(libuv_dep libuv_ep)

    addDependencies()

    set(LIBUV_LIBRARIES uv libuv_dep)
    set(LIBUV_FOUND TRUE)

endif()
