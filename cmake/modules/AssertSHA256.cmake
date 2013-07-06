if("${FILE}" STREQUAL "")
    message(FATAL_ERROR "Can't take the hash of nothing!")
endif()



if(${CMAKE_PATCH_VERSION} GREATER 6)
    file(SHA256 "${FILE}" hash)
else()
    find_program(sha256sum sha256sum)
    if(NOT sha256sum)
        find_program(sha256sum gsha256sum)
    endif()
    if(NOT sha256sum)
        find_program(shasum shasum)
        if (shasum)
            set(sha256sum shasum -a 256)
        endif()
    endif()
    if(NOT sha256sum)
        find_program(openssl openssl)
        if (openssl)
            set(sha256sum openssl sha256)
        endif()
    endif()

    if(NOT sha256sum)
        message(FATAL_ERROR "could not find sha256 checksummer.")
    endif()

    execute_process(COMMAND ${sha256sum} "${FILE}"
        RESULT_VARIABLE returnCode
        OUTPUT_VARIABLE hash
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

if(NOT "${hash}" MATCHES ".*${EXPECTED}.*")
    message("expected: [${EXPECTED}] got: [${trimmedHash}]")
    message(FATAL_ERROR "${FILE} SHA-256 check failed")
endif()

message("Hash of [${FILE}] good: ${trimmedHash}")
