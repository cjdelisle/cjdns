#ifndef ARRAYWRITER_H
#define ARRAYWRITER_H

#include "Writer.h"
#include <memory/MemAllocator.h>
#include <stdio.h>

/**
 * Create a new Writer which writes to a file or stream.
 *
 * @param writeTo the file/stream which should be written to.
 * @param allocator the memory allocator to use for allocating the writer and context.
 */
struct Writer* FileWriter_new(FILE* writeTo, const struct MemAllocator* allocator);

#endif
