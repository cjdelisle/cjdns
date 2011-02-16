#include "ArrayReader.h"
#include <string.h>

struct ArrayReader_context {
    char* pointer;
    char* endPointer;
};

static int read(void* readInto, size_t length, struct Reader* reader);
static void skip(size_t byteCount, struct Reader* reader);

/** @see ArrayReader.h */
struct Reader* ArrayReader_new(const void* bufferToRead,
                               size_t length,
                               struct MemAllocator* allocator)
{
    struct Reader* reader =
        allocator->calloc(sizeof(struct Reader), 1, allocator);
    struct ArrayReader_context* context =
        allocator->calloc(sizeof(struct ArrayReader_context), 1, allocator);

    if (context == NULL || reader == NULL) {
        return NULL;
    }

    context->pointer = (char*) bufferToRead;
    context->endPointer = (char*) bufferToRead + length;

    struct Reader localReader = {
        .context = context,
        .read = read,
        .skip = skip
    };
    memcpy(reader, &localReader, sizeof(struct Reader));

    return reader;
}

/** @see Reader->read() */
static int read(void* readInto, size_t length, struct Reader* reader)
{
    struct ArrayReader_context* context =
        (struct ArrayReader_context*) reader->context;

    /* Prove that it doesn't run off the end of the buffer or roll over. */
    if (context->pointer + length >= context->endPointer
        || context->pointer + length < context->pointer)
    {
        return -1;
    }

    if (length == 0) {
        /* Allow peaking. */
        *((char*)readInto) = *context->pointer;
        return 0;
    }

    memcpy(readInto, context->pointer, length);
    context->pointer += length;

    return 0;
}

/** @see Reader->skip() */
static void skip(size_t byteCount, struct Reader* reader)
{
    ((struct ArrayReader_context*) reader->context)->pointer += byteCount;
}
