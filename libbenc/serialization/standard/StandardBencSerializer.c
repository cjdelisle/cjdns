/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <string.h>
/* for parseInteger */
#include <errno.h>
#include <limits.h>

#include "memory/MemAllocator.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "libbenc/benc.h"
#include "libbenc/serialization/BencSerializer.h"

static int32_t parseGeneric(const struct Reader* reader,
                            const struct MemAllocator* allocator,
                            bobj_t** output);
static int32_t serializeGeneric(const struct Writer* writer,
                                const bobj_t* obj);

/**
 * Helper function for writing an integer into a writer in base 10 format.
 *
 * @param writer the place to write the integer to.
 * @param integer the number to write.
 */
static int32_t writeInteger(const struct Writer* writer,
                            Integer integer)
{
    char buffer[32];
    memset(buffer, 0, 32);

    /* Need to handle 32 bit or 64 bit boxen. */
    sprintf(buffer, (sizeof(long int) == 8) ? "%ld" : "%lld", integer);

    return writer->write(buffer, strlen(buffer), writer);
}

/** @see BencSerializer.h */
static int32_t serializeString(const struct Writer* writer,
                               const String* string)
{
    writeInteger(writer, string->len);
    writer->write(":", 1, writer);
    return writer->write(string->bytes, string->len, writer);
}

/** @see BencSerializer.h */
static int32_t parseString(const struct Reader* reader,
                           const struct MemAllocator* allocator,
                           String** output)
{
    #define OUT_OF_CONTENT_TO_READ -2
    #define UNPARSABLE -3

    /* Strings longer than 1*10^21-1 represent numbers definitly larger than uint64. */
    #define NUMBER_MAXLENGTH 21
    char number[32];
    char nextChar;
    int ret;

    /* Parse the size of the string. */
    size_t i = 0;
    for (i = 0; ; i++) {
        ret = reader->read(&nextChar, 1, reader);
        if (ret != 0) {
            return OUT_OF_CONTENT_TO_READ;
        }
        if (nextChar == ':') {
            /* Found the separator. */
            break;
        }
        if (nextChar < '0' || nextChar > '9') {
            /* Invalid character. */
            return UNPARSABLE;
        }
        if (i >= NUMBER_MAXLENGTH) {
            /* Massive huge number. */
            return UNPARSABLE;
        }
        number[i] = nextChar;
    }
    number[i] = '\0';
    size_t length = strtoul(number, NULL, 10);

    char* string = allocator->malloc(length + 1, allocator);
    String* bstr = allocator->malloc(sizeof(String), allocator);

    /* Put a null terminator after the end so it can be treated as a normal string. */
    string[length] = '\0';

    if (reader->read(string, length, reader) != 0) {
        return OUT_OF_CONTENT_TO_READ;
    }
    bstr->bytes = string;
    bstr->len = length;

    *output = bstr;

    return 0;

    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
    #undef NUMBER_MAXLENGTH
}

/** @see BencSerializer.h */
static int32_t serializeInteger(const struct Writer* writer,
                                const Integer integer)
{
    writer->write("i", 1, writer);
    writeInteger(writer, integer);
    return writer->write("e", 1, writer);
}

/** @see BencSerializer.h */
static int32_t parseInteger(const struct Reader* reader,
                            Integer* output)
{
    #define OUT_OF_CONTENT_TO_READ -2
    #define UNPARSABLE -3
    char buffer[32];
    int i;
    for (i = 0; ; i++) {
        if (reader->read(buffer + i, 1, reader) != 0) {
            return OUT_OF_CONTENT_TO_READ;
        }
        if (i == 0) {
            if (buffer[i] != 'i') {
                /* Not an int. */
                return UNPARSABLE;
            } else {
                continue;
            }
        }
        if (buffer[i] == 'e') {
            break;
        }
        if (i == 1 && buffer[i] == '-') {
            /* It's just a negative number, no need to fail it. */
            continue;
        }
        if (buffer[i] < '0' || buffer[i] > '9') {
            return UNPARSABLE;
        }
        if (i > 21) {
            /* Larger than the max possible int64. */
            return UNPARSABLE;
        }
    }
    errno = 0;

    /* buffer + 1, skip the 'i' */
    benc_int_t out = strtol(buffer + 1, NULL, 10);

    /* Failed parse causes 0 to be set. */
    if (out == 0 && buffer[1] != '0' && (buffer[1] != '-' || buffer[2] != '0')) {
        return UNPARSABLE;
    }
    if ((out == LONG_MAX || out == LONG_MIN) && errno == ERANGE) {
        /* errno (holds nose) */
        return UNPARSABLE;
    }

    *output = out;
    return 0;

    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
}

/** @see BencSerializer.h */
static int32_t serializeList(const struct Writer* writer,
                             const List* list)
{
    int ret = writer->write("l", 1, writer);
    const benc_list_entry_t* entry = *list;
    while (ret == 0 && entry != NULL) {
        ret = serializeGeneric(writer, entry->elem);
        entry = entry->next;
    }
    if (ret == 0) {
        ret = writer->write("e", 1, writer);
    }
    return ret;
}

