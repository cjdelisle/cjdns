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

# Need librt to be included if libevent is static linked.
function(includeLibrt)
    # Apple includes librt in the standard system library.
    if(NOT DEFINED APPLE)
        find_package(Librt REQUIRED)

        set_property(TARGET event2
            PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES ${LIBRT_LIBRARIES})
    endif()
endfunction()

if (NOT LIBEVENT2_FOUND AND "$ENV{STATIC}" STREQUAL "")

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

    # Check that we can link against it first (x86/amd64 wowes)
    if(LIBEVENT2_INCLUDE_DIRS AND LIBEVENT2_LIBRARIES)
        message("Found libevent @ ${LIBEVENT2_INCLUDE_DIRS} & ${LIBEVENT2_LIBRARIES}")
        message("    Attempting to link a trivial program.")
        set(srcfile "${CMAKE_BINARY_DIR}/libevent2/linktest.c")
        file(WRITE
            ${CMAKE_BINARY_DIR}/libevent2/linktest.c
            "int main() { event_base_new(); return 0; }"
        )
        try_compile(success ${CMAKE_BINARY_DIR}/libevent2 ${srcfile}
            CMAKE_FLAGS "-DLINK_LIBRARIES:STRING=${LIBEVENT2_LIBRARIES}"
            OUTPUT_VARIABLE error
        )
        if(success)
            message("    Success!")
            set(LIBEVENT2_FOUND TRUE)
        else()
            message("    Failed to compile/link, building another copy of libevent.")
        endif()
        message("")
    endif()

    if(LIBEVENT2_FOUND)
        if("${LIBEVENT2_INCLUDE_DIRS}" STREQUAL "${CMAKE_BINARY_DIR}/libevent2-bin/include")
            add_library(event2 STATIC IMPORTED)
            set_property(TARGET event2 PROPERTY IMPORTED_LOCATION ${LIBEVENT2_LIBRARIES})
            includeLibrt()
            set(LIBEVENT2_LIBRARIES event2)
        endif()
    endif()

endif()

if (NOT LIBEVENT2_FOUND AND "$ENV{NO_STATIC}" STREQUAL "")
    include(ExternalProject)

    # Without this, the build doesn't happen until link time.
    include_directories(${LIBEVENT2_USE_FILES})

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

    if(CMAKE_VERSION VERSION_LESS 2.8.4)
        message("Parallel building (-j) will not be available.")
        message("To build in parallel, upgrade to cmake 2.8.4 or newer.")
        message("see: http://www.cmake.org/Bug/print_bug_page.php?bug_id=10395")
    else()
        add_dependencies(event2 Libevent2)
    endif()

    set_property(TARGET event2
        PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/libevent2-bin/lib/libevent.a)

    includeLibrt()
    set(LIBEVENT2_LIBRARIES event2)
    set(LIBEVENT2_FOUND TRUE)
endif()
