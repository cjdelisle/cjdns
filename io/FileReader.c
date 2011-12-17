#include "io/Reader.h"
#include "memory/MemAllocator.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024

struct Context {
    FILE* toRead;
    char buffer[BUFFER_SIZE];
    char* pointer;
    char* endPointer;
    bool skipFailed;
    struct Reader reader;
};

static int read(void* readInto, size_t length, const struct Reader* reader);
static void skip(size_t byteCount, const struct Reader* reader);

/** @see ArrayReader.h */
struct Reader* FileReader_new(FILE* toRead, const struct MemAllocator* allocator)
{
    struct Context* context = allocator->calloc(sizeof(struct Context), 1, allocator);

    context->toRead = toRead;
    context->pointer = context->buffer;
    context->endPointer = context->buffer + fread(context->buffer, 1, BUFFER_SIZE, toRead);

    struct Reader localReader = {
        .context = context,
        .read = read,
        .skip = skip
    };
    memcpy(&context->reader, &localReader, sizeof(struct Reader));

    return &context->reader;
}

/** @see Reader->read() */
static int read(void* readInto, size_t length, const struct Reader* reader)
{
    struct Context* context = (struct Context*) reader->context;

    if (length == 0) {
        int ret = read(readInto, 1, reader);
        context->pointer--;
        return ret;
    }

    if (context->pointer + length > context->endPointer) {
        size_t firstRead = context->endPointer - context->pointer;
        if (firstRead > 0) {
            read(readInto, firstRead, reader);
        }
        if (length - firstRead >= BUFFER_SIZE) {
            fread((char*)readInto + firstRead, 1, length - firstRead, context->toRead);
        }
        context->pointer = context->buffer;
        context->endPointer =
            context->buffer + fread(context->buffer, 1, BUFFER_SIZE, context->toRead);
        if (length - firstRead >= BUFFER_SIZE) {
            return 0;
        }
        if (context->pointer == context->endPointer) {
            // EOF
            return -1;
        }
        if (length - firstRead < BUFFER_SIZE) {
            return read((char*)readInto + firstRead, length - firstRead, reader);
        }
    }

    // Check for integer overflow
    if (context->pointer + length < context->pointer) {
        return -2;
    }

    memcpy(readInto, context->pointer, length);
    context->pointer += length;

    return 0;
}

/** @see Reader->skip() */
static void skip(size_t length, const struct Reader* reader)
{
    struct Context* context = (struct Context*) reader->context;

    while (context->pointer + length > context->endPointer) {
        length -= (context->endPointer - context->pointer + 1);
        char* discard;
        context->pointer = context->endPointer;
        if (read(&discard, 1, reader)) {
            context->skipFailed = true;
            return;
        }
    }

    context->pointer += length;
}
