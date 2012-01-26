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
project(cjdns C)
cmake_minimum_required(VERSION 2.4)

if(CMAKE_BINARY_DIR STREQUAL ${CMAKE_SOURCE_DIR})
        message( FATAL_ERROR "type: mkdir build ; cd build ; cmake .. ; make" )
endif(CMAKE_BINARY_DIR STREQUAL ${CMAKE_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/modules)

# validation
add_definitions(-Wall -std=c99 -Werror -pedantic)

# taken from -Wextra
add_definitions(
    -Wclobbered
    -Wempty-body
    -Wignored-qualifiers
    -Wmissing-field-initializers
    -Wmissing-parameter-type
    -Wold-style-declaration
    -Woverride-init
    -Wsign-compare
    -Wtype-limits
    -Wuninitialized
#    -Wunused-parameter messes up logging
)

# hardening
add_definitions(
    -pie
    -fPIE

    # Broken GCC patch makes -fstack-protector-all not work
    # workaround is to give -fno-stack-protector first.
    # see: https://bugs.launchpad.net/ubuntu/+source/gcc-4.5/+bug/691722
    -fno-stack-protector
    -fstack-protector-all

    -Wstack-protector
    -D_FORTIFY_SOURCE=2
    -Wa,--noexecstack
    -Wl,-z,relro
    -Wl,-z,now
    -Wl,-z,noexecstack
)

# debugging
add_definitions(-g)

if(NOT $ENV{Log_LEVEL} STREQUAL "")
    message("Log_LEVEL = $ENV{Log_LEVEL}")
    if ($ENV{Log_LEVEL} STREQUAL "KEYS")
        message("\n\nEXPECT TO SEE PRIVATE KEYS PRINTED IN YOUR LOGS!\n\n")
    endif ($ENV{Log_LEVEL} STREQUAL "KEYS")
    add_definitions("-D Log_$ENV{Log_LEVEL}")
endif(NOT $ENV{Log_LEVEL} STREQUAL "")

add_definitions(-O3 -funroll-loops)

include_directories(${CMAKE_SOURCE_DIR})

find_package(Libevent2 REQUIRED)
include_directories(${LIBEVENT2_INCLUDE_DIRS})

find_package(NACL REQUIRED)
include_directories(${NACL_INCLUDE_DIRS})

add_subdirectory(admin)
add_subdirectory(benc)
add_subdirectory(crypto)
add_subdirectory(dht)
add_subdirectory(interface)
add_subdirectory(io)
add_subdirectory(memory)
add_subdirectory(util)
add_subdirectory(switch)


add_executable(cjdroute cjdroute.c)
target_link_libraries(cjdroute
    crypto
    interface
    switch
    dht
    dhtcore
    cjdbenc
    cjdbenc_JsonBencSerializer
    cjdmemory
    cjdadmin
    ${LIBEVENT2_LIBRARIES}
)

# Everything must be tested.
enable_testing()
