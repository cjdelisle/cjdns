#include <stdio.h>
#include <string.h>

#include "memory/MemAllocator.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "libbenc/benc.h"
#include "libbenc/serialization/BencSerializer.h"

static int32_t serializeGenericWithPadding(const struct Writer* writer,
                                           size_t padSpaceCount,
                                           const bobj_t* obj);

/** What the name says. */
static const char* thirtyTwoSpaces = "                                ";

/**
 * Write some number of spaces for padding.
 *
 * @param padSpaces the number of spaces to pad.
 * @param padCounter an integer which is used for internal bookkeeping.
 * @param writer where to write the padding.
 */
#define PAD(padSpaces, padCounter, writer)                              \
    padCounter = 0;                                                     \
    while (32 < padSpaces + padCounter) {                               \
        writer->write(thirtyTwoSpaces, 32, writer);                     \
        padCounter += 32;                                               \
    }                                                                   \
    writer->write(thirtyTwoSpaces, padSpaces - padCounter, writer)


/** @see BencSerializer.h */
static int32_t serializeString(const struct Writer* writer,
                               const String* string)
{
    writer->write("\"", 1, writer);
    size_t i;
    unsigned int chr;
    char buffer[4];
    for (i = 0; i < string->len; i++) {
        chr = (unsigned int) string->bytes[i] & 0xFF;
        /* Nonprinting chars, and \ and " are hex'd */
        if (chr < 0x7f && chr > 0x20 && chr != 0x5c && chr != 0x22) {
            sprintf(buffer, "%c", string->bytes[i]);
            writer->write(buffer, 1, writer);
        } else {
            sprintf(buffer, "\\x%.2X", (unsigned int) string->bytes[i] & 0xFF);
            writer->write(buffer, 4, writer);
        }
    }
    return writer->write("\"", 1, writer);
}

static int32_t notImplemented()
{
    fprintf(stderr, "Oops, parsing is (still) not implemented in the JsonBencSerializer\n"
                    "Be an hero and write it, won't you? For the children?");
    abort();
}

/** @see BencSerializer.h */
static int32_t parseString(const struct Reader* reader,
                           const struct MemAllocator* allocator,
                           benc_bstr_t** output)
{
    reader = reader;
    allocator = allocator;
    output = output;
    return notImplemented();
}

/** @see BencSerializer.h */
static int32_t serializeInteger(const struct Writer* writer,
                                benc_int_t integer)
{
    char buffer[32];
    memset(buffer, 0, 32);

    /* Need to handle 32 bit or 64 bit boxen. */
    sprintf(buffer, (sizeof(long int) == 8) ? "%ld" : "%lld", integer);

    return writer->write(buffer, strlen(buffer), writer);
}

/** @see BencSerializer.h */
static int32_t parseInteger(const struct Reader* reader,
                            Integer* output)
{
    reader = reader;
    output = output;
    return notImplemented();
}

/**
 * Serialize a bencoded list with padding at the beginning of each line.
 *
 * @param writer the place to write the output to.
 * @param padSpaceCount the number of spaces to place at the beginning of each line.
 * @param list the list to serialize
 */
static int32_t serializeListWithPadding(const struct Writer* writer,
                                        const size_t padSpaceCount,
                                        const List* list)
{
    int padCounter;

    writer->write("[\n", 2, writer);

    const benc_list_entry_t* entry = *list;
    while (entry != NULL) {
        PAD(padSpaceCount + 2, padCounter, writer);
        serializeGenericWithPadding(writer, padSpaceCount + 2, entry->elem);
        entry = entry->next;
        if (entry != NULL) {
            writer->write(",\n", 2, writer);
        }
    }

    writer->write("\n", 1, writer);
    PAD(padSpaceCount, padCounter, writer);
    return writer->write("]", 1, writer);
}

/** @see BencSerializer.h */
static int32_t serializeList(const struct Writer* writer,
                             const List* list)
{
    return serializeListWithPadding(writer, 0, list);
}

/** @see BencSerializer.h */
static int32_t parseList(const struct Reader* reader,
                         const struct MemAllocator* allocator,
                         List* output)
{
    reader = reader;
    allocator = allocator;
    output = output;
    return notImplemented();
}

/**
 * Serialize a bencoded dictionary with padding before each line.
 *
 * @param writer the place to write the output to.
 * @param padSpaceCount the number of spaces to place at the beginning of each line.
 * @param dictionary the dictionary to serialize.
 */
static int32_t serializeDictionaryWithPadding(const struct Writer* writer,
                                              size_t padSpaceCount,
                                              const Dict* dictionary)
{
    int padCounter = 0;
    writer->write("{\n", 2, writer);
    const benc_dict_entry_t* entry = *dictionary;
    while (entry != NULL) {
        PAD(padSpaceCount + 2, padCounter, writer);
        serializeString(writer, entry->key);
        writer->write(" : ", 3, writer);        
        serializeGenericWithPadding(writer, padSpaceCount + 2, entry->val);
        entry = entry->next;
        if (entry != NULL) {
            writer->write(",\n", 2, writer);
        }
    }

    writer->write("\n", 1, writer);
    PAD(padSpaceCount, padCounter, writer);
    return writer->write("}", 1, writer);
}

/** @see BencSerializer.h */
static int32_t serializeDictionary(const struct Writer* writer,
                                   const Dict* dictionary)
{
    return serializeDictionaryWithPadding(writer, 0, dictionary);
}

/** @see BencSerializer.h */
static int32_t parseDictionary(const struct Reader* reader,
                               const struct MemAllocator* allocator,
                               Dict* output)
{
    reader = reader;
    allocator = allocator;
    output = output;
    return notImplemented();
}

/**
 * Serialize a benc object into a json string with padding before each line.
 *
 * @param writer a Writer which to write the output to.
 * @param number of pad spaces to place before each line.
 * @param obj the object to serialize.
 * @return -2 if the type of object cannot be determined, otherwise
 *            whatever is returned by the Writer.
 */
static int32_t serializeGenericWithPadding(const struct Writer* writer,
                                           size_t padSpaceCount,
                                           const bobj_t* obj)
{
    switch (obj->type)
    {
        case BENC_BSTR:
            return serializeString(writer, obj->as.bstr);
        case BENC_DICT:
            return serializeDictionaryWithPadding(writer, padSpaceCount, obj->as.dictionary);
        case BENC_LIST:
            return serializeListWithPadding(writer, padSpaceCount, obj->as.list);
        case BENC_INT:
            return serializeInteger(writer, obj->as.int_);
        default:
            return -2;
    }
}

static const struct BencSerializer SERIALIZER =
{
    .serializeString = serializeString,
    .parseString = parseString,
    .serializeInteger = serializeInteger,
    .parseInteger = parseInteger,
    .serializeList = serializeList,
    .parseList = parseList,
    .serializeDictionary = serializeDictionary,
    .parseDictionary = parseDictionary
};

const struct BencSerializer* benc_getJsonBencSerializer()
{
    return &SERIALIZER;
}

#undef PAD
