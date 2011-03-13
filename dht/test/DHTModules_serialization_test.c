/*
 * This test tests the ability of DHTModules.c to serialize the contexts
 * of the modules. For (de)serialization of the messages go in and out,
 * see SerializationModule_test.c
 */

#include <stdio.h>
#include <string.h>

#include "libbenc/bencode.h"
#include "dht/DHTModules.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"

static const char* control = "d10:TestModule11:Hello Worlde";

static bobj_t* serialize(void* vcontext)
{
    char* context = (char*) vcontext;
    benc_bstr_t* str = calloc(sizeof(benc_bstr_t), 1);
    bobj_t* out = calloc(sizeof(bobj_t), 1);
    if (out == NULL || str == NULL) {
        return NULL;
    }
    str->len = strlen(context);
    str->bytes = context;
    out->type = BENC_BSTR;
    out->as.bstr = str;
    return out;
}

static int testSerialization()
{
    char* context = "Hello World";

    struct DHTModule module = {
        .name = "TestModule",
        .context = context,
        .serialize = serialize
    };

    char buffer[256];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 256);
    char writeBuffer[64];
    memset(writeBuffer, 0, 64);
    struct Writer* writer = ArrayWriter_new(writeBuffer, 64, allocator);

    struct DHTModuleRegistry* reg = DHTModules_new(/*allocator*/);
    DHTModules_register(&module, reg);

    DHTModules_serialize(reg, writer);

    /* This is ok because the output is null padded at the end. */
    printf("The content is: %s\nand the length is: %d\n",
           writeBuffer,
           (int) writer->bytesWritten(writer));

    if (writer->bytesWritten(writer) != strlen(control)) {
        printf("Length mismatch! expected: %d", (int) strlen(control));
    }

    return memcmp(writeBuffer, control, writer->bytesWritten(writer));
}

static void deserialize(const bobj_t* serialData, void* vcontext)
{
    char* context = (char*) vcontext;
    memcpy(context, serialData->as.bstr->bytes, serialData->as.bstr->len);
}

int testDeserialization()
{
    char context[12];

    struct DHTModule module = {
        .name = "TestModule",
        .context = context,
        .deserialize = deserialize
    };

    char buffer[256];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 256);
    struct Reader* reader = ArrayReader_new(control, strlen(control), allocator);

    struct DHTModuleRegistry* reg = DHTModules_deserialize(reader, allocator);
    DHTModules_register(&module, reg);

    context[11] = '\0';

    printf("Deserialization output is: %s\n", context);

    return memcmp(context, "Hello World", 11);
}

int main()
{
    return
        testSerialization()
      | testDeserialization();
}
