#include <stdio.h>
#include <time.h>

#include "memory/BufferAllocator.h"

#define IS_TESTING
#include "dht/RouterModule.c"

int main()
{
    srand(time(NULL));
    char buffer[1024];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 1024);
    struct SearchNodeIndex index = {rand() % MAX_SEARCHES, rand() % SEARCH_NODES};
    String* str = tidForSearchNodeIndex(&index, allocator);
    struct SearchNodeIndex index2 = searchNodeIndexForTid(str);
    if (index.search - index2.search + index.node - index2.node) {
        printf("inputs were %d and %d\noutputs were %d and %d\nintermediet was %s\nlength was: %d",
               index.search, index.node, index2.search, index2.node, str->bytes, (int) str->len);
        return -1;
    }
    return 0;
}
