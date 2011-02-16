#include "bencode.h"

benc_dict_entry_t * benc_dict_entry_new(benc_dict_entry_t *next, benc_bstr_t *k, bobj_t *v);
bobj_t * benc_dict_entry_free_return_val(benc_dict_entry_t *d);

void benc_dict_free(benc_dict_entry_t *head)
{
    benc_dict_entry_t *curr = head;
    benc_dict_entry_t *tmp;
    while (NULL != curr)
    {
        tmp = curr->next;
        bobj_t *val = benc_dict_entry_free_return_val(curr);
        bobj_free(val);
        curr = tmp;
    }
}

size_t benc_dict_repsize(benc_dict_entry_t *head)
{
    size_t repsize = 2;
    benc_dict_entry_t *curr = head;
    while (NULL != curr)
    {
        repsize += benc_bstr_repsize(curr->key);
        repsize += bobj_repsize(curr->val);
        curr = curr->next;
    }
    return repsize;
}

void benc_dict_encode(bbuf_t *b, benc_dict_entry_t *head)
{
    benc_dict_entry_t *curr = head;
    *(b->ptr)++ = 'd';
    while (NULL != curr)
    {
        benc_bstr_encode(b, curr->key);
        bobj_encode(b, curr->val);
        curr = curr->next;
    }
    *(b->ptr)++ = 'e';
}

bool benc_dict_decode(bbuf_t *b, benc_dict_entry_t **head_p)
{
    bbuf_inc_ptr(b);
    benc_dict_entry_t **prev_p, *curr;
    prev_p = head_p;
    while (*(b->ptr) != 'e')
    {
        benc_bstr_t *key = NULL;
        if (!benc_bstr_decode(b, &key))
        {
            BENC_LOG_EXCEPTION("couldn't read dict key");
            return false;
        }
        bobj_t *val = bdec_mem(b);
        if (NULL == val)
        {
            BENC_LOG_EXCEPTION("couldn't read dict value");
            return false;
        }
        curr = benc_dict_entry_new(NULL, key, val);
        *prev_p = curr;
        prev_p = &(curr->next);
    }
    bbuf_inc_ptr(b);
    return true;
}

/** @see bencode.h */
int benc_dict_print(struct Writer* writer,
                    size_t padSpaceCount,
                    benc_dict_entry_t* head)
{
    char* thirtyTwoSpaces = "                                ";
    int padCounter = 0;
    #define PAD(padSpaces)                                                  \
        while (32 < padSpaces + padCounter) {                               \
            writer->write(thirtyTwoSpaces, 32, writer);                     \
            padCounter += 32;                                               \
        }                                                                   \
        writer->write(thirtyTwoSpaces, padSpaces - padCounter, writer)

    writer->write("{\n", 2, writer);

    benc_dict_entry_t* entry = head;
    while (entry != NULL) {
        PAD(padSpaceCount + 2);
        benc_bstr_print(writer, entry->key);
        writer->write(" : ", 3, writer);        
        benc_bobj_print(writer, padSpaceCount + 2, entry->val);
        entry = entry->next;
        if (entry != NULL) {
            writer->write(",\n", 2, writer);
        }
    }

    writer->write("\n", 1, writer);
    PAD(padSpaceCount);
    return writer->write("}", 1, writer);

    #undef PAD
}

/** @see bencode.h */
int benc_dict_serialize(struct Writer* writer,
                        benc_dict_entry_t* head)
{
    benc_dict_entry_t* entry = head;
    writer->write("d", 1, writer);
    while (entry != NULL)
    {
        benc_bstr_serialize(writer, entry->key);
        bobj_serialize(writer, entry->val);
        entry = entry->next;
    }
    return writer->write("e", 1, writer);
}

/** @see bencode.h */
int benc_dict_parse(struct Reader* reader,
                    struct MemAllocator* allocator,
                    benc_dict_entry_t** headPointer)
{
    #define OUT_OF_SPACE_TO_WRITE -1
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

    benc_bstr_t* key;
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
        ret = benc_bstr_parse(reader, allocator, &key);
        if (ret != 0) {
            return ret;
        }
        ret = bobj_parse(reader, allocator, &value);
        if (ret != 0) {
            return ret;
        }

        /* Allocate the entry. */
        entryPointer = allocator->malloc(sizeof(benc_dict_entry_t), allocator);
        if (entryPointer == NULL) {
            return OUT_OF_SPACE_TO_WRITE;
        }

        entryPointer->next = lastEntryPointer;
        entryPointer->key = key;
        entryPointer->val = value;
        lastEntryPointer = entryPointer;
    }

    /* We got an 'e', leave the pointer on the next char after it. */
    reader->skip(1, reader);

    *headPointer = lastEntryPointer;

    return 0;

    #undef OUT_OF_SPACE_TO_WRITE
    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
}

benc_dict_entry_t * benc_dict_entry_new(benc_dict_entry_t *next, benc_bstr_t *key, bobj_t *val)
{
    benc_dict_entry_t *d = (benc_dict_entry_t *)malloc(sizeof(benc_dict_entry_t));
    d->next = next;
    d->key = key;
    d->val = val;
    return d;
}

bobj_t * benc_dict_entry_free_return_val(benc_dict_entry_t *d)
{
    bobj_t *val = d->val;
    benc_bstr_free(d->key);
    free(d);
    return val;
}

bobj_t * bobj_dict_new()
{
    bobj_t *obj = bobj_new(BENC_DICT);
    obj->as.dict = NULL;
    return obj;
}

bobj_t * bobj_dict_lookup(bobj_t *obj, benc_bstr_t *key)
{
    if (obj == NULL || key == NULL || obj->type != BENC_DICT) {
        return NULL;
    }
    benc_dict_entry_t *curr = obj->as.dict;
    while (NULL != curr)
    {
        if (0 == benc_bstr_compare(key, curr->key))
        {
            return curr->val;
        }
        
        curr = curr->next;
    }
    
    /* key not found */
    return NULL;
}

bool bobj_dict_insert(bobj_t *obj, benc_bstr_t *key, bobj_t *val)
{
    benc_dict_entry_t **prev_p = &(obj->as.dict);
    benc_dict_entry_t *curr = obj->as.dict;
    while (NULL != curr)
    {
        int cmp = benc_bstr_compare(key, curr->key);
        if (cmp < 0)
        {
            break;
        }
        else if (cmp == 0)
        {
            BENC_LOG_EXCEPTION("dict already contains this key");
            return false;
        }
        
        prev_p = &(curr->next);
        curr = curr->next;
    }
    
    *prev_p = benc_dict_entry_new(curr, key, val);
    return true;
}

bobj_t * bobj_dict_remove(bobj_t *obj, benc_bstr_t *key)
{
    benc_dict_entry_t **prev_p = &(obj->as.dict);
    benc_dict_entry_t *curr = obj->as.dict;
    while (NULL != curr)
    {
        if (0 == benc_bstr_compare(key, curr->key))
        {
            *prev_p = curr->next;
            return benc_dict_entry_free_return_val(curr);
        }
        
        prev_p = &(curr->next);
        curr = curr->next;
    }
    
    BENC_LOG_EXCEPTION("key not found");
    return NULL;
}
