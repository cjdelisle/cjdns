find_program(sha256sum sha256sum)
if(NOT sha256sum)
    find_program(sha256sum sha256)
endif()

if(NOT sha256sum)
    message(FATAL_ERROR "could not find sha256 checksummer.")
endif()

if("${FILE}" STREQUAL "")
    message(FATAL_ERROR "Can't take the hash of nothing!")
endif()

execute_process(COMMAND ${sha256sum} "${FILE}"
    RESULT_VARIABLE returnCode
    OUTPUT_VARIABLE hash
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE " .*$" "" trimmedHash "${hash}")
if(NOT "${trimmedHash}" STREQUAL "${EXPECTED}")
    message("expected: [${EXPECTED}] got: [${trimmedHash}]")
    message(FATAL_ERROR "${FILE} SHA-256 check failed")
endif()

message("Hash of [${FILE}] good: ${trimmedHash}")
