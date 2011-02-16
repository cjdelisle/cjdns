#include "ArrayWriter.h"
#include <string.h>

struct ArrayWriter_context {
    char* beginPointer;
    char* pointer;
    char* endPointer;
    int returnCode;
};

static int write(void* toWrite, size_t length, struct Writer* writer);
static uint64_t bytesWritten(struct Writer* writer);

/** @see ArrayWriter.h */
struct Writer* ArrayWriter_new(void* writeToBuffer,
                               size_t length,
                               struct MemAllocator* allocator)
{
    struct Writer* writer =
        allocator->calloc(sizeof(struct Writer), 1, allocator);
    struct ArrayWriter_context* context =
        allocator->calloc(sizeof(struct ArrayWriter_context), 1, allocator);

    if (context == NULL || writer == NULL) {
        return NULL;
    }

    context->beginPointer = (char*) writeToBuffer;
    context->pointer = (char*) writeToBuffer;
    context->endPointer = (char*) writeToBuffer + length;

    struct Writer localWriter = {
        .context = context,
        .write = write,
        .bytesWritten = bytesWritten
    };
    memcpy(writer, &localWriter, sizeof(struct Writer));

    return writer;
}

/** @see Writer->write() */
static int write(void* toWrite, size_t length, struct Writer* writer)
{
    struct ArrayWriter_context* context =
        (struct ArrayWriter_context*) writer->context;

    /* If there was a previous failure then don't allow any more writing. */
    if (context->returnCode != 0) {
        return context->returnCode;
    }

    /* Prove that it doesn't run off the end of the buffer or roll over. */
    if (context->pointer + length >= context->endPointer
        || context->pointer + length < context->pointer)
    {
        context->returnCode = -1;
        return -1;
    }

    memcpy(context->pointer, toWrite, length);
    context->pointer += length;

    return 0;
}

static uint64_t bytesWritten(struct Writer* writer)
{
    struct ArrayWriter_context* context =
        (struct ArrayWriter_context*) writer->context;

    return context->pointer - context->beginPointer;
}
