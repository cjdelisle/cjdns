add_subdirectory(seccure)

add_library(crypto Crypto.c HashFunctions.c)
target_link_libraries(crypto seccure benc MemAllocator)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
