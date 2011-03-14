#include <string.h>
#include <stdio.h>

#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "libbenc/benc.h"
#include "libbenc/serialization/BencSerializer.h"
#include "libbenc/serialization/standard/StandardBencSerializer.h"

int expect(char* str, struct Writer* writer, struct Reader* reader, int ret)
{
    char buffer[32];
    writer->write("\0", 1, writer);
    reader->read(buffer, strlen(str) + 1, reader);
    if (strcmp(str, buffer) != 0) {
        printf("Expected %s\n Got %s\n", str, buffer);
        return -1;
    }
    return ret;
}

int testSerialize(struct Writer* writer, struct Reader* reader)
{
    int ret = 0;

    ret |= StandardBencSerializer.serializeInteger(writer, 1);
    ret |= expect("i1e", writer, reader, ret);

    ret |= StandardBencSerializer.serializeInteger(writer, 1000);
    ret |= expect("i1000e", writer, reader, ret);

    ret |= StandardBencSerializer.serializeInteger(writer, -100);
    ret |= expect("i-100e", writer, reader, ret);

    return ret;
}


int main()
{
    char buffer[512];
    char out[512];
    struct MemAllocator* alloc = BufferAllocator_new(buffer, 512);
    struct Writer* writer = ArrayWriter_new(out, 512, alloc);
    struct Reader* reader = ArrayReader_new(out, 512, alloc);

    return testSerialize(writer, reader);
}
