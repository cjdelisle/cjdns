#include "bencode.h"

benc_list_entry_t * benc_list_entry_new(benc_list_entry_t *next, bobj_t *e);
bobj_t * benc_list_entry_free_return_elem(benc_list_entry_t *l);

void benc_list_free(benc_list_entry_t *head)
{
    benc_list_entry_t *curr = head;
    benc_list_entry_t *tmp;
    while (NULL != curr)
    {
        tmp = curr->next;
        bobj_free(curr->elem);
        free(curr);
        curr = tmp;
    }
}

size_t benc_list_repsize(benc_list_entry_t *head)
{
    size_t repsize = 2;
    benc_list_entry_t *curr = head;
    while (NULL != curr)
    {
        repsize += bobj_repsize(curr->elem);
        curr = curr->next;
    }
    return repsize;
}

void benc_list_encode(bbuf_t *b, benc_list_entry_t *head)
{
    *(b->ptr)++ = 'l';
    benc_list_entry_t *curr = head;
    while (NULL != curr)
    {
        bobj_encode(b, curr->elem);
        curr = curr->next;
    }
    *(b->ptr)++ = 'e';
}

/** @see bencode.h */
int benc_list_print(struct Writer* writer,
                    size_t padSpaceCount,
                    benc_list_entry_t* head)
{
    char* thirtyTwoSpaces = "                                ";
    int padCounter = 0;
    #define PAD(padSpaces)                                                  \
        while (32 < padSpaces + padCounter) {                               \
            writer->write(thirtyTwoSpaces, 32, writer);                     \
            padCounter += 32;                                               \
        }                                                                   \
        writer->write(thirtyTwoSpaces, padSpaces - padCounter, writer)

    writer->write("[\n", 2, writer);

    benc_list_entry_t* entry = head;
    while (entry != NULL) {
        PAD(padSpaceCount + 2);
        benc_bobj_print(writer, padSpaceCount + 2, entry->elem);
        entry = entry->next;
        if (entry != NULL) {
            writer->write(",\n", 2, writer);
        }
    }

    writer->write("\n", 1, writer);
    PAD(padSpaceCount);
    return writer->write("]", 1, writer);

    #undef PAD
}

/** @see bencode.h */
int benc_list_serialize(struct Writer* writer,
                        benc_list_entry_t* head)
{
    int ret = writer->write("l", 1, writer);
    benc_list_entry_t* entry = head;
    while (ret == 0 && entry != NULL) {
        bobj_serialize(writer, entry->elem);
        entry = entry->next;
    }
    ret = writer->write("e", 1, writer);
    return ret;
}

/** @see bencode.h */
int benc_list_parse(struct Reader* reader,
                    struct MemAllocator* allocator,
                    benc_list_entry_t** listPointer)
{
    #define OUT_OF_SPACE_TO_WRITE -1
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
    benc_list_entry_t* thisEntry;
    benc_list_entry_t** lastEntryPointer = listPointer;
    int ret;

    while (nextChar != 'e') {
        ret = bobj_parse(reader, allocator, &element);
        if (ret != 0) {
            return ret;
        }
        thisEntry = allocator->malloc(sizeof(benc_list_entry_t), allocator);
        if (thisEntry == NULL) {
            return OUT_OF_SPACE_TO_WRITE;
        }
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

    #undef OUT_OF_SPACE_TO_WRITE
    #undef OUT_OF_CONTENT_TO_READ
    #undef UNPARSABLE
}

bool benc_list_decode(bbuf_t *b, benc_list_entry_t **head_p)
{
    bbuf_inc_ptr(b);
    benc_list_entry_t **prev_p, *curr;
    prev_p = head_p;
    while (*(b->ptr) != 'e')
    {
        bobj_t *elem = bdec_mem(b);
        if (NULL == elem)
        {
            BENC_LOG_EXCEPTION("couldn't read list element");
            return false;
        }
        curr = benc_list_entry_new(NULL, elem);
        *prev_p = curr;
        prev_p = &(curr->next);
    }
    bbuf_inc_ptr(b);
    return true;
}

benc_list_entry_t * benc_list_entry_new(benc_list_entry_t *next, bobj_t *elem)
{
    benc_list_entry_t *l = (benc_list_entry_t *)malloc(sizeof(benc_list_entry_t));
    l->next = next;
    l->elem = elem;
    return l;
}

bobj_t * benc_list_entry_free_return_elem(benc_list_entry_t *l)
{
    bobj_t *elem = l->elem;
    free(l);
    return elem;
}

bobj_t * bobj_list_new()
{
    bobj_t *obj = bobj_new(BENC_LIST);
    obj->as.list = NULL;
    return obj;
}

void bobj_list_push(bobj_t *obj, bobj_t *elem)
{
    obj->as.list = benc_list_entry_new(obj->as.list, elem);
}

bobj_t * bobj_list_pop(bobj_t *obj)
{
    if (NULL == obj->as.list)
    {
        return NULL;
    }
    
    benc_list_entry_t *new_head = obj->as.list->next;
    bobj_t *elem = benc_list_entry_free_return_elem(obj->as.list);
    obj->as.list = new_head;
    return elem;
}
