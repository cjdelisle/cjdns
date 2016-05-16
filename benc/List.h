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
#ifndef List_H
#define List_H

#include "memory/Allocator.h"
#include "benc/Object.h"
#include "util/Linker.h"
Linker_require("benc/List.c");

struct List_Item;
struct List_Item {
    struct List_Item* next;
    Object* elem;
};

List* List_new(struct Allocator* alloc);

/**
 * Get the length of a list.
 *
 * @param a list
 * @return the length of the given list or -1 if the list argument is NULL.
 */
int32_t List_size(const List* list);

/**
 * Add an integer to a list, if the list does not exist then it is allocated.
 *
 * @param list the list to add the integer item to, if NULL then it is allocated.
 * @param toAdd the integer to add to the list. int64_t is an alias for int64_t
 * @param allocator the means of getting memory space for storing the list entry.
 */
void List_addInt(List* list, int64_t toAdd, struct Allocator* allocator);

/**
 * Add a string to a list, if the list does not exist then it is allocated.
 * NOTE: This will not copy the given string, only add a pointer to it in the list.
 *
 * @param list the list to add the string item to, if NULL then it is allocated.
 * @param toAdd the string to add to the list.
 * @param allocator the means of getting memory space for storing the list entry.
 */
void List_addString(List* list, String* toAdd, struct Allocator* allocator);

#define List_addStringC(l,s,a) List_addString(l, String_new(s, a), a)

/**
 * Add a dictionary to a list, if the list does not exist then it is allocated.
 * NOTE: This will not copy the given dictionary, only add a pointer to it in the list.
 *
 * @param list the list to add the string item to, if NULL then it is allocated.
 * @param toAdd the dictionary to add to the list.
 * @param allocator the means of getting memory space for storing the list entry.
 */
void List_addDict(List* list, Dict* toAdd, struct Allocator* allocator);

/**
 * Add a list as an item to another list, if the list does not exist then it is allocated.
 * NOTE: This will not copy the list which it is adding, only add a pointer to it in the list.
 *
 * @param list the list to add the item to, if NULL then it is allocated.
 * @param toAdd the list to add as an item.
 * @param allocator the means of getting memory space for storing the list entry.
 */
void List_addList(List* list, List* toAdd, struct Allocator* allocator);

/**
 * Get an integer from a list.
 *
 * @param list the list to get the integer item from.
 * @param index the index of the item.
 * @return a pointer to the integer at that index or NULL if either the index
 *         is out of range or the item at that index is not an integer.
 */
int64_t* List_getInt(const List* list, uint32_t index);

/**
 * Get a string from a list.
 *
 * @param list the list to get the string item from.
 * @param index the index of the item.
 * @return a pointer to the string at that index or NULL if either the index
 *         is out of range or the item at that index is not a string.
 */
String* List_getString(const List* list, uint32_t index);

/**
 * Get a dictionary from a list.
 *
 * @param list the list to get the dictionary from.
 * @param index the index of the item.
 * @return a pointer to the string at that index or NULL if either the index
 *         is out of range or the item at that index is not a dictionary.
 */
Dict* List_getDict(const List* list, uint32_t index);

/**
 * Get a list from another list.
 *
 * @param list the list to get the item from.
 * @param index the index of the item.
 * @return a pointer to the string at that index or NULL if either the index
 *         is out of range or the item at that index is not a list.
 */
List* List_getList(const List* list, uint32_t index);

#define List_OBJ(x) (&(Object) { .type = Object_LIST, .as.list = x })

#endif
