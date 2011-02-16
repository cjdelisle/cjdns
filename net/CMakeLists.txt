project(net C)
cmake_minimum_required(VERSION 2.4)

find_package(Libevent2 REQUIRED)

add_definitions(-Wall -Wextra -std=c89 -Werror)

include_directories(${LIBEVENT2_INCLUDE_DIRS})

add_library(net NetworkTools.c)
target_link_libraries(net ${LIBEVENT2_LIBRARIES})

# Everything must be tested.
enable_testing()
#add_subdirectory(test) later
