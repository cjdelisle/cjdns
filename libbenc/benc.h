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
#ifndef BENC_H
#define BENC_H

#include <stdint.h>

#include "memory/Allocator.h"

typedef struct bobj_s               bobj_t;
typedef int64_t                     benc_int_t;
typedef struct benc_bstr_s          benc_bstr_t;
typedef struct benc_list_entry_s    benc_list_entry_t;
typedef struct benc_dict_entry_s    benc_dict_entry_t;
#define String benc_bstr_t
/* Dictionaries and lists are pointers to the head entry so that the head can change. */
typedef benc_dict_entry_t* benc_dict_t;
#define Dict benc_dict_t
typedef benc_list_entry_t* benc_list_t;
#define List benc_list_t
#define Object bobj_t
#define Integer int64_t

enum benc_data_type { BENC_INT, BENC_BSTR, BENC_LIST, BENC_DICT, BENC_UNPARSABLE };

bobj_t *        bobj_dict_lookup(bobj_t* obj, const benc_bstr_t* key);

/**
 * In order to make the names somewhat intuitive and not horribly verbose
 * while obeying C's prohabition on function overloading, I have used some
 * linguistic tricks to differentiate the operations which are done on lists
 * and those done on dictionaries.
 * When you think "put", "lookup" or "entry", think dictionary.
 * When you think "add", "get", "set", or "item", think list.
 * The exception is remove. If you are removing an item from a list, you want to
 * use benc_removeItem() while removing a dictionary entry requires benc_removeEntry()
 */


/**
 * Remove an entry from the dictionary.
 *
 * @param dictionary the dictionary to remove the entry from.
 * @param key the key which the entry is entered under.
 * @return 1 if there was an entry removed, 0 if there was not.
 */
int32_t benc_removeEntry(Dict* dictionary, const String* key);

/*----------------------- Size Functions -----------------------*/

/**
 * Get the length of a list.
 *
 * @param a list
 * @return the length of the given list or -1 if the list argument is NULL.
 */
int32_t benc_itemCount(const List* list);

/**
 * Get the number of entries in a dictionary.
 *
 * @param a dictionary
 * @return the number of entries in the dictionary or -1 if the dictionary argument is NULL.
 */
int32_t benc_entryCount(const Dict* dictionary);

/*----------------------- List Add Functions -----------------------*/

/**
 * Add an integer to a list, if the list does not exist then it is allocated.
 *
 * @param list the list to add the integer item to, if NULL then it is allocated.
 * @param toAdd the integer to add to the list. Integer is an alias for int64_t
 * @param allocator the means of getting memory space for storing the list entry.
 * @return the list after adding the integer.
 */
List* benc_addInteger(List* list, Integer toAdd, const struct Allocator* allocator);

/**
 * Add a string to a list, if the list does not exist then it is allocated.
 * NOTE: This will not copy the given string, only add a pointer to it in the list.
 *
 * @param list the list to add the string item to, if NULL then it is allocated.
 * @param toAdd the string to add to the list.
 * @param allocator the means of getting memory space for storing the list entry.
 * @return the list after adding the string.
 */
List* benc_addString(List* list, String* toAdd, const struct Allocator* allocator);

/**
 * Add a dictionary to a list, if the list does not exist then it is allocated.
 * NOTE: This will not copy the given dictionary, only add a pointer to it in the list.
 *
 * @param list the list to add the string item to, if NULL then it is allocated.
 * @param toAdd the dictionary to add to the list.
 * @param allocator the means of getting memory space for storing the list entry.
 * @return the list after adding the dictionary.
 */
List* benc_addDictionary(List* list, Dict* toAdd, const struct Allocator* allocator);

/**
 * Add a list as an item to another list, if the list does not exist then it is allocated.
 * NOTE: This will not copy the list which it is adding, only add a pointer to it in the list.
 *
 * @param list the list to add the item to, if NULL then it is allocated.
 * @param toAdd the list to add as an item.
 * @param allocator the means of getting memory space for storing the list entry.
 * @return the list after adding the item.
 */
List* benc_addList(List* list, List* toAdd, const struct Allocator* allocator);

/*----------------------- List Get Functions -----------------------*/

/**
 * Get an integer from a list.
 *
 * @param list the list to get the integer item from.
 * @param index the index of the item.
 * @return a pointer to the integer at that index or NULL if either the index
 *         is out of range or the item at that index is not an integer.
 */
Integer* benc_getInteger(const List* list, uint32_t index);

/**
 * Get a string from a list.
 *
 * @param list the list to get the string item from.
 * @param index the index of the item.
 * @return a pointer to the string at that index or NULL if either the index
 *         is out of range or the item at that index is not a string.
 */
String* benc_getString(const List* list, uint32_t index);

/**
 * Get a dictionary from a list.
 *
 * @param list the list to get the dictionary from.
 * @param index the index of the item.
 * @return a pointer to the string at that index or NULL if either the index
 *         is out of range or the item at that index is not a dictionary.
 */
Dict* benc_getDictionary(const List* list, uint32_t index);

