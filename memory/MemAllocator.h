#ifndef MEMALLOCATOR_H
#define MEMALLOCATOR_H

#include <stdlib.h>

struct MemAllocator;

/**
 * Writer interface which writes data to memory and
 * provides pointers to the memory locations where it wrote.
 */
struct MemAllocator {
    /** The internal state of the MemAllocator. */
    void* const context;

    /**
     * Free the heap allocations held by the allocator and any of it's children.
     *
     * @param this the MemAllocator which is being called. Use: allocator->free(allocator);
     */
    void (* const free)(struct MemAllocator* this);

    /**
     * Allocate some memory from this memory allocator.
     * The allocation will be aligned on the size of a pointer, if you need further alignment then
     * you must handle it manually.
     *
     * @param numberOfBytes how much memory to allocate.
     * @param this the memory allocator, use allocator->malloc(10, allocator) to allocate 10 bytes.
     * @return a pointer to the newly allocated memory or NULL if allocation fails.
     * @see malloc()
     */
    void* (* const malloc)(size_t numberOfBytes,
                           struct MemAllocator* this);

    /**
     * Allocate some memory from this memory allocator.
     * The allocation will be aligned on the size of a pointer, if you need further alignment then
     * you must handle it manually.
     * Memory location will be initialized to 0.
     *
     * @param numberOfBytes how much memory to allocate.
     * @param multiplier how many times the number of bytes to allocate.
     * @param this the memory allocator, use allocator->malloc(10, allocator) to allocate 10 bytes.
     * @return a pointer to the newly allocated memory or NULL if allocation fails.
     * @see calloc()
     */
    void* (* const calloc)(size_t numberOfBytes,
                           size_t multiplier,
                           struct MemAllocator* this);

    /**
     * Get a new child of this allocator.
     * When this allocator is freed all of it's children will be freed as well.
     *
     * @param this the memory allocator, use allocator->child(allocator) to get a child.
     * @return a child allocator.
     */
    struct MemAllocator* (* const child)(struct MemAllocator* this);
};

#endif /* MEMALLOCATOR_H */
