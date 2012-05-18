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

message(${CMAKE_VERSION})

if(CMAKE_BINARY_DIR STREQUAL ${CMAKE_SOURCE_DIR} AND NOT OPENWRT)
    message( FATAL_ERROR "type: git clean -df && mkdir build && cd build && cmake .. && make" )
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/modules)

# validation
add_definitions(-Wall -Wextra -std=c99 -Werror -pedantic)

# This breaks logging since loggers are passed to functions
# and only used if the log level is high.
add_definitions(-Wno-unused-parameter)

include(CheckCCompilerFlag)

# There are a number of places in Admin where read() and write() are used
# and they fail silently if anything goes wrong so the result is unused.
check_c_compiler_flag(-Wno-unused-result HAS_NO_UNUSED_RESULT)
if(HAS_NO_UNUSED_RESULT)
    add_definitions(-Wno-unused-result)
endif()

# GCC only sends a warning and not an error if it can't compile with stack canaries.
set(CMAKE_REQUIRED_FLAGS "-Werror")
check_c_compiler_flag(-fstack-protector-all HAS_F_STACK_PROTECTOR)
if(NOT HAS_F_STACK_PROTECTOR)
    message("WARNING: Stack Smashing Protector is not available on this target.")
    message("SSP is an important security measure to minimize the risk of a vulnerability.")
    message("")
else()
    add_definitions(
        # Broken GCC patch makes -fstack-protector-all not work
        # workaround is to give -fno-stack-protector first.
        # see: https://bugs.launchpad.net/ubuntu/+source/gcc-4.5/+bug/691722
        -fno-stack-protector
        -fstack-protector-all
        -Wstack-protector
    )
endif()

check_c_compiler_flag(-fcatch-undefined-behavior HAS_CATCH_UNDEFINED)
if(HAS_CATCH_UNDEFINED)
    add_definitions(-fcatch-undefined-behavior)
endif()

# __builtin_constant_p()
include(CheckCSourceCompiles)
check_c_source_compiles("int main() { return __builtin_constant_p(0);}" HAS_BUILTIN_CONSTANT_P)
if (HAS_BUILTIN_CONSTANT_P)
    add_definitions("-D HAS_BUILTIN_CONSTANT_P")
endif()

# allow position independent executable to be turned off with NO_PIE cmake ..
if(NOT "$ENV{NO_PIE}" STREQUAL "")
   set(NO_PIE TRUE)
endif()
if(NOT NO_PIE)
    message("Building a position independent executable (ASLR security enabled).")
    message("NOTE: PIE executables cannot run in a debugger.")
    message("")
    add_definitions(-fPIE)
    set(PIE "-pie")
else()
    message("Building a position dependent executable (ASLR security disabled).")
    set(PIE "")

    # debugging
    add_definitions(-g)
endif()

if(NOT DEFINED APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${PIE} -Wl,-z,relro,-z,now,-z,noexecstack")
else()
    # apple seems to set noexecstack by defualt and not support relro.
    set(CMAKE_EXE_LINKER_FLAGS "${PIE}")
endif()

# logging
if(NOT $ENV{Log_LEVEL} STREQUAL "")
    string(TOUPPER $ENV{Log_LEVEL} LEVEL)
    message("Log_LEVEL = ${LEVEL}")
    if ($ENV{Log_LEVEL} STREQUAL "KEYS")
        message("\n\nEXPECT TO SEE PRIVATE KEYS PRINTED IN YOUR LOGS!\n\n")
    endif ($ENV{Log_LEVEL} STREQUAL "KEYS")
    add_definitions("-D Log_${LEVEL}")
else()
    add_definitions("-D Log_DEBUG")
endif()

# vrooooooom
add_definitions(-O2 -funroll-loops)


if(NOT "$ENV{LTO}" STREQUAL "")
    message("Building with link time optimization.")
    add_definitions(-flto)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_C_FLAGS} -flto -fuse-linker-plugin -fwhole-program")
endif()


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
add_subdirectory(net)

add_executable(cjdroute cjdroute.c)
target_link_libraries(cjdroute
    crypto
    crypto_benchmark
    interface
    switch
    dht
    dhtcore
    cjdbenc
    cjdbenc_JsonBencSerializer
    cjdmemory
    cjdadmin
    cjdnet
    ${LIBEVENT2_LIBRARIES}
)

install_targets(/bin cjdroute)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
