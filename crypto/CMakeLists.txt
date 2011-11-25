find_package(NACL REQUIRED)

include_directories(${NACL_INCLUDE_DIRS})

add_library(crypto Crypto.c HashFunctions.c CryptoAuth.c sha1/sha1.c)
target_link_libraries(crypto benc MemAllocator ${NACL_LIBRARIES})

# Everything must be tested.
enable_testing()
#add_subdirectory(test)
