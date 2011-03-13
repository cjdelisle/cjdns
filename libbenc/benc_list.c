#include "bencode.h"

bobj_t * benc_list_entry_free_return_elem(benc_list_entry_t *l);

/** @see bencode.h */
int benc_list_print(const struct Writer* writer,
                    const size_t padSpaceCount,
                    const benc_list_entry_t* head)
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

    const benc_list_entry_t* entry = head;
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
int benc_list_serialize(const struct Writer* writer,
                        const benc_list_entry_t* head)
{
    int ret = writer->write("l", 1, writer);
    const benc_list_entry_t* entry = head;
    while (ret == 0 && entry != NULL) {
        bobj_serialize(writer, entry->elem);
        entry = entry->next;
    }
    ret = writer->write("e", 1, writer);
    return ret;
}

/** @see bencode.h */
int benc_list_parse(const struct Reader* reader,
                    const struct MemAllocator* allocator,
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

bobj_t * benc_list_entry_free_return_elem(benc_list_entry_t *l)
{
    bobj_t *elem = l->elem;
    free(l);
    return elem;
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

List* benc_addObject(List* list, Object* item, const struct MemAllocator* allocator)
{
    if (list == NULL) {
        List* newList = allocator->calloc(sizeof(List), 1, allocator);
        return benc_addObject(newList, item, allocator);
    }

    benc_list_entry_t* entry = allocator->malloc(sizeof(benc_list_entry_t), allocator);
    entry->next = *list;
    entry->elem = item;
    *list = entry;

    return list;
}

List* benc_addString(List* list, String* toAdd, const struct MemAllocator* allocator)
{
    Object* strObj = allocator->malloc(sizeof(Object), allocator);
    strObj->type = BENC_BSTR;
    strObj->as.bstr = toAdd;
    return benc_addObject(list, strObj, allocator);
}
