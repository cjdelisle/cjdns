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
#include "memory/Allocator.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/String.h"
#include "benc/serialization/BencSerializer.h"
#include "util/Bits.h"
#include "util/Hex.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

static int32_t parseGeneric(struct Reader* reader,
                            struct Allocator* allocator,
                            Object** output);
static int32_t serializeGenericWithPadding(struct Writer* writer,
                                           size_t padSpaceCount,
                                           const Object* obj);

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
        Writer_write(writer, thirtyTwoSpaces, 32);                      \
        padCounter += 32;                                               \
    }                                                                   \
    Writer_write(writer, thirtyTwoSpaces, padSpaces - padCounter)

static inline int outOfContent()
{
    return -2;
}
#define OUT_OF_CONTENT_TO_READ outOfContent()

static inline int unparsable()
{
    return -3;
}
#define UNPARSABLE unparsable()

/** @see BencSerializer.h */
static int32_t serializeString(struct Writer* writer,
                               const String* string)
{
    Writer_write(writer, "\"", 1);
    size_t i;
    uint8_t chr;
    char buffer[4];
    for (i = 0; i < string->len; i++) {
        chr = (uint8_t) string->bytes[i] & 0xFF;
        /* Nonprinting chars, \ and " are hex'd */
        if (chr < 126 && chr > 31 && chr != '\\' && chr != '"') {
            snprintf(buffer, 4, "%c", chr);
            Writer_write(writer, buffer, 1);
        } else {
            snprintf(buffer, 4, "\\x%.2X", chr);
            Writer_write(writer, buffer, 4);
        }
    }
    return Writer_write(writer, "\"", 1);
}

/**
 * Read until 1 char after the target character.
 */
static inline int readUntil(uint8_t target, struct Reader* reader)
{
    uint8_t nextChar;
    do {
        if (Reader_read(reader, (char*)&nextChar, 1)) {
            printf("Unexpected end of input while looking for '%c'\n",target);
            return OUT_OF_CONTENT_TO_READ;
        }
    } while (nextChar != target);
    return 0;
}

static inline int parseString(struct Reader* reader,
                              struct Allocator* allocator,
                              String** output)
{
    #define BUFF_SZ (1<<8)
    #define BUFF_MAX (1<<20)

    int curSize = BUFF_SZ;
    struct Allocator* localAllocator = Allocator_child(allocator);
    uint8_t* buffer = Allocator_malloc(localAllocator, curSize);
    if (readUntil('"', reader) || Reader_read(reader, buffer, 1)) {
        printf("Unterminated string\n");
        Allocator_free(localAllocator);
        return OUT_OF_CONTENT_TO_READ;
    }
    for (int i = 0; i < BUFF_MAX - 1; i++) {
        if (buffer[i] == '\\') {
            // \x01 (skip the x)
            Reader_skip(reader, 1);
            uint8_t hex[2];
            if (Reader_read(reader, (char*)hex, 2)) {
                printf("Unexpected end of input parsing escape sequence\n");
                Allocator_free(localAllocator);
                return OUT_OF_CONTENT_TO_READ;
            }
            int byte = Hex_decodeByte(hex[0], hex[1]);
            if (byte == -1) {
                printf("Invalid escape \"%c%c\" after \"%.*s\"\n",hex[0],hex[1],i+1,buffer);
                Allocator_free(localAllocator);
                return UNPARSABLE;
            }
            buffer[i] = (uint8_t) byte;
        } else if (buffer[i] == '"') {
            *output = String_newBinary((char*)buffer, i, allocator);
            Allocator_free(localAllocator);
            return 0;
        }
        if (i == curSize - 1) {
            curSize <<= 1;
            buffer = Allocator_realloc(localAllocator, buffer, curSize);
        }
        if (Reader_read(reader, buffer + i + 1, 1)) {
            if (i+1 <= 20) {
                printf("Unterminated string \"%.*s\"\n", i+1, buffer);
            } else {
                printf("Unterminated string starting with \"%.*s...\"\n", 20, buffer);
            }
            Allocator_free(localAllocator);
            return OUT_OF_CONTENT_TO_READ;
        }
    }

    printf("Maximum string length of %d bytes exceeded.\n",BUFF_SZ);
    Allocator_free(localAllocator);
    return UNPARSABLE;

    #undef BUFF_SZ
    #undef BUFF_MAX
}

/** @see BencSerializer.h */
static int32_t serializeint64_t(struct Writer* writer,
                                int64_t integer)
{
    char buffer[32];
    Bits_memset(buffer, 0, 32);

    snprintf(buffer, 32, "%" PRId64, integer);

    return Writer_write(writer, buffer, CString_strlen(buffer));
}

