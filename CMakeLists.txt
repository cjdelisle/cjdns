project(cjdns C)
cmake_minimum_required(VERSION 2.4)

if(CMAKE_BINARY_DIR STREQUAL ${CMAKE_SOURCE_DIR})
        message( FATAL_ERROR "Building stuff in source dir sucks. Remove CMakeCache.txt from ${CMAKE_SOURCE_DIR} and read the README.Build" )
endif(CMAKE_BINARY_DIR STREQUAL ${CMAKE_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(Libevent2 REQUIRED)

add_definitions(-Wall -Wextra -std=gnu99 -Werror -g)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(memory)
add_subdirectory(io)
add_subdirectory(libbenc)
add_subdirectory(net)
add_subdirectory(crypto)
add_subdirectory(dht)
add_subdirectory(dns)

include_directories(${LIBEVENT2_INCLUDE_DIRS})

add_executable(cjdns cjdns.c BridgeModule.c)
target_link_libraries(cjdns crypto dht dns net MemAllocator ${LIBEVENT2_LIBRARIES})

# Everything must be tested.
enable_testing()
