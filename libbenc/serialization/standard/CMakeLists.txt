add_definitions(-Wall -Wextra -std=c99 -Werror -pedantic)

add_library(benc_StandardBencSerializer StandardBencSerializer.c)
target_link_libraries(benc_StandardBencSerializer MemAllocator cjdio)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
