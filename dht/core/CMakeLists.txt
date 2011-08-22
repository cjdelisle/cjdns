add_subdirectory(juliusz)

add_library(dhtcore LegacyConnectorModule.c Parsing.c Serializing.c)
target_link_libraries(dhtcore benc juliusz dht)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
