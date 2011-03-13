#include "FileWriter.h"

/* For memcpy. */
#include <string.h>

struct FileWriter_context {
    FILE* writeTo;
    uint64_t bytesWritten;
    struct Writer writer;
};

static int write(const void* toWrite, size_t length, const struct Writer* writer);
static uint64_t bytesWritten(const struct Writer* writer);

/** @see ArrayWriter.h */
struct Writer* FileWriter_new(FILE* writeTo, const struct MemAllocator* allocator)
{
    struct FileWriter_context* context =
        allocator->calloc(sizeof(struct FileWriter_context), 1, allocator);

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
static int write(const void* toWrite, size_t length, const struct Writer* writer)
{
    struct FileWriter_context* context =
        (struct FileWriter_context*) writer->context;

    size_t written = fwrite(toWrite, 1, length, context->writeTo);

    context->bytesWritten += written;

    return written - length;
}

static uint64_t bytesWritten(const struct Writer* writer)
{
    struct FileWriter_context* context =
        (struct FileWriter_context*) writer->context;

    return context->bytesWritten;
}
