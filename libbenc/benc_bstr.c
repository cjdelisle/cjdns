#include "bencode.h"

size_t pseudo_log10_size_t(size_t x);

benc_bstr_t * benc_bstr_new(size_t len, char *bytes)
{
    if (NULL == bytes && len > 0)
    {
        bytes = (char *)malloc(len * sizeof(char));
    }
    if (NULL == bytes)
    {
        BENC_LOG_EXCEPTION("couldn't allocate %zu-byte bstr", len);
        return NULL;
    }
    benc_bstr_t *s = (benc_bstr_t *)malloc(sizeof(benc_bstr_t));
    s->len = len;
    s->bytes = bytes;
    return s;
}

void benc_bstr_free(benc_bstr_t *s)
{
    free(s->bytes);
    free(s);
}

size_t benc_bstr_repsize(benc_bstr_t *s)
{
    size_t repsize = 1;
    repsize += pseudo_log10_size_t(s->len);
    repsize += s->len;
    return repsize;
}

void benc_bstr_encode(bbuf_t *b, benc_bstr_t *s)
{
    size_t x = s->len;
    b->ptr += pseudo_log10_size_t(x);
    char *resume = b->ptr;
    do
    {
        *--(b->ptr) = '0' + (char)(x % 10);
        x /= 10;
    } while (x > 0);
    b->ptr = resume;
    *(b->ptr)++ = ':';
    memcpy(b->ptr, s->bytes, s->len);
    b->ptr += s->len;
}

/** @see bencode.h */
int benc_bstr_print(struct Writer* writer,
                    benc_bstr_t* string)
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
int benc_bstr_serialize(struct Writer* writer,
                        benc_bstr_t* string)
{
    benc_int_print(writer, string->len);
    writer->write(":", 1, writer);
    return writer->write(string->bytes, string->len, writer);
}

/** @see bencode.h */
int benc_bstr_parse(struct Reader* reader,
                    struct MemAllocator* allocator,
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

    char* string = allocator->malloc(length, allocator);
    benc_bstr_t* bstr = allocator->malloc(sizeof(benc_bstr_t), allocator);
    if (string == NULL || bstr == NULL) {
        return OUT_OF_SPACE_TO_WRITE;
    }

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

bool benc_bstr_decode(bbuf_t *b, benc_bstr_t **s_p)
{
    size_t len = 0;
    do
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
            len *= 10;
            len += (size_t)(*(b->ptr) - '0');
            break;
        case ':':
            goto found_separator;
        default:
            goto unexpected_character;
        }
    }
    while (bbuf_inc_ptr(b));
    /*
    // if loop exits normally, we ran out of buffer
    // and an exception would have been raised
    */
    return false;
    
unexpected_character:    
    BENC_LOG_EXCEPTION("unexpected character: \\x%02x", *(b->ptr));
    return false;
    
found_separator:
    bbuf_inc_ptr(b);
    if (b->ptr + len > b->base + b->len)
    {
        BENC_LOG_EXCEPTION("bstr data would run past end of buffer");
        return false;
    }
    *s_p = benc_bstr_new(len, NULL);
    memcpy((*s_p)->bytes, b->ptr, len);
    b->ptr += len;
    return true;
}

int benc_bstr_compare(benc_bstr_t *a, benc_bstr_t *b)
{
    size_t i;
    int d;
    for (i = 0; i < (a->len < b->len ? a->len : b->len); i++)
    {
        d = a->bytes[i] - b->bytes[i];
        if (0 != d) return d;
    }
    return a->len - b->len;
}

size_t pseudo_log10_size_t(size_t x)
{
    size_t pseudo_log = 0;
    do {
        ++pseudo_log;
        x /= 10;
    } while (x > 0);
    return pseudo_log;
}

bobj_t * bobj_bstr_new(size_t len, char *bytes)
{
    bobj_t *obj = bobj_new(BENC_BSTR);
    obj->as.bstr = benc_bstr_new(len, bytes);
    return obj;
}