/** @see BencSerializer.h */
static int32_t parseint64_t(struct Reader* reader,
                            int64_t* output)
{
    uint8_t buffer[32];

    for (int i = 0; i < 21; i++) {
        int32_t status = Reader_read(reader, buffer + i, 0);
        if (i == 0 && buffer[i] == '-' && status == 0) {
            // It's just a negative number, no need to fail it.
            continue;
        }
        if (buffer[i] < '0' || buffer[i] > '9' || status != 0 /* end of input */) {
            buffer[i] = '\0';
            int64_t out = strtol((char*)buffer, NULL, 10);
            // Failed parse causes 0 to be set.
            if (out == 0 && buffer[0] != '0' && (buffer[0] != '-' || buffer[1] != '0')) {
                printf("Failed to parse \"%s\": not a number\n",buffer);
                return UNPARSABLE;
            }
            if ((out == INT64_MAX || out == INT64_MIN) && errno == ERANGE) {
                printf("Failed to parse \"%s\": number too large/small\n",buffer);
                return UNPARSABLE;
            }
            *output = out;
            return 0;
        }
        Reader_skip(reader, 1);
    }

    // Larger than the max possible int64.
    buffer[22] = '\0';
    printf("Failed to parse \"%s\": number too large\n",buffer);
    return UNPARSABLE;
}

/**
 * Serialize a bencoded list with padding at the beginning of each line.
 *
 * @param writer the place to write the output to.
 * @param padSpaceCount the number of spaces to place at the beginning of each line.
 * @param list the list to serialize
 */
static int32_t serializeListWithPadding(struct Writer* writer,
                                        const size_t padSpaceCount,
                                        const List* list)
{
    int padCounter;

    Writer_write(writer, "[\n", 2);

    const struct List_Item* entry = *list;
    while (entry != NULL) {
        PAD(padSpaceCount + 2, padCounter, writer);
        serializeGenericWithPadding(writer, padSpaceCount + 2, entry->elem);
        entry = entry->next;
        if (entry != NULL) {
            Writer_write(writer, ",\n", 2);
        }
    }

    Writer_write(writer, "\n", 1);
    PAD(padSpaceCount, padCounter, writer);
    return Writer_write(writer, "]", 1);
}

/** @see BencSerializer.h */
static int32_t serializeList(struct Writer* writer,
                             const List* list)
{
    return serializeListWithPadding(writer, 0, list);
}

/**
 * Parse a comment in with "slash splat" or double slash notation,
 * leave the reader on the first character after the last end of comment mark.
 */
static inline int parseComment(struct Reader* reader)
{
    char chars[2];
    int ret = Reader_read(reader, &chars, 2);
    if (ret) {
        printf("Warning: expected comment\n");
        return OUT_OF_CONTENT_TO_READ;
    }
    if (chars[0] != '/') {
        printf("Warning: expected a comment starting with '/', instead found '%c'\n",chars[0]);
        return UNPARSABLE;
    }
    switch (chars[1]) {
        case '*':;
            do {
                readUntil('*', reader);
            } while (!(ret = Reader_read(reader, &chars, 1)) && chars[0] != '/');
            if (ret) {
                printf("Unterminated multiline comment\n");
                return OUT_OF_CONTENT_TO_READ;
            }
            return 0;
        case '/':;
            return readUntil('\n', reader);
        default:
            printf("Warning: expected a comment starting with \"//\" or \"/*\", "
                   "instead found \"/%c\"\n",chars[1]);
            return UNPARSABLE;
    }
}

/** @see BencSerializer.h */
static int32_t parseList(struct Reader* reader,
                         struct Allocator* allocator,
                         List* output)
{
    char nextChar;
    readUntil('[', reader);

    Object* element;
    struct List_Item* thisEntry = NULL;
    struct List_Item** lastEntryPointer = output;
    int ret;

    for (;;) {
        for (;;) {
            if (Reader_read(reader, &nextChar, 0) != 0) {
                printf("Unterminated list\n");
                return OUT_OF_CONTENT_TO_READ;
            }
            if (nextChar == '/') {
                if ((ret = parseComment(reader)) != 0) {
                    return ret;
                }
                continue;
            }
            switch (nextChar) {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '[':
                case '{':
                case '"':
                    break;

                case ']':
                    Reader_skip(reader, 1);
                    return 0;

                default:
                    // FIXME(gerard): silently skipping anything we don't understand
                    // might not be the best idea
                    Reader_skip(reader, 1);
                    continue;
            }
            break;
        }

        if ((ret = parseGeneric(reader, allocator, &element)) != 0) {
            return ret;
        }
        thisEntry = Allocator_malloc(allocator, sizeof(struct List_Item));
        thisEntry->elem = element;
        thisEntry->next = NULL;

        // Read backwards so that the list reads out forward.
        *lastEntryPointer = thisEntry;
        lastEntryPointer = &(thisEntry->next);
    }
}

/**
 * Serialize a bencoded dictionary with padding before each line.
 *
 * @param writer the place to write the output to.
 * @param padSpaceCount the number of spaces to place at the beginning of each line.
 * @param dictionary the dictionary to serialize.
 */
