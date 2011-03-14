add_definitions(-Wall -Wextra -std=c99 -Werror -pedantic)

add_library(benc_JsonSerializer JsonBencSerializer.c)
target_link_libraries(benc_JsonSerializer MemAllocator cjdio)

# Everything must be tested.
enable_testing()
#add_subdirectory(test)
