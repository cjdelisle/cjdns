message("Running NACL tests:")
file(GLOB tests "${CMAKE_BINARY_DIR}/nacl/build/bin/tests/*")
foreach(test ${tests})
    if(NOT ${test} MATCHES ".out$")
        string(REGEX REPLACE ".*/" "" shortName "${test}")
        message("    ${shortName}")
        execute_process(
            COMMAND ${test}
            RESULT_VARIABLE returnCode
            OUTPUT_VARIABLE out
        )
        if(NOT "${returnCode}" STREQUAL "0")
            message(FATAL_ERROR "${test} failed returning ${returnCode}")
        endif()
        file(READ "${test}.out" expected)
        if(NOT "${out}" STREQUAL "${expected}")
            message("expected:")
            message("${expected}")
            message("got:")
            message("${out}")
            message(FATAL_ERROR "${test} gave wrong output")
        endif()
    endif()
endforeach()
file(WRITE ${CMAKE_BINARY_DIR}/nacl/build/nacl_test.out "it passed, yay!")

