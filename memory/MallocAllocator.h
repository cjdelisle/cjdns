#ifndef MALLOC_ALLOCATOR_H
#define MALLOC_ALLOCATOR_H

#include "memory/MemAllocator.h"

/**
 * Create a new MemAllocator which is a wrapper around malloc().
 *
 * @param sizeLimit the number of bytes which are allowed to be allocated by
 *                  this allocator or any of it's children before the program
 *                  will be halted with an error.
 */
struct MemAllocator* MallocAllocator_new(size_t sizeLimit);

#endif
