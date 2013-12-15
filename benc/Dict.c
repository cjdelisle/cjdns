/* vim: set expandtab ts=4 sw=4: */
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
#include "benc/Dict.h"
#include "benc/String.h"

#include <stddef.h>

int32_t Dict_size(const Dict* dictionary)
{
    if (dictionary != NULL) {
        struct Dict_Entry* entry = *dictionary;
        int32_t i;
        for (i = 0; entry != NULL; i++) {
            entry = entry->next;
        }
        return i;
    }
    return 0;
}

static Object* lookupObject(const Dict* dictionary, const String* key)
{
    if (dictionary == NULL || key == NULL) {
        return NULL;
    }
    const struct Dict_Entry* curr = *dictionary;
    while (curr != NULL) {
        if (String_equals(key, curr->key)) {
            return curr->val;
        }

        curr = curr->next;
    }

    /* key not found */
    return NULL;
}

/** @see Object.h */
int64_t* Dict_getInt(const Dict* dictionary, const String* key)
{
    Object* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != Object_INTEGER) {
        return NULL;
    }
    return &(obj->as.number);
}

/** @see Object.h */
String* Dict_getString(const Dict* dictionary, const String* key)
{
    Object* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != Object_STRING) {
        return NULL;
    }
    return obj->as.string;
}

/** @see Object.h */
Dict* Dict_getDict(const Dict* dictionary, const String* key)
{
    Object* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != Object_DICT) {
        return NULL;
    }
    return obj->as.dictionary;
}

/** @see Object.h */
List* Dict_getList(const Dict* dictionary, const String* key)
{
    Object* obj = lookupObject(dictionary, key);
    if (obj == NULL || obj->type != Object_LIST) {
        return NULL;
    }
    return obj->as.list;
}

/** @see Object.h */
Dict* Dict_new(struct Allocator* allocator)
{
    return Allocator_calloc(allocator, sizeof(Dict), 1);
}

/**
 * Put a key:value pair into a dictionary.
 * NOTE: This will not copy the given object,
 *       only add a pointer to it in the dictionary.
 * If dictionary is NULL then a new dictionary will be created and returned.
 *
 * @param dictionary this must be a bencoded dictionary or NULL, if NULL then a new dictionary
                     is made.
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
                         struct Allocator* allocator)
{
    struct Dict_Entry** prev_p = dictionary;
    struct Dict_Entry* current = *dictionary;
    while (current != NULL) {
        int cmp = String_compare(key, current->key);
        if (cmp < 0) {
            break;
        } else if (cmp == 0) {
            Object* out = current->val;
            current->val = value;
            return out;
        }
        prev_p = &(current->next);
        current = current->next;
    }
    struct Dict_Entry* entry = Allocator_malloc(allocator, sizeof(struct Dict_Entry));
    entry->key = (String*) key; // need to drop the const :(
    entry->val = value;
    entry->next = current;
    *prev_p = entry;

    return NULL;
}

/** @see Object.h */
Object* Dict_putInt(Dict* dictionary,
                        const String* key,
                        int64_t value,
                        struct Allocator* allocator)
{
    Object* v = Allocator_clone(allocator, (&(Object) {
        .type = Object_INTEGER,
        .as.number = value
    }));
    return putObject(dictionary, key, v, allocator);
}

/** @see Object.h */
Object* Dict_putString(Dict* dictionary,
                       const String* key,
                       String* value,
                       struct Allocator* allocator)
{
    if (key == NULL || value == NULL) {
        return NULL;
    }
    Object* v = Allocator_clone(allocator, (&(Object) {
        .type = Object_STRING,
        .as.string = value
    }));
    return putObject(dictionary, key, v, allocator);
}

/** @see Object.h */
Object* Dict_putList(Dict* dictionary,
                     const String* key,
                     List* value,
                     struct Allocator* allocator)
{
    if (key == NULL || value == NULL) {
        return NULL;
    }
    Object* v = Allocator_clone(allocator, (&(Object) {
        .type = Object_LIST,
        /* Lists and dictionaries are double pointers so they have to be loaded. */
        .as.list = value
    }));
    return putObject(dictionary, key, v, allocator);
}

Object* Dict_putDict(Dict* dictionary,
                           const String* key,
                           Dict* value,
                           struct Allocator* allocator)
{
    if (key == NULL || value == NULL) {
        return NULL;
    }
    Object* v = Allocator_clone(allocator, (&(Object) {
        .type = Object_DICT,
        /* Lists and dictionaries are double pointers so they have to be loaded. */
        .as.dictionary = value
    }));
    return putObject(dictionary, key, v, allocator);
}

/** @see Object.h */
int32_t Dict_remove(Dict* dictionary, const String* key)
{
    struct Dict_Entry** prev_p = dictionary;
    struct Dict_Entry* current = *dictionary;
    while (current != NULL) {
        if (String_equals(key, current->key)) {
            *prev_p = current->next;
            return 1;
        }

        prev_p = &(current->next);
        current = current->next;
    }

    return 0;
}
