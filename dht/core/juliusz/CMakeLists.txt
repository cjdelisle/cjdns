add_library(juliusz dht_modified.c)
#to prevent linker errors this lib must link the dhtcore library which links it.
target_link_libraries(juliusz net dhtcore)
