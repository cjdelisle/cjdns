add_library(dhtstore
    DHTStoreModule.c
    DHTStoreTools.c
    SHA1Store.c
    PeerAddressStore.c
)
target_link_libraries(dhtstore crypto benc)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
