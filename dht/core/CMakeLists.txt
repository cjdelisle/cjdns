add_subdirectory(juliusz)

add_library(dhtoldcore
    LegacyConnectorModule.c
    Parsing.c
    Serializing.c
)
target_link_libraries(dhtoldcore benc juliusz dht)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
