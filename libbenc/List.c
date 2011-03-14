#include "memory/MemAllocator.h"
#include "benc.h"

int32_t benc_listLength(List* list)
{
    if (list != NULL) {
        benc_list_entry_t* entry = *list;
        int32_t i;
        for (i = 0; entry != NULL; i++) {
            entry = entry->next;
        }
        return i;
    }
    return -1;
}

static List* addObject(List* list, Object* item, const struct MemAllocator* allocator)
{
    if (list == NULL) {
        List* newList = allocator->calloc(sizeof(List), 1, allocator);
        return addObject(newList, item, allocator);
    }

    benc_list_entry_t* entry = allocator->malloc(sizeof(benc_list_entry_t), allocator);
    entry->next = *list;
    entry->elem = item;
    *list = entry;

    return list;
}

/** @see benc.h */
List* benc_addInteger(List* list, Integer toAdd, const struct MemAllocator* allocator)
{
    Object* obj = allocator->clone(sizeof(Object), allocator, &(bobj_t) {
        .type = BENC_INT,
        .as.int_ = toAdd
    });
    return addObject(list, obj, allocator);
}

/** @see benc.h */
List* benc_addString(List* list, String* toAdd, const struct MemAllocator* allocator)
{
    Object* obj = allocator->clone(sizeof(Object), allocator, &(bobj_t) {
        .type = BENC_BSTR,
        .as.bstr = toAdd
    });
    return addObject(list, obj, allocator);
}

/** @see benc.h */
List* benc_addDictionary(List* list, Dict* toAdd, const struct MemAllocator* allocator)
{
    Object* obj = allocator->clone(sizeof(Object), allocator, &(bobj_t) {
        .type = BENC_DICT,
        .as.dict = *toAdd
    });
    return addObject(list, obj, allocator);
}

/** @see benc.h */
List* benc_addList(List* list, List* toAdd, const struct MemAllocator* allocator)
{
    Object* obj = allocator->clone(sizeof(Object), allocator, &(bobj_t) {
        .type = BENC_LIST,
        .as.list = *toAdd
    });
    return addObject(list, obj, allocator);
}

static Object* getObject(const List* list, uint32_t index)
{
    if (list != NULL && *list != NULL) {
        benc_list_entry_t* entry = *list;
        uint32_t i;
        for (i = 0; i < index && entry != NULL; i++) {
            entry = entry->next;
        }
        return entry->elem;
    }
    return NULL;
}

/** @see benc.h */
Integer* benc_getInteger(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == BENC_INT) {
        return &(o->as.int_);
    }
    return NULL;
}

/** @see benc.h */
String* benc_getString(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == BENC_BSTR) {
        return o->as.bstr;
    }
    return NULL;
}

/** @see benc.h */
Dict* benc_getDictionary(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == BENC_DICT) {
        return &(o->as.dict);
    }
    return NULL;
}

/** @see benc.h */
List* benc_getList(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == BENC_LIST) {
        return &(o->as.list);
    }
    return NULL;
}
