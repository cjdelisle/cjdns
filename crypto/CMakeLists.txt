add_subdirectory(seccure)

add_library(crypto Crypto.c)
target_link_libraries(crypto seccure)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
