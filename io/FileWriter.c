#include "FileWriter.h"

/* For memcpy. */
#include <string.h>

struct ArrayWriter_context {
    FILE* writeTo;
    uint64_t bytesWritten;
    struct Writer writer;
};

static int write(void* toWrite, size_t length, struct Writer* writer);
static uint64_t bytesWritten(struct Writer* writer);

/** @see ArrayWriter.h */
struct Writer* FileWriter_new(FILE* writeTo, struct MemAllocator* allocator)
{
    struct ArrayWriter_context* context =
        allocator->calloc(sizeof(struct ArrayWriter_context), 1, allocator);

    if (context == NULL) {
        return NULL;
    }

    context->writeTo = writeTo;
    context->bytesWritten = 0;

    struct Writer localWriter = {
        .context = context,
        .write = write,
        .bytesWritten = bytesWritten
    };
    memcpy(&context->writer, &localWriter, sizeof(struct Writer));

    return &context->writer;
}

/** @see Writer->write() */
static int write(void* toWrite, size_t length, struct Writer* writer)
{
    struct ArrayWriter_context* context =
        (struct ArrayWriter_context*) writer->context;

    size_t written = fwrite(toWrite, 1, length, context->writeTo);

    context->bytesWritten += written;

    return written - length;
}

static uint64_t bytesWritten(struct Writer* writer)
{
    struct ArrayWriter_context* context =
        (struct ArrayWriter_context*) writer->context;

    return context->bytesWritten;
}
