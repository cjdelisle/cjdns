/* vim: set expandtab ts=4 sw=4: */
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
/* for parseint64_t */
#include <limits.h>

#include "util/Bits.h"
#include "memory/Allocator.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "benc/Object.h"
#include "benc/serialization/BencSerializer.h"
#include "util/Errno.h"
#define string_strlen
#include "util/platform/libc/string.h"

static int32_t parseGeneric(const struct Reader* reader,
                            const struct Allocator* allocator,
                            Object** output);
static int32_t serializeGeneric(const struct Writer* writer,
                                const Object* obj);

/**
 * Helper function for writing an integer into a writer in base 10 format.
 *
 * @param writer the place to write the integer to.
 * @param integer the number to write.
 */
static int32_t writeint64_t(const struct Writer* writer,
                            int64_t integer)
{
    char buffer[32];
    Bits_memset(buffer, 0, 32);

    sprintf(buffer, "%" PRId64, integer);

    return writer->write(buffer, strlen(buffer), writer);
}

/** @see BencSerializer.h */
static int32_t serializeString(const struct Writer* writer,
                               const String* string)
{
    writeint64_t(writer, string->len);
    writer->write(":", 1, writer);
    return writer->write(string->bytes, string->len, writer);
}

/** @see BencSerializer.h */
static int32_t parseString(const struct Reader* reader,
                           const struct Allocator* allocator,
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

    char* bytes = allocator->malloc(length + 1, allocator);
    String* string = allocator->malloc(sizeof(String), allocator);

    /* Put a null terminator after the end so it can be treated as a normal string. */
    bytes[length] = '\0';

    if (reader->read(bytes, length, reader) != 0) {
        return OUT_OF_CONTENT_TO_READ;
    }
    string->bytes = bytes;
    string->len = length;

    *output = string;

    return 0;

    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
    #undef NUMBER_MAXLENGTH
}

/** @see BencSerializer.h */
static int32_t serializeint64_t(const struct Writer* writer,
                                const int64_t integer)
{
    writer->write("i", 1, writer);
    writeint64_t(writer, integer);
    return writer->write("e", 1, writer);
}

/** @see BencSerializer.h */
static int32_t parseint64_t(const struct Reader* reader,
                            int64_t* output)
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

    /* buffer + 1, skip the 'i' */
    int64_t out = strtol(buffer + 1, NULL, 10);

    /* Failed parse causes 0 to be set. */
    if (out == 0 && buffer[1] != '0' && (buffer[1] != '-' || buffer[2] != '0')) {
        return UNPARSABLE;
    }
    if ((out == LONG_MAX || out == LONG_MIN) && Errno_get() == Errno_ERANGE) {
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
    if (list) {
        const struct List_Item* entry = *list;
        while (ret == 0 && entry != NULL) {
            ret = serializeGeneric(writer, entry->elem);
            entry = entry->next;
        }
    }
    if (ret == 0) {
        ret = writer->write("e", 1, writer);
    }
    return ret;
}

/** @see BencSerializer.h */
static int32_t parseList(const struct Reader* reader,
                         const struct Allocator* allocator,
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

    Object* element;
    struct List_Item* thisEntry = NULL;
    struct List_Item** lastEntryPointer = output;
    int ret;

    if (reader->read(&nextChar, 0, reader) != 0) {
        return OUT_OF_CONTENT_TO_READ;
    }
    *lastEntryPointer = NULL;

    while (nextChar != 'e') {
        ret = parseGeneric(reader, allocator, &element);
        if (ret != 0) {
            return ret;
        }
        thisEntry = allocator->malloc(sizeof(struct List_Item), allocator);
        thisEntry->elem = element;

        /* Read backwards so that the list reads out forward. */
        *lastEntryPointer = thisEntry;
        lastEntryPointer = &(thisEntry->next);

        if (reader->read(&nextChar, 0, reader) != 0) {
            return OUT_OF_CONTENT_TO_READ;
        }
    }

    if (thisEntry) {
        thisEntry->next = NULL;
    }

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
    const struct Dict_Entry* entry = *dictionary;
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
                               const struct Allocator* allocator,
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
    Object* value;
    struct Dict_Entry* entryPointer;
    struct Dict_Entry* lastEntryPointer = NULL;
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
        entryPointer = allocator->malloc(sizeof(struct Dict_Entry), allocator);

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
                            const struct Allocator* allocator,
                            Object** output)
{
    #define OUT_OF_CONTENT_TO_READ -2
    #define UNPARSABLE -3

    int ret;
    char firstChar;
    ret = reader->read(&firstChar, 0, reader);
    if (ret != 0) {
        return OUT_OF_CONTENT_TO_READ;
    }

    Object* out = allocator->malloc(sizeof(Object), allocator);

    if (firstChar <= '9' && firstChar >= '0') {
        /* It's a string! */
        String* string = NULL;
        ret = parseString(reader, allocator, &string);
        out->type = Object_STRING;
        out->as.string = string;
    } else {
        switch (firstChar) {
            case 'i':;
                /* int64_t. Int is special because it is not a pointer but a int64_t. */
                int64_t bint = 0;
                ret = parseint64_t(reader, &bint);
                out->type = Object_INTEGER;
                out->as.number = bint;
                break;
            case 'l':;
                /* List. */
                List* list = allocator->calloc(sizeof(List), 1, allocator);
                ret = parseList(reader, allocator, list);
                out->type = Object_LIST;
                out->as.list = list;
                break;
            case 'd':;
                /* Dictionary. */
                Dict* dict = allocator->calloc(sizeof(Dict), 1, allocator);
                ret = parseDictionary(reader, allocator, dict);
                out->type = Object_DICT;
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
                                const Object* obj)
{
    switch (obj->type)
    {
        case Object_STRING:
            return serializeString(writer, obj->as.string);
            break;
        case Object_DICT:
            return serializeDictionary(writer, obj->as.dictionary);
            break;
        case Object_LIST:
            return serializeList(writer, obj->as.list);
            break;
        case Object_INTEGER:
            return serializeint64_t(writer, obj->as.number);
            break;
        default:
            return -2;
    }
}

static const struct BencSerializer SERIALIZER =
{
    .serializeString = serializeString,
    .parseString = parseString,
    .serializeint64_t = serializeint64_t,
    .parseint64_t = parseint64_t,
    .serializeList = serializeList,
    .parseList = parseList,
    .serializeDictionary = serializeDictionary,
    .parseDictionary = parseDictionary
};

const struct BencSerializer* StandardBencSerializer_get()
{
    return &SERIALIZER;
}
