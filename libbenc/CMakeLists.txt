add_definitions(-Wall -Wextra -std=c99 -Werror -pedantic)

add_subdirectory(serialization)

add_library(benc String.c Dict.c List.c)
target_link_libraries(benc MemAllocator cjdio)

# Everything must be tested.
enable_testing()
