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
#ifndef Dict_H
#define Dict_H

#include "memory/Allocator.h"
#include "benc/Object.h"
#include "util/Linker.h"
Linker_require("benc/Dict.c");

struct Dict_Entry;
struct Dict_Entry {
    struct Dict_Entry* next;
    String* key;
    Object* val;
};

/**
 * Remove an entry from the dictionary.
 *
 * @param dictionary the dictionary to remove the entry from.
 * @param key the key which the entry is entered under.
 * @return 1 if there was an entry removed, 0 if there was not.
 */
int32_t Dict_remove(Dict* dictionary, const String* key);

/*----------------------- Size Functions -----------------------*/

/**
 * Get the number of entries in a dictionary.
 *
 * @param a dictionary
 * @return the number of entries in the dictionary or -1 if the dictionary argument is NULL.
 */
int32_t Dict_size(const Dict* dictionary);


/*----------------------- Walk the Dictionary -----------------------*/

#define Dict_forEach(_dict, _key) \
    for (struct Dict_Entry* e = *_dict; e && (_key = e->key); e = e->next)
// CHECKFILES_IGNORE


/*----------------------- Dictionary Lookup Functions -----------------------*/

/**
 * Lookup an integer value from a dictionary, if the value is not present
 * or is not an integer type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 * @return an integer which is in the dictionary under the given key or else NULL.
 */
int64_t* Dict_getInt(const Dict* dictionary, const String* key);

#define Dict_getIntC(getFromHere, key) Dict_getInt(getFromHere, String_CONST(key))

/**
 * Lookup a string value from a dictionary, if the value is not present
 * or is not a string type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 * @return a string which is in the dictionary under the given key or else NULL.
 */
String* Dict_getString(const Dict* dictionary, const String* key);

#define Dict_getStringC(getFromHere, key) Dict_getString(getFromHere, String_CONST(key))

/**
 * Lookup a dictionary value from another dictionary, if the value is not present
 * or is not a dictionary type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 * @return a dictionary which is in the dictionary under the given key or else NULL.
 */
Dict* Dict_getDict(const Dict* dictionary, const String* key);

#define Dict_getDictC(getFromHere, key) Dict_getDict(getFromHere, String_CONST(key))

/**
 * Lookup a list value from a dictionary, if the value is not present
 * or is not a list type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 * @return a list which is in the dictionary under the given key or else NULL.
 */
List* Dict_getList(const Dict* dictionary, const String* key);

#define Dict_getListC(getFromHere, key) Dict_getList(getFromHere, String_CONST(key))

/*----------------------- Dictionary Put Functions -----------------------*/

/**
 * Insert an integer into a dictionary.
 *
 * @param putIntoThis the dictionary to insert an entry into.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the integer to insert. int64_t is an alias to int64_t.
 * @param allocator the means to get memory for storing the dictionary entry wrapper.
 * @return if the key already exists in the dictionary then the value which was
 *         displaced by the put, otherwise NULL.
 */
Object* Dict_putInt(Dict* putIntoThis,
                    const String* key,
                    int64_t value,
                    struct Allocator* allocator);

#define Dict_putIntC(putHere, key, val, alloc) \
    Dict_putInt(putHere, String_new(key, alloc), val, alloc)

/**
 * Insert a String object into another dictionary.
 * NOTE: This will not copy the given object, only add a pointer to it in the dictionary.
 *
 * @param putIntoThis the dictionary to insert an entry into.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the string to insert.
 * @param allocator the means to get memory for storing the dictionary entry wrapper.
 * @return if the key already exists in the dictionary then the value which was
 *         displaced by the put, otherwise NULL.
 */
Object* Dict_putString(Dict* putIntoThis,
                       const String* key,
                       String* value,
                       struct Allocator* allocator);

#define Dict_putStringC(putHere, key, val, alloc) \
    Dict_putString(putHere, String_new(key, alloc), val, alloc)

#define Dict_putStringCC(putHere, key, val, alloc) \
    Dict_putStringC(putHere, key, String_new(val, alloc), alloc)

/**
 * Insert a Dictionary object into another dictionary.
 * NOTE: This will not copy the given object, only add a pointer to it in the dictionary.
 *
 * @param putIntoThis the dictionary to insert an entry into.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the value to insert.
 * @param allocator the memory allocator to use for getting memory for the entry.
 * @return if the key already exists in the dictionary then the value which was
 *         displaced by the put, otherwise NULL.
 */
Object* Dict_putDict(Dict* putIntoThis,
                     const String* key,
                     Dict* value,
                     struct Allocator* allocator);

#define Dict_putDictC(putHere, key, val, alloc) \
    Dict_putDict(putHere, String_new(key, alloc), val, alloc)

/**
 * Insert a List object into a dictionary.
 * NOTE: This will not copy the given object, only add a pointer to it in the dictionary.
 *
 * @param putIntoThis the dictionary to insert an entry into.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the list to insert.
 * @param allocator the memory allocator to use for getting memory for the entry.
 * @return if the key already exists in the dictionary then the value which was
 *         displaced by the put, otherwise NULL.
 */
Object* Dict_putList(Dict* putIntoThis,
                     const String* key,
                     List* value,
                     struct Allocator* allocator);

#define Dict_putListC(putHere, key, val, alloc) \
    Dict_putList(putHere, String_new(key, alloc), val, alloc)

/*----------------------- Constructors -----------------------*/

/**
 * Create a new bencoded dictionary type.
 *
 * @param allocator the place to allocate the memory for storing the dictionary.
 */
Dict* Dict_new(struct Allocator* allocator);

/**
 * Create a dictionary on the stack.
 *
 * @param entryKey the key which must be a string.
 * @param value the value which must be an object.
 * @param next another Dict, can be used to chain Dict_CONST() calls.
 * @return a Dict. NOTE: This does *NOT* return a Dict*, just a Dict.
 */
#define Dict_CONST(entryKey, value, nextDict) \
    (&(struct Dict_Entry) {                   \
        .key = entryKey,                      \
        .val = value,                         \
        .next = nextDict                      \
    })

#define Dict_OBJ(x) (&(Object) { .type = Object_DICT, .as.dictionary = x })

#endif
