#include "bencode.h"

/** @see bencode.h */
int benc_dict_print(const struct Writer* writer,
                    size_t padSpaceCount,
                    const benc_dict_entry_t* head)
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

    const benc_dict_entry_t* entry = head;
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
int benc_dict_serialize(const struct Writer* writer,
                        const benc_dict_entry_t* head)
{
    const benc_dict_entry_t* entry = head;
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
int benc_dict_parse(const struct Reader* reader,
                    const struct MemAllocator* allocator,
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

static bobj_t* lookupObject(const Dict* dictionary, const String* key)
{
    if (dictionary == NULL || key == NULL) {
        return NULL;
    }
    const benc_dict_entry_t* curr = *dictionary;
    while (curr != NULL) {
        if (benc_stringEquals(key, curr->key)) {
            return curr->val;
        }
        
        curr = curr->next;
    }
    
    /* key not found */
    return NULL;
}

bobj_t * bobj_dict_lookup(bobj_t* obj, const benc_bstr_t* key)
{
    if (obj == NULL || key == NULL || obj->type != BENC_DICT) {
        return NULL;
    }
    benc_dict_entry_t* dict = obj->as.dict;
    return lookupObject(&dict, key);
}

String* benc_lookupString(const Dict* dictionary, const String* key)
{
    bobj_t* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != BENC_BSTR) {
        return NULL;
    }
    return obj->as.bstr;
}

Dict* benc_lookupDictionary(const Dict* dictionary, const String* key)
{
    bobj_t* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != BENC_DICT) {
        return NULL;
    }
    return &(obj->as.dict);
}

Dict* benc_newDictionary(const struct MemAllocator* allocator)
{
    return allocator->calloc(sizeof(Dict), 1, allocator);
}

/** @see benc.h */
Object* benc_putObject(Dict* dictionary,
                       const String* key,
                       Object* value,
                       const struct MemAllocator* allocator)
{
    benc_dict_entry_t** prev_p = dictionary;
    benc_dict_entry_t* current = *dictionary;
    while (current != NULL) {
        int cmp = benc_bstr_compare(key, current->key);
        if (cmp < 0) {
            break;
        } else if (cmp == 0) {
            bobj_t* out = current->val;
            current->val = value;
            return out;
        }
        prev_p = &(current->next);
        current = current->next;
    }
    benc_dict_entry_t* entry = allocator->malloc(sizeof(benc_dict_entry_t), allocator);
    entry->key = key;
    entry->val = value;
    entry->next = current;
    *prev_p = entry;

    return NULL;
}

/** @see benc.h */
Object* benc_putString(Dict* dictionary,
                       const String* key,
                       String* value,
                       const struct MemAllocator* allocator)
{
    Object* v = allocator->clone(sizeof(bobj_t), allocator, &(bobj_t) {
        .type = BENC_BSTR,
        .as.bstr = value
    });
    return benc_putObject(dictionary, key, v, allocator);
}

/** @see benc.h */
Object* benc_putList(Dict* dictionary,
                     const String* key,
                     List* value,
                     const struct MemAllocator* allocator)
{
    Object* v = allocator->clone(sizeof(bobj_t), allocator, &(bobj_t) {
        .type = BENC_LIST,
        /* Lists and dictionaries are double pointers so they have to be loaded. */
        .as.list = *value
    });
    return benc_putObject(dictionary, key, v, allocator);
}

Object* benc_putDictionary(Dict* dictionary,
                           const String* key,
                           Dict* value,
                           const struct MemAllocator* allocator)
{
    Object* v = allocator->clone(sizeof(bobj_t), allocator, &(bobj_t) {
        .type = BENC_DICT,
        /* Lists and dictionaries are double pointers so they have to be loaded. */
        .as.dict = *value
    });
    return benc_putObject(dictionary, key, v, allocator);
}

/** @see benc.h */
bobj_t* benc_removeEntry(Dict* dictionary, const String* key)
{
    benc_dict_entry_t** prev_p = dictionary;
    benc_dict_entry_t* current = *dictionary;
    while (current != NULL) {
        if (benc_stringEquals(key, current->key)) {
            *prev_p = current->next;
            return current->val;
        }
        
        prev_p = &(current->next);
        current = current->next;
    }

    return NULL;
}
