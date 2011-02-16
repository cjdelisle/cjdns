#ifndef ARRAYWRITER_H
#define ARRAYWRITER_H

#include "Writer.h"
#include <memory/MemAllocator.h>

/**
 * Create a new Writer which writes to a user supplied buffer.
 *
 * @param writeTo the array which should be written to.
 * @param length the size of the given buffer.
 * @param allocator the memory allocator to use for allocating the writer and context.
 */
struct Writer* ArrayWriter_new(void* writeTo,
                               size_t length,
                               struct MemAllocator* allocator);

#endif
