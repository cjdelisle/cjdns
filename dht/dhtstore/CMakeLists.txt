add_definitions(-Wall -Wextra -std=c89 -Werror)

add_library(dhtstore
    DHTStoreModule.c
)
target_link_libraries(dhtstore crypto benc)

# Everything must be tested.
enable_testing()
#add_subdirectory(test)
