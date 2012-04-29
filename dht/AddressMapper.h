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
 * Then the tail is made the head, because it is the most recently used item.
 */
struct AddressMapperEntry
{
    uint64_t label;
    uint8_t address[16];
    struct AddressMapperEntry *next;
    struct AddressMapperEntry *prev;
};

/* macro gets the index of a linked list entry (with respect to the
 * start of the element array)
 */
#define Entry_indexOf(map, entry) ((entry) - &(map)->entries[0])

#define AddressMapper_MAX_ENTRIES 128
struct AddressMapper
{
    struct AddressMapperEntry entries[AddressMapper_MAX_ENTRIES];
    struct AddressMapperEntry *head;
    uint8_t accessNumber;
    uint8_t canary[3];
};

//#define LL_LRU_VALIDATE
#ifdef LL_LRU_VALIDATE
#include <stdio.h> /* XXX: removeme */

#define validate() validateLinkedList(map)

static void printLinkedList(struct AddressMapper *map)
{
 //   unsigned int entryList[AddressMapper_MAX_ENTRIES];
    struct AddressMapperEntry *entry;
    unsigned int n = 0;

    entry = map->head;

    printf("lbl>[");
    do {
//           entryList[n] = Entry_indexOf(map, entry);
           printf("%llu", (unsigned long long) entry->label);
        n++;
        entry = entry->next;
        if(entry != map->head) {
            printf(", ");
        }
    } while(entry != map->head);
    printf("]>\n");
    printf("total: %u\n", n);

    entry = map->head->prev;

    printf("lbl<[");
    do {
           printf("%llu", (unsigned long long) entry->label);
        n++;
        entry = entry->prev;
        if(entry != map->head->prev) {
            printf(", ");
        }
    } while(entry != map->head->prev);
    printf("]<\n");

#if 0
    printf("idx>[");
    for(unsigned int i = 0; i < n; i++) {
        printf("%u", entryList[i]);
        if(i < (n - 1)) {
            printf(", ");
        }
    }
    printf("]>\n");
#endif
}

static void validateError(unsigned int idx)
{
    fprintf(stderr, "Validation error: %u\n", idx);
    abort();
}

static void validateLinkedList(struct AddressMapper* map)
{
    uint8_t hitlist[AddressMapper_MAX_ENTRIES];
    struct AddressMapperEntry *entry;
    unsigned int count;

    /* validate forwards */

    memset(hitlist, 0, AddressMapper_MAX_ENTRIES);

    entry = map->head;
    count = 0;

    do {
        hitlist[Entry_indexOf(map, entry)] = 1;

        entry = entry->next;
        count++;
    } while(entry != map->head);

    assert(count == AddressMapper_MAX_ENTRIES);

    for(unsigned int i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        if(hitlist[i] == 0) {
            validateError(i);
        }
    }

    /* validate backwards */

    memset(hitlist, 0, AddressMapper_MAX_ENTRIES);

    entry = map->head->prev;

    do {
        hitlist[Entry_indexOf(map, entry)] = 1;

        entry = entry->prev;
    } while(entry != map->head->prev);

    for(unsigned int i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        if(hitlist[i] == 0) {
            validateError(i);
        }
    }
}
#else
#define validate()
#endif

static inline struct AddressMapper* AddressMapper_new(struct Allocator* allocator)
{
    struct AddressMapper* map;

    map = allocator->calloc(sizeof(struct AddressMapper), 1, allocator);

    /* initialise the doubly linked list */

    map->entries[0].prev = &map->entries[AddressMapper_MAX_ENTRIES - 1];
    map->entries[0].next = &map->entries[1];

    int i;
    for(i = 1; i < AddressMapper_MAX_ENTRIES - 1; i++) {
        map->entries[i].prev = &map->entries[i - 1];
        map->entries[i].next = &map->entries[i + 1];
    }

    map->entries[AddressMapper_MAX_ENTRIES - 1].prev = &map->entries[AddressMapper_MAX_ENTRIES - 2];
    map->entries[AddressMapper_MAX_ENTRIES - 1].next = &map->entries[0];

    /* set the initial head */
    map->head = &map->entries[0];

    validate();
#ifdef LL_LRU_VALIDATE
    printLinkedList(map);
#endif

    return map;
}

static inline void Entry_detach(struct AddressMapperEntry *entry, struct AddressMapper *map)
{
        /* detach entry */
        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;
        if(entry == map->head) {
            map->head = map->head->next;
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
    entry = &map->head[0];

    do {
        if(entry->label == label) {
            Entry_detach(entry, map);

            /* move to head */

            map->head->prev->next = entry;
            entry->prev = map->head->prev;

            entry->next = map->head;
            map->head->prev = entry;

            map->head = entry;

            validate();
            return Entry_indexOf(map, entry);
        }
        entry = entry->next;
    } while(entry != map->head);

    validate();
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

        entry = &map->entries[index];
        Entry_detach(entry, map);

        /* move to tail */

        map->head->prev->next = entry;
        entry->prev = map->head->prev;

        entry->next = map->head;
        map->head->prev = entry;

        /* invalidate the entry */
        entry->label = 0L;

        validate();
        return 0;
    }

    validate();
    return -1;
}

static inline int AddressMapper_put(uint64_t label, uint8_t address[16], struct AddressMapper* map)
{
    struct AddressMapperEntry *tail;

    tail = map->head->prev;
    memcpy(tail->address, address, 16);
    tail->label = label;
    map->head = tail;

    assert(!memcmp(map->canary, "\0\0\0", 3));
    validate();
    return Entry_indexOf(map, tail);
}

#endif

