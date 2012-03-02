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
if ("$ENV{STATIC}" STREQUAL "")
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
                ${CMAKE_BINARY_DIR}/libevent2-bin/include
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
endif ("$ENV{STATIC}" STREQUAL "")

if (NOT LIBEVENT2_FOUND AND "$ENV{NO_STATIC}" STREQUAL "")
    include(ExternalProject)

    # Without this, the build doesn't happen until link time.
    include_directories(${LIBEVENT2_USE_FILES})

    # Librt functions are in the standard library on apple.
    if(NOT DEFINED APPLE)
        # Need librt to be included if libevent is static linked.
        find_package(Librt REQUIRED)
    endif()

    ExternalProject_Add(Libevent2
        URL "http://cloud.github.com/downloads/libevent/libevent/libevent-2.0.16-stable.tar.gz"
        URL_MD5 "899efcffccdb3d5111419df76e7dc8df"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/libevent2"
        BINARY_DIR "${CMAKE_BINARY_DIR}/libevent2-build"
        CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/libevent2/configure
            --prefix=${CMAKE_BINARY_DIR}/libevent2-bin
            --disable-openssl
            --disable-shared
            --with-pic
            --disable-thread-support
        BUILD_COMMAND make
        TEST_COMMAND ""
        INSTALL_COMMAND make install
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
    )

    set(LIBEVENT2_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/libevent2-bin/include/")

    add_library(event2 STATIC IMPORTED)

    if(CMAKE_SYSTEM_VERSION VERSION_LESS "2.8.4")
        message("Parallel building (-j) will not be available.")
        message("To build in parallel, upgrade to cmake 2.8.4 or newer.")
        message("see: http://www.cmake.org/Bug/print_bug_page.php?bug_id=10395")
    else()
        add_dependencies(event2 Libevent2)
    endif()

    set_property(TARGET event2
        PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/libevent2-bin/lib/libevent.a)

    if(NOT DEFINED APPLE)
        # This is needed to make sure libevent2 pulls in rt as a dependency.
        set_property(TARGET event2
            PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES ${LIBRT_LIBRARIES})
    endif()

    set(LIBEVENT2_LIBRARIES event2)

endif (NOT LIBEVENT2_FOUND AND "$ENV{NO_STATIC}" STREQUAL "")

if(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
   set(LIBEVENT2_FOUND TRUE)
endif(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
