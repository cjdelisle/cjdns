project(util C)
cmake_minimum_required(VERSION 2.4)

add_library(util AverageRoller.c)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
