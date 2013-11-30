#
# Public Domain.
#
cmake_minimum_required(VERSION 2.4)


function(dependenciesForFile file operations output)
    # If it's not in the top of the file, we'll just miss it.
    file(STRINGS "${file}" lines LIMIT_COUNT 16)
    foreach(line ${lines})
        if("${line}" MATCHES "#include ")
            foreach(op ${operations})
                if("${line}" MATCHES "${op}.h")
                    list(APPEND outList "${op}")
                endif()
            endforeach()
        endif()
    endforeach()
    set(${output} "${outList}" PARENT_SCOPE)
endfunction()


include_directories(
    "${CMAKE_BINARY_DIR}/include/"
    "${CMAKE_BINARY_DIR}/include_internal/"
)


string(REGEX REPLACE "^.*/" "" main_dir_name ${CMAKE_SOURCE_DIR})
string(REPLACE ${CMAKE_SOURCE_DIR} ${main_dir_name} this_dir ${CMAKE_CURRENT_SOURCE_DIR})
message("-- Tests to run for " ${this_dir})
add_definitions(-g)

include("${CMAKE_SOURCE_DIR}/cmake/Primitives.cmake")
file(STRINGS "${CMAKE_SOURCE_DIR}/OPERATIONS" OPERATIONS)
Primitives_get("${OPERATIONS}" opPrimitives)

# Anything in this dir which ends with ".c" is considered a test.
file(GLOB tests "*.c")
foreach(test_source_fullpath ${tests})
    string(REGEX REPLACE "^.*/" "" test_source ${test_source_fullpath})
    string(REPLACE ".c" "_test" test_bin ${test_source})
    set(allTests "")

    dependenciesForFile(${test_source_fullpath} "${OPERATIONS}" depends)
    foreach(op ${depends})
        foreach(opPrim ${opPrimitives})
            if(${opPrim} MATCHES "^${op}_.*")
                string(REPLACE "${op}_" "" primitive "${opPrim}")
                set(testPrimBin ${test_bin}_${primitive})
                add_executable(${testPrimBin} ${test_source})
                # Replace with target_include_directories when that branch is merged.
                set_property(TARGET ${testPrimBin} APPEND PROPERTY COMPILE_FLAGS
                    "-I${CMAKE_BINARY_DIR}/${op}/${primitive}")
                target_link_libraries(${testPrimBin} randombytes nacl)
                add_test(${testPrimBin} ${testPrimBin})
                list(APPEND allTests ${testPrimBin})
            endif()
        endforeach()
    endforeach()

    if("${depends}" STREQUAL "")
        add_executable(${test_bin} ${test_source})
        target_link_libraries(${test_bin} nacl randombytes)
        add_test(${test_bin} ${test_bin})
        list(APPEND allTests ${test_bin})
    endif()

    # Get expected output.
    foreach(test ${allTests})
        string(REPLACE "_test" "" testOut "${test}")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${testOut}.out")
            set(outFile "${CMAKE_CURRENT_SOURCE_DIR}/${testOut}.out")
        else()
            string(REPLACE "_test" ".out" outFile "${CMAKE_CURRENT_SOURCE_DIR}/${test_bin}")
        endif()
        file(READ ${outFile} test_output)
        set_tests_properties(${test} PROPERTIES PASS_REGULAR_EXPRESSION "^${test_output}$")
    endforeach()
endforeach()
# Add an empty line after tests.
message("")
