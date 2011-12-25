find_package(NACL REQUIRED)

include_directories(${NACL_INCLUDE_DIRS})

# Libevent is used to get random numbers.
include_directories(${LIBEVENT2_INCLUDE_DIRS})

add_library(crypto Crypto.c CryptoAuth.c HashFunctions.c AddressCalc.c sha1/sha1.c)
target_link_libraries(crypto
    benc
    MemAllocator
    ${NACL_LIBRARIES}
    ${LIBEVENT2_LIBRARIES}
    util
)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
