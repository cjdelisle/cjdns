#ifndef ARRAYREADER_H
#define ARRAYREADER_H

#include "Reader.h"
#include <memory/MemAllocator.h>

/**
 * Create a new Reader which reads a user supplied buffer.
 *
 * @param bufferToRead the array which should be read from.
 * @param length the size of the given buffer.
 * @param allocator the memory allocator to use for allocating the writer and context.
 */
struct Reader* ArrayReader_new(const void* bufferToRead,
                               size_t length,
                               struct MemAllocator* allocator);

#endif
