find_package(Libevent2 REQUIRED)

add_subdirectory(dhtcore)

add_library(dht
    CJDHTConstants.c
    DHTModules.c
    #LibeventNetworkModule.c
    SerializationModule.c
    #DebugModule.c
    DHTConstants.c
    ReplyModule.c
    SanityModule.c
    RateLimitModule.c
)
target_link_libraries(dht benc benc_StandardBencSerializer util)#dhtstore)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
