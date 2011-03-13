#include "bencode.h"

/** @see bencode.h */
int benc_bobj_print(const struct Writer* writer,
                    size_t padSpaceCount,
                    const bobj_t* obj)
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
int bobj_print(const struct Writer* writer,
               bobj_t* obj)
{
    return benc_bobj_print(writer, 0, obj);
}

/** @see benc.h */
int bobj_serialize(const struct Writer* writer,
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
int bobj_parse(const struct Reader* reader,
               const struct MemAllocator* allocator,
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