static int32_t serializeDictionaryWithPadding(struct Writer* writer,
                                              size_t padSpaceCount,
                                              const Dict* dictionary)
{
    int padCounter = 0;
    Writer_write(writer, "{\n", 2);
    const struct Dict_Entry* entry = *dictionary;
    while (entry != NULL) {
        PAD(padSpaceCount + 2, padCounter, writer);
        serializeString(writer, entry->key);
        Writer_write(writer, " : ", 3);
        serializeGenericWithPadding(writer, padSpaceCount + 2, entry->val);
        entry = entry->next;
        if (entry != NULL) {
            Writer_write(writer, ",\n", 2);
        }
    }

    Writer_write(writer, "\n", 1);
    PAD(padSpaceCount, padCounter, writer);
    return Writer_write(writer, "}", 1);
}

/** @see BencSerializer.h */
static int32_t serializeDictionary(struct Writer* writer,
                                   const Dict* dictionary)
{
    return serializeDictionaryWithPadding(writer, 0, dictionary);
}

/** @see BencSerializer.h */
static int32_t parseDictionary(struct Reader* reader,
                               struct Allocator* allocator,
                               Dict* output)
{
    uint8_t nextChar;
    readUntil('{', reader);

    String* key;
    Object* value;
    struct Dict_Entry* entryPointer;
    struct Dict_Entry* lastEntryPointer = NULL;
    int ret = 0;

    for (;;) {
        while (!ret) {
            ret = Reader_read(reader, &nextChar, 0);
            switch (nextChar) {
                case '"':
                    break;

                case '}':
                    Reader_skip(reader, 1);
                    *output = lastEntryPointer;
                    return 0;

                case '/':
                    parseComment(reader);
                    continue;

                default:
                    Reader_skip(reader, 1);
                    continue;
            }
            break;
        }
        if (ret) {
            printf("Unterminated dictionary\n");
            return OUT_OF_CONTENT_TO_READ;
        }

        // Get key and value.
        if ((ret = parseString(reader, allocator, &key)) != 0) {
            return ret;
        }

        readUntil(':', reader);

        if ((ret = parseGeneric(reader, allocator, &value)) != 0) {
            return ret;
        }

        /* Allocate the entry. */
        entryPointer = Allocator_malloc(allocator, sizeof(struct Dict_Entry));

        entryPointer->next = lastEntryPointer;
        entryPointer->key = key;
        entryPointer->val = value;
        lastEntryPointer = entryPointer;
    }
}

static int32_t parseGeneric(struct Reader* reader,
                            struct Allocator* allocator,
                            Object** output)
{
    int ret = 0;
    char firstChar;

    for (;;) {
        ret = Reader_read(reader, &firstChar, 0);
        switch (firstChar) {
            case ' ':
            case '\r':
            case '\n':
            case '\t':
                Reader_skip(reader, 1);
                continue;

            case '/':;
                if ((ret = parseComment(reader)) != 0) {
                    return ret;
                }
                continue;

            default:
                break;
        }
        if (ret) {
            printf("Unexpected end of input\n");
            return OUT_OF_CONTENT_TO_READ;
        }
        break;
    }

    Object* out = Allocator_malloc(allocator, sizeof(Object));

    switch (firstChar) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':;
            // int64_t. Int is special because it is not a pointer but a int64_t.
            int64_t bint;
            if ((ret = parseint64_t(reader, &bint)) == UNPARSABLE) {
                break;
            }
            out->type = Object_INTEGER;
            out->as.number = bint;
            break;

        case '[':;
            // List.
            List* list = Allocator_calloc(allocator, sizeof(List), 1);
            ret = parseList(reader, allocator, list);
            out->type = Object_LIST;
            out->as.list = list;
            break;

        case '{':;
            // Dictionary
            Dict* dict = Allocator_calloc(allocator, sizeof(Dict), 1);
            ret = parseDictionary(reader, allocator, dict);
            out->type = Object_DICT;
            out->as.dictionary = dict;
            break;

        case '"':;
            // String
            String* string = NULL;
            ret = parseString(reader, allocator, &string);
            out->type = Object_STRING;
            out->as.string = string;
            break;

        default:
            printf("While looking for something to parse: "
                   "expected one of 0 1 2 3 4 5 6 7 8 9 [ { \", found '%c'\n", firstChar);
            return UNPARSABLE;
    }

    if (ret != 0) {
        // Something went wrong while parsing.
        return ret;
    }

    *output = out;
    return 0;
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
static int32_t serializeGenericWithPadding(struct Writer* writer,
                                           size_t padSpaceCount,
                                           const Object* obj)
{
    switch (obj->type)
    {
        case Object_STRING:
            return serializeString(writer, obj->as.string);
        case Object_DICT:
            return serializeDictionaryWithPadding(writer, padSpaceCount, obj->as.dictionary);
        case Object_LIST:
            return serializeListWithPadding(writer, padSpaceCount, obj->as.list);
        case Object_INTEGER:
            return serializeint64_t(writer, obj->as.number);
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

const struct BencSerializer* JsonBencSerializer_get()
{
    return &SERIALIZER;
}

#undef PAD
