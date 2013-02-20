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

#include "util/Bits.h"
#include "memory/Allocator.h"
#include "AddressMapper.h"

#include <stdint.h>
#include "util/platform/libc/string.h"
#include <stdlib.h>

/* if set, address mapper linked list entries are validated before use */
#define ENTRY_VALIDATION

/* convenience macro for entryGet() that assumes the pointer to
 * the map is in scope
 */
#define ENTRY(e) entryGet(e, map)

/* gets the index of a linked list entry with respect to the
 * start of the element array
 */
#define entryIndexOf(map, entry) ((entryGet(entry, map)) - &(map)->entries[0])


void AddressMapper_init(struct AddressMapper* map)
{
    /* initialize the doubly linked list */

    for (int i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        map->entries[i].prev = &map->entries[i - 1];
        map->entries[i].next = &map->entries[i + 1];
    }

    map->entries[0].prev = &map->entries[AddressMapper_MAX_ENTRIES - 1];
    map->entries[AddressMapper_MAX_ENTRIES - 1].next = &map->entries[0];

    /* set the initial head */
    map->head = &map->entries[0];
}

struct AddressMapper* AddressMapper_new(struct Allocator* allocator)
{
    struct AddressMapper* map;

    map = Allocator_calloc(allocator, sizeof(struct AddressMapper), 1);
    AddressMapper_init(map);

    return map;
}

/* validate the given address mapper entry to detect linked list corruption then
 * return a pointer to itself.
 * aborts on error
 */
static inline struct AddressMapper_Entry* entryGet(struct AddressMapper_Entry* entry,
                                                                struct AddressMapper* map)
{
#ifdef ENTRY_VALIDATION
    if ((entry == NULL) ||
        (entry < &map->entries[0]) ||
        (entry > &map->entries[AddressMapper_MAX_ENTRIES - 1]) ||
        (((((uintptr_t) entry) - (uintptr_t) &map->entries[0])
                    % sizeof(struct AddressMapper_Entry)) != 0)) {
            fprintf(stderr, "AddressMapper: linked list corruption detected.\n");
            abort();
    }
#endif
    return entry;
}

/**
 * Move the given entry to the tail of the linked list
 */
static inline void moveEntryToTail(struct AddressMapper_Entry* entry, struct AddressMapper* map)
{
    if (ENTRY(entry) == ENTRY(map->head)) {
        map->head = ENTRY(ENTRY(map->head)->next);
    } else {
        /* detach entry */
        ENTRY(ENTRY(entry)->prev)->next = ENTRY(ENTRY(entry)->next);
        ENTRY(ENTRY(entry)->next)->prev = ENTRY(ENTRY(entry)->prev);

        /* insert at the tail */
        ENTRY(ENTRY(map->head)->prev)->next = ENTRY(entry);
        ENTRY(entry)->prev = ENTRY(ENTRY(map->head)->prev);
        ENTRY(entry)->next = ENTRY(map->head);
        ENTRY(map->head)->prev = ENTRY(entry);
    }
}

/**
 * Move the given entry to the head of the linked list.
 *
 * Unlike moveEntryToTail, this function has an optimisation for
 * moving the tail entry to the head, since this is a common case
 */
static inline void moveEntryToHead(struct AddressMapper_Entry* entry, struct AddressMapper* map)
{
    if (ENTRY(entry) != ENTRY(map->head)) {
        if (ENTRY(entry) != ENTRY(ENTRY(map->head)->prev)) {
            /* detach entry */
            ENTRY(entry->prev)->next = ENTRY(entry->next);
            ENTRY(entry->next)->prev = ENTRY(entry->prev);

            /* insert at the tail */
            ENTRY(ENTRY(map->head)->prev)->next = ENTRY(entry);
            ENTRY(entry)->prev = ENTRY(ENTRY(map->head)->prev);
            ENTRY(entry)->next = ENTRY(map->head);
            ENTRY(map->head)->prev = ENTRY(entry);
        }
        map->head = ENTRY(entry);
    }
}

/**
 * This is a very hot loop,
 * a large amount of code relies on this being fast so it is a good target for optimization.
 */
int AddressMapper_indexOf(uint64_t label, struct AddressMapper* map)
{
    struct AddressMapper_Entry* entry;

    /* search from head to tail */
    entry = ENTRY(map->head);

    do {
        if (entry->label == label) {
            /* move entry to head */
            moveEntryToHead(entry, map);
            return entryIndexOf(map, entry);
        }
        entry = ENTRY(entry->next);
    } while (ENTRY(entry) != ENTRY(map->head));

    return -1;
}

int AddressMapper_remove(int index, struct AddressMapper* map)
{
    if ((index >= 0) && (index < AddressMapper_MAX_ENTRIES)) {
        struct AddressMapper_Entry* entry;
        /* move entry to tail */
        entry = ENTRY(&map->entries[index]);
        moveEntryToTail(entry, map);

        /* invalidate the entry */
        ENTRY(entry)->label = 0L;

        return 0;
    }

    return -1;
}

int AddressMapper_put(uint64_t label, uint8_t address[16], struct AddressMapper* map)
{
    struct AddressMapper_Entry* tail;

    tail = ENTRY(ENTRY(map->head)->prev);
    Bits_memcpyConst(tail->address, address, 16);
    ENTRY(tail)->label = label;
    map->head = tail;

    return entryIndexOf(map, tail);
}
