find_package(Libevent2 REQUIRED)

include_directories(${LIBEVENT2_INCLUDE_DIRS})
add_library(dns DNSCheckZoneModule.c DNSModules.c DNSNetworkModule.c DNSTools.c)
target_link_libraries(dns ${LIBEVENT2_LIBRARIES})

# Everything must be tested.
enable_testing()
add_subdirectory(test)
