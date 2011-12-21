find_package(Libevent2 REQUIRED)

add_subdirectory(dhtcore)

find_package(NACL REQUIRED)

include_directories(${NACL_INCLUDE_DIRS})

add_library(dht
    CJDHTConstants.c
    DHTModules.c
    SerializationModule.c
    ReplyModule.c
    #SanityModule.c
    #RateLimitModule.c
    Ducttape.c
)
target_link_libraries(dht benc benc_StandardBencSerializer util interface ${NACL_LIBRARIES})

# Everything must be tested.
enable_testing()
add_subdirectory(test)
