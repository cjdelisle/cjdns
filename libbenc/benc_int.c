#include <string.h>
#include <stdio.h>
/* For playing nice with strtol() */
#include <errno.h>
#include <limits.h>

#include "bencode.h"

/** @see bencode.h */
int benc_int_print(const struct Writer* writer,
                   benc_int_t integer)
{
    char buffer[32];
    memset(buffer, 0, 32);

    /* Need to handle 32 bit or 64 bit boxen. */
    sprintf(buffer, (sizeof(long int) == 8) ? "%ld" : "%lld", integer);

    return writer->write(buffer, strlen(buffer), writer);
}

/** @see bencode.h */
int benc_int_serialize(const struct Writer* writer,
                       benc_int_t integer)
{
    writer->write("i", 1, writer);
    benc_int_print(writer, integer);
    return writer->write("e", 1, writer);
}

/** @see bencode.h */
int benc_int_parse(const struct Reader* reader,
                   benc_int_t* intPointer)
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
    benc_int_t out = strtol(buffer + 1, NULL, 10);

    if (out == 0 && buffer[1] != '0' && buffer[1] != '-' && buffer[2] != '0') {
        /* Failed parse causes 0 to be set. */
        return UNPARSABLE;
    }
    if ((out == LONG_MAX || out == LONG_MIN) && errno == ERANGE) {
        /* errno (holds nose) */
        return UNPARSABLE;
    }

    *intPointer = out;
    return 0;

    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
}

/** @see benc.h */
bobj_t* benc_newInteger(int64_t number, const struct MemAllocator* allocator)
{
    bobj_t* out = allocator->malloc(sizeof(bobj_t), allocator);
    out->type = BENC_INT;
    out->as.int_ = number;
    return out;
}
