#ifndef BUFFERALLOCATOR_H
#define BUFFERALLOCATOR_H

#include "MemAllocator.h"

/**
 * Create a new MemAllocator which allocates from to a user supplied buffer.
 * This allocator will reset the pointer to the beginning of the buffer when
 * free() is called on it, it is up to the user to free their buffer.
 * allocator->child(allocator) always returns NULL.
 *
 * @param buffer an array to write to.
 * @param length the size of the array. If more is written than this length,
 *               further allocations will fail and return NULL.
 */
struct MemAllocator* BufferAllocator_new(void* buffer, size_t length);

#endif /* BUFFERALLOCATOR_H */
