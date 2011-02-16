project(cjdio C)
cmake_minimum_required(VERSION 2.4)

include_directories(../memory)

add_definitions(-Wall -Wextra -std=c89 -Werror)

add_library(cjdio ArrayReader.c ArrayWriter.c)
target_link_libraries(cjdio MemAllocator)

# Everything must be tested.
enable_testing()
#add_subdirectory(test) later
