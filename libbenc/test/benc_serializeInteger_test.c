#include <libbenc/bencode.h>
#include <memory/MemAllocator.h>
#include <memory/BufferAllocator.h>
#include <io/Reader.h>
#include <io/ArrayReader.h>
#include <io/Writer.h>
#include <io/ArrayWriter.h>

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

int testSerializeRaw(struct Writer* writer, struct Reader* reader)
{
    int ret = 0;

    benc_int_print(writer, 1);
    ret = expect("1", writer, reader, ret);

    benc_int_print(writer, 1000);
    ret = expect("1000", writer, reader, ret);

    benc_int_print(writer, -100);
    ret = expect("-100", writer, reader, ret);

    return ret;
}

int testSerialize(struct Writer* writer, struct Reader* reader)
{
    int ret = 0;

    benc_int_serialize(writer, 1);
    ret = expect("i1e", writer, reader, ret);

    benc_int_serialize(writer, 1000);
    ret = expect("i1000e", writer, reader, ret);

    benc_int_serialize(writer, -100);
    ret = expect("i-100e", writer, reader, ret);

    return ret;
}


int main()
{
    char buffer[512];
    char out[512];
    struct MemAllocator* alloc = BufferAllocator_new(buffer, 512);
    struct Writer* writer = ArrayWriter_new(out, 512, alloc);
    struct Reader* reader = ArrayReader_new(out, 512, alloc);

    return   testSerializeRaw(writer, reader)
           | testSerialize(writer, reader);
}
