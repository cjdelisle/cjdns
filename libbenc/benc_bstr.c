#include <string.h>
#include <stdio.h>

#include "bencode.h"

/** @see benc.h */
String* benc_newString(const char* bytes, const struct MemAllocator* allocator)
{
    return benc_newBinaryString(bytes, strlen(bytes), allocator);
}

/** @see benc.h */
String* benc_newBinaryString(const char* bytes, size_t length, const struct MemAllocator* allocator)
{
    char* copy = allocator->malloc(length, allocator);
    memcpy(copy, bytes, length);
    benc_bstr_t* string = allocator->malloc(sizeof(benc_bstr_t), allocator);
    string->len = length;
    string->bytes = copy;
    return string;
}

/** @see bencode.h */
int benc_bstr_print(const struct Writer* writer,
                    const benc_bstr_t* string)
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

/** @see bencode.h */
int benc_bstr_serialize(const struct Writer* writer,
                        const benc_bstr_t* string)
{
    benc_int_print(writer, string->len);
    writer->write(":", 1, writer);
    return writer->write(string->bytes, string->len, writer);
}

/** @see bencode.h */
int benc_bstr_parse(const struct Reader* reader,
                    const struct MemAllocator* allocator,
                    benc_bstr_t** stringPointer)
{
    #define OUT_OF_SPACE_TO_WRITE -1
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
    benc_bstr_t* bstr = allocator->malloc(sizeof(benc_bstr_t), allocator);
    if (string == NULL || bstr == NULL) {
        return OUT_OF_SPACE_TO_WRITE;
    }
    /* Put a null terminator after the end so it can be treated as a normal string. */
    string[length] = '\0';

    if (reader->read(string, length, reader) != 0) {
        return OUT_OF_CONTENT_TO_READ;
    }
    bstr->bytes = string;
    bstr->len = length;

    /* Set the pointer to the string struct. */
    *stringPointer = bstr;

    return 0;

    #undef OUT_OF_SPACE_TO_WRITE
    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
    #undef NUMBER_MAXLENGTH
}

int benc_bstr_compare(const benc_bstr_t* a, const benc_bstr_t* b)
{
    if (a == NULL || b == NULL) {
        return (a == NULL) - (b == NULL);
    }
    size_t i;
    int d;
    for (i = 0; i < (a->len < b->len ? a->len : b->len); i++)
    {
        d = a->bytes[i] - b->bytes[i];
        if (0 != d) return d;
    }
    return a->len - b->len;
}

int32_t benc_stringEquals(const benc_bstr_t* a, const benc_bstr_t* b)
{
    return (a == NULL) ? ((a == NULL) == (b == NULL)) :
        a->len == b->len && (memcmp(a->bytes, b->bytes, a->len) == 0);
}
