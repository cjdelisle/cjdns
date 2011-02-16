/*
 * This test tests the ability of DHTModules.c to serialize the contexts
 * of the modules. For (de)serialization of the messages go in and out,
 * see SerializationModule_test.c
 */

#include <stdio.h>
#include "libbenc/bencode.h"
#include "dht/DHTModules.h"

int testSerialization()
{
    benc_bstr_t* serialize(void* vcontext)
    {
        char* context = (char*) vcontext;
        benc_bstr_t* out = calloc(sizeof(benc_bstr_t), 1);
        if (out == NULL) {
            return NULL;
        }
        out->len = strlen(context);
        out->bytes = context;
        return out;
    }

    char* context = "Hello World";

    struct DHTModule module = {
        .name = "TestModule",
        .context = context,
        .serialize = serialize
    };

    struct DHTModuleRegistry* reg = DHTModules_new();
    DHTModules_register(&module, reg);

    benc_bstr_t* serialized = DHTModules_serialize(reg);
    DHTModules_free(reg);

    /* This is ok because the output is null padded at the end. */
    /*
    printf("The content is: %s\nand the length is: %d\n",
           serialized->bytes,
           (int) serialized->len);
    */

    return memcmp(serialized->bytes,
                  "d10:TestModule11:Hello Worlde",
                  serialized->len);
}

int testDeserialization()
{
    void deserialize(const benc_bstr_t serialData, void* vcontext)
    {
        char* context = (char*) vcontext;
        memcpy(context, serialData.bytes, serialData.len);
    }

    char context[12];

    struct DHTModule module = {
        .name = "TestModule",
        .context = context,
        .deserialize = deserialize
    };

    benc_bstr_t serialized = {29, "d10:TestModule11:Hello Worlde"};

    struct DHTModuleRegistry* reg = DHTModules_deserialize(serialized);
    DHTModules_register(&module, reg);

    context[11] = '\0';

    /*printf("Deserialization output is: %s\n", context);*/

    return memcmp(context, "Hello World", 11);
}

int main()
{
    return
        testSerialization()
      | testDeserialization();
}
