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

INCLUDE(CheckLibraryExists)
INCLUDE(CheckFunctionExists)

find_package(Socket REQUIRED)
if(NOT WIN32)
    # Libevent doesn't use gettime if it's in windows mode
    find_package(ClockGettime REQUIRED)
endif()

function(addDependencies)
    set_property(TARGET event2
        PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES ${SOCKET_LIBRARIES} ${CLOCK_GETTIME_LIBRARIES})
endFunction()

if (NOT LIBEVENT2_FOUND AND "$ENV{STATIC}" STREQUAL "")

    find_path(LIBEVENT2_INCLUDE_DIRS
        NAMES
            event2/dns.h
        PATHS
            ${LIBEVENT2_PREFIX}/include
            /usr/include
            /usr/local/include
            /opt/local/include
            ${CMAKE_BINARY_DIR}/libevent2/include
        NO_DEFAULT_PATH
    )

    find_library(LIBEVENT2_LIBRARIES
        NAMES
            event
        PATHS
            ${LIBEVENT2_INCLUDE_DIRS}/../.libs
            ${LIBEVENT2_PREFIX}/lib
            /usr/lib
            /usr/local/lib
            /opt/local/lib
        NO_DEFAULT_PATH
    )

    # Check that we can link against it first (x86/amd64 wowes)
    if(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
        get_filename_component(libdir "${LIBEVENT2_LIBRARIES}" PATH)
        message("Found libevent @ ${LIBEVENT2_INCLUDE_DIRS} & ${LIBEVENT2_LIBRARIES} in ${libdir}")
        message("    Attempting to link a trivial program.")
        unset(HAVE_LIBEVENT2_LIBRARY CACHE)
        unset(HAVE_LIBEVENT2_LIBRARY)
        CHECK_LIBRARY_EXISTS(event event_new "${libdir}" HAVE_LIBEVENT2_LIBRARY)
        if(HAVE_LIBEVENT2_LIBRARY)
            message("    Success!")
            set(LIBEVENT2_FOUND TRUE)
        else()
            message("    Failed to compile/link, building another copy of libevent.")
        endif()
        message("")
    endif()

    if(LIBEVENT2_FOUND)
        if("${LIBEVENT2_INCLUDE_DIRS}" STREQUAL "${CMAKE_BINARY_DIR}/libevent2/include")
            add_library(event2 STATIC IMPORTED)
            set_property(TARGET event2 PROPERTY IMPORTED_LOCATION ${LIBEVENT2_LIBRARIES})
            addDependencies()
            set(LIBEVENT2_LIBRARIES event2)
        endif()
    endif()

endif()

if (NOT LIBEVENT2_FOUND AND "$ENV{NO_STATIC}" STREQUAL "")
    include(ExternalProject)

    # Without this, the build doesn't happen until link time.
    include_directories(${LIBEVENT2_USE_FILES})

    list(APPEND EVENT2_CONFIG --prefix=${CMAKE_BINARY_DIR}/libevent2)
    list(APPEND EVENT2_CONFIG --disable-openssl --disable-shared --with-pic)
    if(TOOLCHAIN)
        list(APPEND EVENT2_CONFIG --host=${TOOLCHAIN})
    endif()

    # https://sourceforge.net/tracker/?func=detail&aid=3527257&group_id=50884&atid=461322
    if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        list(APPEND EVENT2_CONFIG --disable-thread-support)
    endif()

    set(url "${CMAKE_SOURCE_DIR}/cmake/externals/libevent-2.0.19-stable.tar.gz")
    if(NOT EXISTS "${url}")
        set(url "http://cloud.github.com/downloads/libevent/libevent/libevent-2.0.19-stable.tar.gz")
    endif()

    set(file ${CMAKE_BINARY_DIR}/Libevent2-prefix/src/libevent-2.0.19-stable.tar.gz)

    ExternalProject_Add(Libevent2
        URL ${url}
        URL_MD5 "91111579769f46055b0a438f5cc59572"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/libevent2"
        BINARY_DIR "${CMAKE_BINARY_DIR}/libevent2"
        CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/libevent2/configure "${EVENT2_CONFIG}"
        BUILD_COMMAND make
        TEST_COMMAND ""
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
    )

    set(LIBEVENT2_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/libevent2/include/")

    add_library(event2 STATIC IMPORTED)

    if(CMAKE_VERSION VERSION_LESS 2.8.4)
        message("Parallel building (-j) will not be available.")
        message("To build in parallel, upgrade to cmake 2.8.4 or newer.")
        message("see: http://www.cmake.org/Bug/print_bug_page.php?bug_id=10395")
    else()
        add_dependencies(event2 Libevent2)
    endif()

    set_property(TARGET event2
        PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/libevent2/.libs/libevent.a)

    addDependencies()

    set(LIBEVENT2_LIBRARIES event2)
    set(LIBEVENT2_FOUND TRUE)
endif()
