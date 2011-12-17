#include "libbenc/benc.h"
#include "io/Reader.h"
#include "io/FileReader.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "libbenc/benc.h"
#include "libbenc/serialization/BencSerializer.h"
#include "libbenc/serialization/standard/StandardBencSerializer.h"
#include "libbenc/serialization/json/JsonBencSerializer.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"

#include <assert.h>

int main(int argc, char** argv)
{
    uint8_t buffer[1<<20];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 1<<20);

    struct Reader* reader = FileReader_new(stdin, allocator);
    struct Writer* writer = FileWriter_new(stdout, allocator);
    Dict dict;

    if (argc > 1 && argv[1][0] == 'r') {
        int ret = 0;
        assert(!(ret = benc_getJsonBencSerializer()->parseDictionary(reader, allocator, &dict)));
        assert(!(ret = benc_getStandardBencSerializer()->serializeDictionary(writer, &dict)));
    } else {
        assert(!benc_getStandardBencSerializer()->parseDictionary(reader, allocator, &dict));
        assert(!benc_getJsonBencSerializer()->serializeDictionary(writer, &dict));
        printf("\n");
    }
}
