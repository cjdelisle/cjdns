project(cjdns C)
cmake_minimum_required(VERSION 2.4)

if(CMAKE_BINARY_DIR STREQUAL ${CMAKE_SOURCE_DIR})
        message( FATAL_ERROR "type: mkdir build ; cd build ; cmake .. ; make" )
endif(CMAKE_BINARY_DIR STREQUAL ${CMAKE_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(Libevent2 REQUIRED)

# validation
add_definitions(-Wall -Wextra -std=c99 -Werror -pedantic)

# hardening
add_definitions(
    -pie
    -fPIE
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

include_directories(${LIBEVENT2_INCLUDE_DIRS})

add_executable(cjdns cjdns.c BridgeModule.c)
target_link_libraries(cjdns
    crypto
    dht
    dhtstore
    dhtcore
    dns
    net
    MemAllocator
    ${LIBEVENT2_LIBRARIES}
)

# Everything must be tested.
enable_testing()
