/**
 * Get a pointer which is aligned on memory boundries.
 *
 * @param pointer the location where the pointer should be.
 * @param alignedOn how big the word is that the boundry should be aligned on.
 */
#define AllocatorTools_getAligned(pointer, alignedOn) \
    ((char*) ((uintptr_t)( ((char*)(pointer)) + (alignedOn) - 1) & ~ ((alignedOn) - 1)))
