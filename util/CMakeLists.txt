project(util C)
cmake_minimum_required(VERSION 2.4)

find_package(Libevent2 REQUIRED)

add_library(util
    AverageRoller.c
    Timeout.c
    Hex.c
)
target_link_libraries(util MemAllocator ${LIBEVENT2_LIBRARIES})

# Everything must be tested.
enable_testing()
add_subdirectory(test)
