cmake_minimum_required(VERSION 2.4)

include_directories(${CMAKE_SOURCE_DIR})

string(REGEX REPLACE "^.*/" "" main_dir_name ${CMAKE_SOURCE_DIR})
string(REPLACE ${CMAKE_SOURCE_DIR} ${main_dir_name} this_dir ${CMAKE_CURRENT_SOURCE_DIR})
message("-- Tests to run for " ${this_dir})
add_definitions(-g)

# Anything in this dir which ends with "test.c" is considered a test.
file(GLOB tests "*test.c")
foreach(test_source_fullpath ${tests})
  string(REGEX REPLACE "^.*/" "" test_source ${test_source_fullpath})
  string(REPLACE "test.c" "test" test_bin ${test_source})
  message("     " ${test_source})
  add_executable(${test_bin} ${test_source})
  target_link_libraries(${test_bin} benc cjdio MemAllocator)
  add_test(${test_bin} ${test_bin})
endforeach()
# Add an empty line after tests.
message("")
