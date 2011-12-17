project(cjdio C)
cmake_minimum_required(VERSION 2.4)

add_library(cjdio ArrayReader.c ArrayWriter.c FileReader.c FileWriter.c)
target_link_libraries(cjdio MemAllocator)

# Everything must be tested.
enable_testing()
#add_subdirectory(test) later
