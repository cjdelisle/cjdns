add_library(dhtstore
    DHTStoreConstants.c
    DHTStoreModule.c
    DHTStoreTools.c
    SHA1Store.c
)
target_link_libraries(dhtstore crypto benc)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
