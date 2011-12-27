project(cjdns C)
cmake_minimum_required(VERSION 2.4)

if(CMAKE_BINARY_DIR STREQUAL ${CMAKE_SOURCE_DIR})
        message( FATAL_ERROR "type: mkdir build ; cd build ; cmake .. ; make" )
endif(CMAKE_BINARY_DIR STREQUAL ${CMAKE_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(Libevent2 REQUIRED)

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

#add_definitions(-O3)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(memory)
add_subdirectory(io)
add_subdirectory(libbenc)
add_subdirectory(net)
add_subdirectory(crypto)
add_subdirectory(dht)
add_subdirectory(dns)
add_subdirectory(util)
add_subdirectory(switch)
add_subdirectory(interface)

include_directories(${LIBEVENT2_INCLUDE_DIRS})

add_executable(cjdvpn cjdvpn.c)
target_link_libraries(cjdvpn
    crypto
    interface
)

add_executable(cjdroute cjdroute.c)
target_link_libraries(cjdroute
    crypto
    interface
    switch
    dht
    dhtcore
    benc
    benc_JsonBencSerializer
    MemAllocator
    ${LIBEVENT2_LIBRARIES}
)

add_executable(benc2json benc2json.c)
target_link_libraries(benc2json
    benc
    benc_StandardBencSerializer
    benc_JsonBencSerializer
    MemAllocator
    cjdio
)

find_package(NACL REQUIRED)
include_directories(${NACL_INCLUDE_DIRS})
add_executable(addressgen addressgen.c)
target_link_libraries(addressgen
    crypto
    util
)

# Everything must be tested.
enable_testing()