/**
 * Get a list from another list.
 *
 * @param list the list to get the item from.
 * @param index the index of the item.
 * @return a pointer to the string at that index or NULL if either the index
 *         is out of range or the item at that index is not a list.
 */
List* benc_getList(const List* list, uint32_t index);

/*----------------------- Dictionary Lookup Functions -----------------------*/

/**
 * Lookup an integer value from a dictionary, if the value is not present
 * or is not an integer type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 * @return an integer which is in the dictionary under the given key or else NULL.
 */
Integer* benc_lookupInteger(const Dict* dictionary, const String* key);

/**
 * Lookup a string value from a dictionary, if the value is not present
 * or is not a string type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 * @return a string which is in the dictionary under the given key or else NULL.
 */
String* benc_lookupString(const Dict* dictionary, const String* key);

/**
 * Lookup a dictionary value from another dictionary, if the value is not present
 * or is not a dictionary type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 * @return a dictionary which is in the dictionary under the given key or else NULL.
 */
Dict* benc_lookupDictionary(const Dict* dictionary, const String* key);

/**
 * Lookup a list value from a dictionary, if the value is not present
 * or is not a list type then NULL is returned.
 *
 * @param dictionary the dictionary to look the entry up in.
 * @param key the key to look the entry up with.
 * @return a list which is in the dictionary under the given key or else NULL.
 */
List* benc_lookupList(const Dict* dictionary, const String* key);

/*----------------------- Dictionary Put Functions -----------------------*/

/**
 * Insert an integer into a dictionary.
 *
 * @param putIntoThis the dictionary to insert an entry into.
 * @param key the reference key to use for putting the entry in the dictionary.
 * @param value the integer to insert. Integer is an alias to int64_t.
 * @param allocator the means to get memory for storing the dictionary entry wrapper.
 * @return if the key already exists in the dictionary then the value which was
 *         displaced by the put, otherwise NULL.
 */
Object* benc_putInteger(Dict* putIntoThis,
                        const String* key,
                        Integer value,
                        const struct Allocator* allocator);

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
Object* benc_putString(Dict* putIntoThis,
                       const String* key,
                       String* value,
                       const struct Allocator* allocator);

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
Object* benc_putDictionary(Dict* putIntoThis,
                           const String* key,
                           Dict* value,
                           const struct Allocator* allocator);

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
Object* benc_putList(Dict* putIntoThis,
                     const String* key,
                     List* value,
                     const struct Allocator* allocator);

/*----------------------- Constructors -----------------------*/

/**
 * Create a new bencoded dictionary type.
 *
 * @param allocator the place to allocate the memory for storing the dictionary.
 */
Dict* benc_newDictionary(const struct Allocator* allocator);

/**
 * Create a new bencoded string from a C null terminated string.
 * This implementation will make a copy of the string into the memory provided by the allocator.
 *
 * @param bytes the beginning of a memory location containing the string to use.
 * @param allocator a means of getting the memory to store the string object.
 * @return a bencoded string.
 */
String* benc_newString(const char* bytes, const struct Allocator* allocator);

/**
 * Create a new bencoded string from a set of bytes.
 * This implementation will make a copy of the string into the memory provided by the allocator.
 *
 * @param bytes the beginning of a memory location containing thre string to use.
                if NULL then this will make a new string of null characters.
 * @param length the number of bytes to use from the location.
 * @param allocator a means of getting the memory to store the string object.
 * @return a bencoded string.
 */
String* benc_newBinaryString(const char* bytes, size_t length, const struct Allocator* allocator);

/*----------------------- Comparitors -----------------------*/

/**
 * Compare 2 bencoded strings.
 * If the first differing character is numerically smaller for input a then
 * a negative number is returned, if the first differing character is numerically
 * smaller for input b then a positive number. If all characters in a and b are
 * the same then the difference in length (a->len - b->len) is returned.
 * If a is NULL and b is not NULL then a negative is returned, if b is NULL and a
 * not NULL then a positive is returned, if both NULL then 0.
 *
 * @param a the first string to compare.
 * @param b the second string to compare.
 * @return the output from comparison, 0 if and only if they are equal.
 */
int benc_bstr_compare(const benc_bstr_t* a, const benc_bstr_t* b);

/**
 * Will return 1 if and only if the benc_bstr_compare() would return 0.
 *
 * @param a the first string to compare.
 * @param b the second string to compare.
 * @return !(benc_bstr_compare(a, b))
 */
int32_t benc_stringEquals(const benc_bstr_t* a, const benc_bstr_t* b);

/*----------------------- Structures -----------------------*/

struct bobj_s {
    enum benc_data_type          type;
    union {
        benc_int_t               int_;
        benc_bstr_t              *bstr;
        List* list;
        Dict* dictionary;
    } as;
};

struct benc_bstr_s {
    size_t                       len;
    char                      *bytes;
};

struct benc_list_entry_s {
    benc_list_entry_t            *next;
    bobj_t                       *elem;
};

struct benc_dict_entry_s {
    benc_dict_entry_t            *next;
    const benc_bstr_t* key;
    bobj_t                       *val;
};

#endif        /* #ifndef BENC_H */
