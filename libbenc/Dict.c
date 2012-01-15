/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "memory/Allocator.h"
#include "benc.h"

int32_t benc_entryCount(const Dict* dictionary)
{
    if (dictionary != NULL) {
        benc_dict_entry_t* entry = *dictionary;
        int32_t i;
        for (i = 0; entry != NULL; i++) {
            entry = entry->next;
        }
        return i;
    }
    return -1;
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
    return lookupObject(obj->as.dictionary, key);
}

/** @see benc.h */
Integer* benc_lookupInteger(const Dict* dictionary, const String* key)
{
    bobj_t* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != BENC_INT) {
        return NULL;
    }
    return &(obj->as.int_);
}

/** @see benc.h */
String* benc_lookupString(const Dict* dictionary, const String* key)
{
    bobj_t* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != BENC_BSTR) {
        return NULL;
    }
    return obj->as.bstr;
}

/** @see benc.h */
Dict* benc_lookupDictionary(const Dict* dictionary, const String* key)
{
    bobj_t* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != BENC_DICT) {
        return NULL;
    }
    return obj->as.dictionary;
}

/** @see benc.h */
List* benc_lookupList(const Dict* dictionary, const String* key)
{
    bobj_t* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != BENC_LIST) {
        return NULL;
    }
    return obj->as.list;
}

/** @see benc.h */
Dict* benc_newDictionary(const struct Allocator* allocator)
{
    return allocator->calloc(sizeof(Dict), 1, allocator);
}

/**
 * Put a key:value pair into a dictionary.
 * NOTE: This will not copy the given object,
 *       only add a pointer to it in the dictionary.
 * If dictionary is NULL then a new dictionary will be created and returned.
 *
 * @param dictionary this must be a bencoded dictionary or NULL, if NULL then a new dictionary is made.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the value to insert with the key.
 * @param allocator the means to get memory for storing the dictionary entry wrapper.
 * @return if the dictionary parameter is NULL then this will be the newly created dictionary.
 *         Otherwise: if the key already exists in the dictionary then the value which was
 *         displaced by the put, if not then NULL.
 */
static Object* putObject(Dict* dictionary,
                         const String* key,
                         Object* value,
                         const struct Allocator* allocator)
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
Object* benc_putInteger(Dict* dictionary,
                        const String* key,
                        Integer value,
                        const struct Allocator* allocator)
{
    Object* v = allocator->clone(sizeof(bobj_t), allocator, &(bobj_t) {
        .type = BENC_INT,
        .as.int_ = value
    });
    return putObject(dictionary, key, v, allocator);
}

/** @see benc.h */
Object* benc_putString(Dict* dictionary,
                       const String* key,
                       String* value,
                       const struct Allocator* allocator)
{
    if (key == NULL || value == NULL) {
        return NULL;
    }
    Object* v = allocator->clone(sizeof(bobj_t), allocator, &(bobj_t) {
        .type = BENC_BSTR,
        .as.bstr = value
    });
    return putObject(dictionary, key, v, allocator);
}

/** @see benc.h */
Object* benc_putList(Dict* dictionary,
                     const String* key,
                     List* value,
                     const struct Allocator* allocator)
{
    if (key == NULL || value == NULL) {
        return NULL;
    }
    Object* v = allocator->clone(sizeof(bobj_t), allocator, &(bobj_t) {
        .type = BENC_LIST,
        /* Lists and dictionaries are double pointers so they have to be loaded. */
        .as.list = value
    });
    return putObject(dictionary, key, v, allocator);
}

Object* benc_putDictionary(Dict* dictionary,
                           const String* key,
                           Dict* value,
                           const struct Allocator* allocator)
{
    if (key == NULL || value == NULL) {
        return NULL;
    }
    Object* v = allocator->clone(sizeof(bobj_t), allocator, &(bobj_t) {
        .type = BENC_DICT,
        /* Lists and dictionaries are double pointers so they have to be loaded. */
        .as.dictionary = value
    });
    return putObject(dictionary, key, v, allocator);
}

/** @see benc.h */
int32_t benc_removeEntry(Dict* dictionary, const String* key)
{
    benc_dict_entry_t** prev_p = dictionary;
    benc_dict_entry_t* current = *dictionary;
    while (current != NULL) {
        if (benc_stringEquals(key, current->key)) {
            *prev_p = current->next;
            return 1;
        }
        
        prev_p = &(current->next);
        current = current->next;
    }

    return 0;
}
