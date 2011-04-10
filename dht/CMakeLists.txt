find_package(Libevent2 REQUIRED)

add_subdirectory(core)
add_subdirectory(dhtstore)

add_library(dht
    DHTModules.c
    LibeventNetworkModule.c
    SerializationModule.c
    DebugModule.c
    DHTConstants.c
)
target_link_libraries(dht benc benc_StandardBencSerializer)#dhtstore)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
