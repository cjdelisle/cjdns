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

if (NOT LIBEVENT2_FOUND)
    include(ExternalProject)

    # Without this, the build doesn't happen until link time.
    include_directories(${NACL_USE_FILES})

    ExternalProject_Add(Libevent2
        GIT_REPOSITORY git://github.com/libevent/libevent.git
        GIT_TAG 5de3fa3208c6d6221f188b623fb4b78992fda89a
        SOURCE_DIR "${CMAKE_BINARY_DIR}/libevent2"
        BINARY_DIR "${CMAKE_BINARY_DIR}/libevent2-build"
        CONFIGURE_COMMAND "${CMAKE_BINARY_DIR}/libevent2/configure
            --prefix=/home/user/src/libevent-bin
            --disable-openssl
            --disable-shared
            --with-pic
            --disable-thread-support
        "
        COMPILE_COMMAND "make"
        INSTALL_COMMAND "make install"
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
    )

    set(LIBEVENT2_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/libevent2-bin/include/")
    set(LIBEVENT2_LIBRARIES    "${CMAKE_BINARY_DIR}/libevent2-bin/lib/libevent.a")

endif (LIBEVENT2_FOUND)
