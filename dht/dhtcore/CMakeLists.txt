find_package(Libevent2 REQUIRED)

add_library(dhtcore
    NodeStore.c
    RouterModule.c
    SearchStore.c
    Janitor.c
)

target_link_libraries(dhtcore benc util crypto switch ${LIBEVENT2_LIBRARIES})#dhtstore)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
