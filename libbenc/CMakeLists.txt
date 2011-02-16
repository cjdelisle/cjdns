add_definitions(-Wall -Wextra -std=c89 -Werror)

add_library(benc benc_bstr.c benc_dict.c benc_int.c benc_list.c bencode.c)
target_link_libraries(benc MemAllocator cjdio)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
