#ifndef FILEREADER_H
#define FILEREADER_H

#include "io/Reader.h"
#include "memory/MemAllocator.h"

#include <stdio.h>

/**
 * Create a new Reader which reads a file or stream.
 *
 * @param toRead the file to read from.
 * @param allocator the memory allocator to use for allocating the reader and context.
 */
struct Reader* FileReader_new(FILE* toRead,
                              const struct MemAllocator* allocator);

#endif
