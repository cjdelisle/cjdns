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

#include "util/Bits.h"

#include <assert.h>
#include <inttypes.h>
#include <string.h>

/** Address mapper doubly linked list LRU.
 * The most recently used item is on the head.
 * The least recently used item is on the tail.
 *
 * When an item is used, it is relocated to the head. Searching from the
 * head therefore means items will be checked in most-recently-used order.
 *
 * When inserting a new item, the item to replace - the least-recently-used item - is simply the tail.
 * Then the tail is set to be the head, because it is the most recently used item.
 */
struct AddressMapperEntry
{
    uint64_t label;
    uint8_t address[16];
    struct AddressMapperEntry *next;
    struct AddressMapperEntry *prev;
};

/* gets the index of a linked list entry with respect to the
 * start of the element array
 */
#define Entry_indexOf(map, entry) ((entry) - &(map)->entries[0])

#define AddressMapper_MAX_ENTRIES 128
struct AddressMapper
{
    struct AddressMapperEntry entries[AddressMapper_MAX_ENTRIES];
    struct AddressMapperEntry *head;
    uint8_t canary[3];
};

static inline void AddressMapper_init(struct AddressMapper *map)
{
    /* initialise the doubly linked list */

    for(int i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        map->entries[i].prev = &map->entries[i - 1];
        map->entries[i].next = &map->entries[i + 1];
    }

    map->entries[0].prev = &map->entries[AddressMapper_MAX_ENTRIES - 1];
    map->entries[AddressMapper_MAX_ENTRIES - 1].next = &map->entries[0];

    /* set the initial head */
    map->head = &map->entries[0];
}

static inline struct AddressMapper* AddressMapper_new(struct Allocator* allocator)
{
    struct AddressMapper* map;

    map = allocator->calloc(sizeof(struct AddressMapper), 1, allocator);
    AddressMapper_init(map);

    return map;
}

/**
 * Move the given entry to the tail of the linked list
 */
static inline void Entry_moveToTail(struct AddressMapperEntry *entry, struct AddressMapper *map)
{
    if(entry == map->head) {
        map->head = map->head->next;
    } else {
        /* detach entry */
        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;

        /* insert at the tail */
        map->head->prev->next = entry;
        entry->prev = map->head->prev;
        entry->next = map->head;
        map->head->prev = entry;
    }
}

/**
 * Move the given entry to the head of the linked list.
 *
 * Unlike Entry_moveToTail, this function has an optimisation for
 * moving the tail entry to the head, since this is a common case
 */
static inline void Entry_moveToHead(struct AddressMapperEntry *entry, struct AddressMapper *map)
{
    if(entry != map->head) {
        if(entry != map->head->prev) {
            /* detach entry */
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev;

            /* insert at the tail */
            map->head->prev->next = entry;
            entry->prev = map->head->prev;
            entry->next = map->head;
            map->head->prev = entry;
        }
        map->head = entry;
    }
}

/**
 * This is a very hot loop,
 * a large amount of code relies on this being fast so it is a good target for optimization.
 */
static inline int AddressMapper_indexOf(uint64_t label, struct AddressMapper* map)
{
    struct AddressMapperEntry* entry;

    /* search from head to tail */
    entry = map->head;

    do {
        if(entry->label == label) {
            /* move entry to head */
            Entry_moveToHead(entry, map);
            return Entry_indexOf(map, entry);
        }
        entry = entry->next;
    } while(entry != map->head);

    return -1;
}

/**
 * @param index the index of the element to remove.
 * @param map the map to remove from.
 * @return 0 if the entry is removed, -1 if it could not be found.
 */
static inline int AddressMapper_remove(int index, struct AddressMapper* map)
{
    if((index >= 0) && (index < AddressMapper_MAX_ENTRIES)) {
        struct AddressMapperEntry* entry;
        /* move entry to tail */
        entry = &map->entries[index];
        Entry_moveToTail(entry, map);

        /* invalidate the entry */
        entry->label = 0L;

        return 0;
    }

    return -1;
}

/**
 * Put a new element in the map
 * @param label the label of the new element
 * @param address the address of the new element
 * @param map the map to add to
 */
static inline int AddressMapper_put(uint64_t label, uint8_t address[16], struct AddressMapper* map)
{
    struct AddressMapperEntry *tail;

    tail = map->head->prev;
    Bits_memcpyConst(tail->address, address, 16);
    tail->label = label;
    map->head = tail;

    assert(!memcmp(map->canary, "\0\0\0", 3));
    return Entry_indexOf(map, tail);
}

#endif