/** @see BencSerializer.h */
static int32_t parseList(const struct Reader* reader,
                         const struct MemAllocator* allocator,
                         List* output)
{
    #define OUT_OF_CONTENT_TO_READ -2
    #define UNPARSABLE -3

    char nextChar;
    if (reader->read(&nextChar, 1, reader) != 0) {
        return OUT_OF_CONTENT_TO_READ;
    }
    if (nextChar != 'l') {
        return UNPARSABLE;
    }

    bobj_t* element;
    benc_list_entry_t* thisEntry = NULL;
    benc_list_entry_t** lastEntryPointer = output;
    int ret;

    while (nextChar != 'e') {
        ret = parseGeneric(reader, allocator, &element);
        if (ret != 0) {
            return ret;
        }
        thisEntry = allocator->malloc(sizeof(benc_list_entry_t), allocator);
        thisEntry->elem = element;

        /* Read backwards so that the list reads out forward. */
        *lastEntryPointer = thisEntry;
        lastEntryPointer = &(thisEntry->next);

        if (reader->read(&nextChar, 0, reader) != 0) {
            return OUT_OF_CONTENT_TO_READ;
        }
    }

    thisEntry->next = NULL;

    /* move the pointer to after the 'e' at the end of the list. */
    reader->skip(1, reader);

    return 0;

    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
}

/** @see BencSerializer.h */
static int32_t serializeDictionary(const struct Writer* writer,
                                   const Dict* dictionary)
{
    const benc_dict_entry_t* entry = *dictionary;
    writer->write("d", 1, writer);
    while (entry != NULL) {
        serializeString(writer, entry->key);
        serializeGeneric(writer, entry->val);
        entry = entry->next;
    }
    return writer->write("e", 1, writer);
}

/** @see BencSerializer.h */
static int32_t parseDictionary(const struct Reader* reader,
                               const struct MemAllocator* allocator,
                               Dict* output)
{
    #define OUT_OF_CONTENT_TO_READ -2
    #define UNPARSABLE -3

    char nextChar;
    if (reader->read(&nextChar, 1, reader) < 0) {
        return OUT_OF_CONTENT_TO_READ;
    }
    if (nextChar != 'd') {
        /* Not a dictionary. */
        return UNPARSABLE;
    }

    String* key;
    bobj_t* value;
    benc_dict_entry_t* entryPointer;
    benc_dict_entry_t* lastEntryPointer = NULL;
    int ret;

    for (;;) {
        /* Peek at the next char. */
        if (reader->read(&nextChar, 0, reader) < 0) {
            /* Ran over read buffer. */
            return OUT_OF_CONTENT_TO_READ;
        }
        if (nextChar == 'e') {
            /* Got to the end. */
            break;
        }

        /* Get key and value. */
        ret = parseString(reader, allocator, &key);
        if (ret != 0) {
            return ret;
        }
        ret = parseGeneric(reader, allocator, &value);
        if (ret != 0) {
            return ret;
        }

        /* Allocate the entry. */
        entryPointer = allocator->malloc(sizeof(benc_dict_entry_t), allocator);

        entryPointer->next = lastEntryPointer;
        entryPointer->key = key;
        entryPointer->val = value;
        lastEntryPointer = entryPointer;
    }

    /* We got an 'e', leave the pointer on the next char after it. */
    reader->skip(1, reader);

    *output = lastEntryPointer;

    return 0;

    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
}

/**
 * Parse an unknown data type.
 * This is not exposed to the world because it is expected that one would
 * know what type they are parsing to begin with. This is used by parseDictionary
 * and parseList to grab pieces of data which are of unknown type and parse them.
 *
 * @param reader the reader to get the stream of data from.
 * @param allocator the means of storing the parsed data.
 * @param output a pointer which will be pointed to the output.
 */
static int32_t parseGeneric(const struct Reader* reader,
                            const struct MemAllocator* allocator,
                            bobj_t** output)
{
    #define OUT_OF_CONTENT_TO_READ -2
    #define UNPARSABLE -3

    int ret;
    char firstChar;
    ret = reader->read(&firstChar, 0, reader);
    if (ret != 0) {
        return OUT_OF_CONTENT_TO_READ;
    }

    bobj_t* out = allocator->malloc(sizeof(bobj_t), allocator);

    if (firstChar <= '9' && firstChar >= '0') {
        /* It's a string! */
        String* string = NULL;
        ret = parseString(reader, allocator, &string);
        out->type = BENC_BSTR;
        out->as.bstr = string;
    } else {
        switch (firstChar) {
            case 'i':;
                /* Integer. Int is special because it is not a pointer but a int64_t. */
                benc_int_t bint;
                ret = parseInteger(reader, &bint);
                out->type = BENC_INT;
                out->as.int_ = bint;
                break;
            case 'l':;
                /* List. */
                List* list = allocator->calloc(sizeof(List), 1, allocator);
                ret = parseList(reader, allocator, list);
                out->type = BENC_LIST;
                out->as.list = list;
                break;
            case 'd':;
                /* Dictionary. */
                Dict* dict = allocator->calloc(sizeof(Dict), 1, allocator);
                ret = parseDictionary(reader, allocator, dict);
                out->type = BENC_DICT;
                out->as.dictionary = dict;
                break;
            default:
                return UNPARSABLE;
        }
    }

    if (ret != 0) {
        /* Something went wrong while parsing. */
        return ret;
    }

    *output = out;
    return 0;

    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
}

/**
 * Serialize a benc object into a bencoded string.
 * This is not exposed to the world because it is expected that one would
 * know what type they are serializing to begin with.
 *
 * @param writer a Writer which to write the output to.
 * @param obj the object to serialize.
 * @return -2 if the type of object cannot be determined, otherwise
 *            whatever is returned by the Writer.
 */
static int32_t serializeGeneric(const struct Writer* writer,
                                const bobj_t* obj)
{
    switch (obj->type)
    {
        case BENC_BSTR:
            return serializeString(writer, obj->as.bstr);
            break;
        case BENC_DICT:
            return serializeDictionary(writer, obj->as.dictionary);
            break;
        case BENC_LIST:
            return serializeList(writer, obj->as.list);
            break;
        case BENC_INT:
            return serializeInteger(writer, obj->as.int_);
            break;
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

const struct BencSerializer* benc_getStandardBencSerializer()
{
    return &SERIALIZER;
}
