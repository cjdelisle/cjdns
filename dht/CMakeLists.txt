add_definitions(-Wall -Wextra -std=gnu99 -Werror)

find_package(Libevent2 REQUIRED)

add_subdirectory(core)

add_library(dht DHTConstants.c DHTModules.c LibeventNetworkModule.c SerializationModule.c DebugModule.c)
target_link_libraries(dht dhtcore)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
