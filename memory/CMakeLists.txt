project(MemAllocator C)

cmake_minimum_required(VERSION 2.4)

add_library(MemAllocator
    BufferAllocator.c
    MallocAllocator.c
)

# Everything must be tested.
enable_testing()
#add_subdirectory(test) later
