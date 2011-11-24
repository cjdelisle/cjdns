if (NACL_INCLUDE_DIRS AND NACL_LIBRARIES)
    set(NACL_FOUND TRUE)
endif (NACL_INCLUDE_DIRS AND NACL_LIBRARIES)


if (NOT NACL_FOUND)
    message("Checking for nacl crypto library.")
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/nacl-build")
        MESSAGE("Compiling nacl crypto library.")
        execute_process(COMMAND sh "${CMAKE_SOURCE_DIR}/crypto/nacl/build-nacl.sh"
                        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/crypto/nacl/")
    endif(NOT EXISTS "${CMAKE_BINARY_DIR}/nacl-build")

    set(NACL_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/nacl-build/include")
    set(NACL_LIBRARIES "${CMAKE_BINARY_DIR}/nacl-build/lib/libnacl.a")
    set(NACL_FOUND TRUE)
endif (NOT NACL_FOUND)


if (NACL_FOUND)
    message(STATUS "Found libnacl: includes ${NACL_INCLUDE_DIRS} libs ${NACL_LIBRARIES}")
else (NACL_FOUND)
    if (NACL_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find libnacl, try to setup NACL_PREFIX accordingly")
    endif (NACL_FIND_REQUIRED)
endif (NACL_FOUND)
