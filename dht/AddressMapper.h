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
#ifndef AddressMapper_H
#define AddressMapper_H

/** Address mapper doubly linked list LRU cache.
 * The most recently used item is on the head.
 * The least recently used item is on the tail.
 *
 * When an item is used, it is relocated to the head. Searching from the
 * head therefore means items will be checked in most-recently-used order.
 *
 * When inserting a new item,
 * the item to replace - the least-recently-used item - is simply the tail.
 * Then the tail is set to be the head, because it is the most recently used item.
 */

#include "memory/Allocator.h"
#include "util/Assert.h"
#include <stdint.h>

struct AddressMapper_Entry
{
    uint64_t label;
    uint8_t address[16];
    struct AddressMapper_Entry* next;
    struct AddressMapper_Entry* prev;
};
#define AddressMapper_Entry_SIZE (16 + 8 + sizeof(char*) + sizeof(char*))
Assert_compileTime(sizeof(struct AddressMapper_Entry) == AddressMapper_Entry_SIZE);

#define AddressMapper_MAX_ENTRIES 128
struct AddressMapper
{
    struct AddressMapper_Entry entries[AddressMapper_MAX_ENTRIES];
    struct AddressMapper_Entry* head;
};
#define AddressMapper_SIZE ((AddressMapper_Entry_SIZE * AddressMapper_MAX_ENTRIES) + sizeof(char*))
Assert_compileTime(sizeof(struct AddressMapper) == AddressMapper_SIZE);

/**
 * Allocate a new address map with the given allocator.
 * The map is automatically initialized.
 *
 * @param allocator the allocator to allocate the map from.
 * @return pointer to the newly created map.
 */
struct AddressMapper* AddressMapper_new(struct Allocator* allocator);

/**
 * Initialize the given address map, ready for use.
 *
 * @param map pointer to an address map to initialize.
 */
void AddressMapper_init(struct AddressMapper* map);

/* Searches for and returns the index of a map element with the the given label.
 *
 * @param label label to search for in the map.
 * @param map the map to search in.
 * @return the index of the element, or -1 if the label was not found.
 */
int AddressMapper_indexOf(uint64_t label, struct AddressMapper* map);

/**
 * Remove the address map element with the given index.
 *
 * @param index the index of the element to remove.
 * @param map the map to remove from.
 * @return 0 if the entry is removed, -1 if it could not be found.
 */
int AddressMapper_remove(int index, struct AddressMapper* map);

/**
 * Put a new element in the map.
 *
 * @param label the label of the new element.
 * @param address the address of the new element.
 * @param map the map to add to.
 */
int AddressMapper_put(uint64_t label, uint8_t address[16], struct AddressMapper* map);

#endif
