add_library(benc_JsonBencSerializer JsonBencSerializer.c)
target_link_libraries(benc_JsonBencSerializer MemAllocator cjdio util benc)

# Everything must be tested.
enable_testing()
#add_subdirectory(test)
