#include "bencode.h"
#include <errno.h>
#include <limits.h>

size_t pseudo_log10_benc_int_t(benc_int_t i);

size_t benc_int_repsize(benc_int_t i)
{
    size_t repsize = 2;
    if (i < 0)
    {
        ++repsize;
        i = -i;
    }
    repsize += pseudo_log10_benc_int_t(i);
    return repsize;
}

void benc_int_encode(bbuf_t *b, benc_int_t i)
{
    *(b->ptr)++ = 'i';
    if (i < 0)
    {
        i = -i;
        *(b->ptr)++ = '-';
    }
    b->ptr += pseudo_log10_benc_int_t(i);
    char *resume = b->ptr;
    do
    {
        *--(b->ptr) = '0' + (char)(i % 10);
        i /= 10;
    } while (i > 0);
    b->ptr = resume;
    *(b->ptr)++ = 'e';
}

/** @see bencode.h */
int benc_int_print(struct Writer* writer,
                           benc_int_t integer)
{
    char buffer[32];
    memset(buffer, 0, 32);

    /* int64_t max value is 19 digits long, this should be safe. */
    sprintf(buffer, "%ld", integer);

    return writer->write(buffer, strlen(buffer), writer);
}

/** @see bencode.h */
int benc_int_serialize(struct Writer* writer,
                       benc_int_t integer)
{
    writer->write("i", 1, writer);
    benc_int_print(writer, integer);
    return writer->write("e", 1, writer);
}

/** @see bencode.h */
int benc_int_parse(struct Reader* reader,
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

bool benc_int_decode(bbuf_t *b, benc_int_t *i_p)
{
    bbuf_inc_ptr(b);
    *i_p = 0;
    benc_int_t sign = 1;
    switch (*(b->ptr))
    {
    case '-':
        sign = -1;
        break;
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
        *i_p = (benc_int_t)(*(b->ptr) - '0');
        break;
    default:
        goto unexpected_character;
    }
    while (bbuf_inc_ptr(b))
    {
        switch (*(b->ptr))
        {
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
            *i_p *= 10;
            *i_p += (benc_int_t)(*(b->ptr) - '0');
            break;
        case 'e':
            goto found_terminator;
        default:
            goto unexpected_character;
        }
    }
    /*
    // if loop exits normally, we ran out of buffer
    // and an exception would have been raised
    */
    return false;
    
unexpected_character:    
    BENC_LOG_EXCEPTION("unexpected character: \\x%02x", *(b->ptr));
    return false;
    
found_terminator:
    bbuf_inc_ptr(b);
    *i_p *= sign;
    return true;
}

size_t pseudo_log10_benc_int_t(benc_int_t i)
{
    size_t pseudo_log = 0;
    do {
        ++pseudo_log;
        i /= 10;
    } while (i > 0);
    return pseudo_log;
}

bobj_t * bobj_int_new(benc_int_t i)
{
    bobj_t *obj = bobj_new(BENC_INT);
    obj->as.int_ = i;
    return obj;
}
