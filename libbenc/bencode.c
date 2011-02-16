#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "bencode.h"

void benc_log_exception(const char *file, int line, const char *func, const char *msg, ...) {
	va_list ap;
	va_start(ap, msg);
	fprintf(stderr, "benc: %s:%d:%s: exception: ", file, line, func);
	vfprintf(stderr, msg, ap);
	va_end(ap);
}

void benc_log_syscall(const char *file, int line, const char *func, const char *syscall_name, const char *msg, ...) {
	va_list ap;
	va_start(ap, msg);
	fprintf(stderr, "benc: %s:%d:%s: %s() failed (status %d): %s. ", file, line, func, syscall_name, errno, strerror(errno));
	vfprintf(stderr, msg, ap);
	va_end(ap);
}

bobj_t * bobj_new(enum benc_data_type type)
{
    bobj_t *o = (bobj_t *)malloc(sizeof(bobj_t));
    o->type = type;
    return o;
}

void bobj_free(bobj_t *o)
{
    switch (o->type)
    {
    case BENC_INT:
        break;
    case BENC_BSTR:
        benc_bstr_free(o->as.bstr);
        break;
    case BENC_LIST:
        benc_list_free(o->as.list);
        break;
    case BENC_DICT:
        benc_dict_free(o->as.dict);
        break;
    default:
        BENC_LOG_EXCEPTION("don't know how to free this object type, may leak");
        break;
    }
    free(o);
}

size_t bobj_repsize(bobj_t *o)
{
    switch (o->type)
    {
    case BENC_INT:
        return benc_int_repsize(o->as.int_);
    case BENC_BSTR:
        return benc_bstr_repsize(o->as.bstr);
    case BENC_LIST:
        return benc_list_repsize(o->as.list);
    case BENC_DICT:
        return benc_dict_repsize(o->as.dict);
    default:
        BENC_LOG_EXCEPTION("don't know how to get representation size of this object type");
        return 0;
    }
}

void bobj_encode(bbuf_t *b, bobj_t *o)
{
    switch (o->type)
    {
    case BENC_INT:
        benc_int_encode(b, o->as.int_);
        break;
    case BENC_BSTR:
        benc_bstr_encode(b, o->as.bstr);
        break;
    case BENC_LIST:
        benc_list_encode(b, o->as.list);
        break;
    case BENC_DICT:
        benc_dict_encode(b, o->as.dict);
        break;
    default:
        BENC_LOG_EXCEPTION("don't know how to encode this object type");
        break;
    }
}

bbuf_t * bbuf_new(size_t len, char *base)
{
    if (NULL == base && len > 0)
    {
        base = (char *)malloc(len * sizeof(char));
    }
    if (NULL == base)
    {
        BENC_LOG_EXCEPTION("couldn't allocate %zu-character buffer", len);
        return NULL;
    }
    bbuf_t *b = (bbuf_t *)malloc(sizeof(bbuf_t));
    b->base = base;
    b->len = len;
    b->ptr = base;
    return b;
}

void bbuf_free(bbuf_t *b)
{
    free(b->base);
    free(b);
}

bool bbuf_inc_ptr(bbuf_t *b)
{
    ++(b->ptr);
    if (b->ptr > b->base + b->len)
    {
        BENC_LOG_EXCEPTION("ran past end of buffer");
        return false;
    }
    return true;
}

/*
bbuf_t * benc_mem(bobj_t *o)
{
    bbuf_t *b = bbuf_new(bobj_repsize(o), NULL);
    bobj_encode(b, o);
}
*/

/** @see bencode.h */
int benc_bobj_print(struct Writer* writer,
                    size_t padSpaceCount,
                    bobj_t* obj)
{
    switch (obj->type)
    {
        case BENC_BSTR:
            return benc_bstr_print(writer, obj->as.bstr);
        case BENC_DICT:
            return benc_dict_print(writer, padSpaceCount, obj->as.dict);
        case BENC_LIST:
            return benc_list_print(writer, padSpaceCount, obj->as.list);
        case BENC_INT:
            return benc_int_print(writer, obj->as.int_);
        default:
            return -2;
    }
}

/** @see benc.h */
int bobj_print(struct Writer* writer,
               bobj_t* obj)
{
    return benc_bobj_print(writer, 0, obj);
}

/** @see benc.h */
int bobj_serialize(struct Writer* writer,
                   bobj_t* obj)
{
    switch (obj->type)
    {
        case BENC_BSTR:
            return benc_bstr_serialize(writer, obj->as.bstr);
        case BENC_DICT:
            return benc_dict_serialize(writer, obj->as.dict);
        case BENC_LIST:
            return benc_list_serialize(writer, obj->as.list);
        case BENC_INT:
            return benc_int_serialize(writer, obj->as.int_);
        default:
            return -2;
    }
}

