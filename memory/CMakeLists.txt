project(MemAllocator C)

cmake_minimum_required(VERSION 2.4)

add_definitions(-Wall -Wextra -std=c89 -Werror)

add_library(MemAllocator BufferAllocator.c)

# Everything must be tested.
enable_testing()
#add_subdirectory(test) later