/** @see benc.h */
int bobj_parse(struct Reader* reader,
               struct MemAllocator* allocator,
               bobj_t** objPointer)
{
    #define OUT_OF_SPACE_TO_WRITE -1
    #define OUT_OF_CONTENT_TO_READ -2
    #define UNPARSABLE -3

    int ret;
    char firstChar;
    ret = reader->read(&firstChar, 0, reader);
    if (ret != 0) {
        return OUT_OF_CONTENT_TO_READ;
    }

    bobj_t* out = allocator->malloc(sizeof(bobj_t), allocator);
    if (out == NULL) {
        return OUT_OF_SPACE_TO_WRITE;
    }

    if (firstChar <= '9' && firstChar >= '0') {
        /* It's a string! */
        benc_bstr_t* string;
        ret = benc_bstr_parse(reader, allocator, &string);
        out->type = BENC_BSTR;
        out->as.bstr = string;
    } else {
        switch (firstChar) {
            case 'i':
                /* Integer. Int is special because it is not a pointer but a int64_t. */
                ; benc_int_t bint;
                ret = benc_int_parse(reader, &bint);
                out->type = BENC_INT;
                out->as.int_ = bint;
                break;
            case 'l':
                /* List. */
                ; benc_list_entry_t* list;
                ret = benc_list_parse(reader, allocator, &list);
                out->type = BENC_LIST;
                out->as.list = list;
                break;
            case 'd':
                /* Dictionary. */
                ; benc_dict_entry_t* dict;
                ret = benc_dict_parse(reader, allocator, &dict);
                out->type = BENC_DICT;
                out->as.dict = dict;
                break;
            default:
                return UNPARSABLE;
        }
    }

    if (ret != 0) {
        /* Something went wrong while parsing. */
        return ret;
    }

    *objPointer = out;
    return 0;

    #undef OUT_OF_SPACE_TO_WRITE
    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
}

bobj_t * bdec_mem(bbuf_t *b)
{
    bool success = false;
    bobj_t *o = NULL;
    switch (*(b->ptr)) {
    case 'i':
        o = bobj_new(BENC_INT);
        success = benc_int_decode(b, &(o->as.int_));
        break;
    case 'l':
        o = bobj_new(BENC_LIST);
        success = benc_list_decode(b, &(o->as.list));
        break;
    case 'd':
        o = bobj_new(BENC_DICT);
        success = benc_dict_decode(b, &(o->as.dict));
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
        o = bobj_new(BENC_BSTR);
        success = benc_bstr_decode(b, &(o->as.bstr));
        break;
    default:
        BENC_LOG_EXCEPTION("unexpected character: \\x%02x", *(b->ptr));
        return NULL;
    }
    
    if (success)
    {
        return o;
    }
    else
    {
        free(o);
        return NULL;
    }
}

/*
bool benc_file(bobj_t *o, char *file_path)
{
    bool success = false;
    bbuf_t b;
    b.len = bobj_repsize(o);
    
    int fd = open(file_path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (-1 == fd)
    {
        BENC_LOG_SYSCALL_ERROR("open", "Can't open output file: %s", file_path);
        goto done;
    }
    if (-1 == ftruncate(fd, (off_t)b.len))
    {
        BENC_LOG_SYSCALL_ERROR("ftruncate", "Can't resize output file: %s", file_path);
        goto close;
    }
    b.base = mmap(NULL, b.len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (NULL == b.base)
    {
        BENC_LOG_SYSCALL_ERROR("mmap", "Can't map output file: %s", file_path);
        goto close;
    }
    b.ptr = b.base;
    
    bobj_encode(&b, o);
    success = true;

unmap:
    if (-1 == munmap(b.base, b.len))
    {
        BENC_LOG_SYSCALL_ERROR("munmap", "Can't unmap output file: %s", file_path);
    }
close:
    if (-1 == close(fd))
    {
        BENC_LOG_SYSCALL_ERROR("close", "Can't close output file: %s", file_path);
    }   
done:
    return success;
}

bobj_t * bdec_file(char *file_path)
{
    bbuf_t b;
    struct stat sb;
    bobj_t *o = NULL;
    
    int fd = open(file_path, O_RDONLY);
    if (-1 == fd)
    {
        BENC_LOG_SYSCALL_ERROR("open", "Can't open input file: %s", file_path);
        goto done;
    }
    if (-1 == fstat(fd, &sb))
    {
        BENC_LOG_SYSCALL_ERROR("fstat", "Can't stat input file: %s", file_path);
        goto close;
    }
    if (!S_ISREG(sb.st_mode))
    {
        BENC_LOG_EXCEPTION("Not a regular file: %s", file_path);
        goto close;
    }
    b.len = sb.st_size;
    if (0 == b.len)
    {
        BENC_LOG_EXCEPTION("File is empty: %s", file_path);
        goto close;
    }
    b.base = mmap(NULL, b.len, PROT_READ, MAP_SHARED, fd, 0);
    if (NULL == b.base)
    {
        BENC_LOG_SYSCALL_ERROR("mmap", "Can't map input file: %s", file_path);
        goto close;
    }
    b.ptr = b.base;
    
    o = bdec_mem(&b);
    if (NULL == o)
    {
        BENC_LOG_EXCEPTION("Encoding failed.");
    }

unmap:    
    if (-1 == munmap(b.base, b.len))
    {
        BENC_LOG_SYSCALL_ERROR("munmap", "Can't unmap input file: %s", file_path);
    }
close:
    if (-1 == close(fd))
    {
        BENC_LOG_SYSCALL_ERROR("close", "Can't close input file: %s", file_path);
    }   
done:
    return o;
}
*/
