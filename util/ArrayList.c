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
#define ArrayList_NOCREATE
#include "util/ArrayList.h"
#include "util/Bits.h"
#include "util/QSort.h"

#include <stddef.h>
#include <stdlib.h>

struct ArrayList_pvt
{
    /** AckTung: The fields in ArrayList.h (struct ArrayList_NAME) must be reflected here first. */
    int length;

    int capacity;
    void** elements;
    struct Allocator* alloc;

    Identity
};

void* ArrayList_new(struct Allocator* alloc, int initialCapacity)
{
    struct ArrayList_pvt* l = Allocator_calloc(alloc, sizeof(struct ArrayList_pvt), 1);
    l->elements = Allocator_calloc(alloc, sizeof(char*), initialCapacity);
    l->capacity = initialCapacity;
    l->alloc = alloc;
    Identity_set(l);
    return l;
}

void* ArrayList_get(struct ArrayList* vlist, int number)
{
    struct ArrayList_pvt* list = Identity_check((struct ArrayList_pvt*) vlist);
    if (number >= list->length || number < 0) { return NULL; }
    return list->elements[number];
}

void* ArrayList_remove(struct ArrayList* vlist, int number)
{
    struct ArrayList_pvt* list = Identity_check((struct ArrayList_pvt*) vlist);
    if (number >= list->length) { return NULL; }
    void* out = list->elements[number];
    if (number < list->length - 1) {
        Bits_memmove(&list->elements[number],
                     &list->elements[number+1],
                     sizeof(char*) * (list->length - number - 1));
    }
    list->length--;
    return out;
}

int ArrayList_put(struct ArrayList* vlist, int number, void* val)
{
    struct ArrayList_pvt* list = Identity_check((struct ArrayList_pvt*) vlist);
    Assert_true(number >= 0 && number <= list->length);
    if (number >= list->capacity) {
        int capacity = list->capacity * 2;
        list->elements = Allocator_realloc(list->alloc, list->elements, capacity * sizeof(char*));
        for (int i = list->capacity; i < capacity; i++) {
            list->elements[i] = NULL;
        }
        list->capacity = capacity;
    }
    list->elements[number] = val;
    if (number == list->length) { list->length++; }
    return number;
}

void ArrayList_sort(struct ArrayList* vlist, int (* compare)(const void* a, const void* b))
{
    struct ArrayList_pvt* list = Identity_check((struct ArrayList_pvt*) vlist);
    QSort(list->elements, list->length, sizeof(char*), compare);
}

void* ArrayList_clone(struct ArrayList* vlist, struct Allocator* alloc)
{
    struct ArrayList_pvt* list = Identity_check((struct ArrayList_pvt*) vlist);
    struct ArrayList_pvt* newlist = Allocator_clone(alloc, list);
    newlist->elements = Allocator_malloc(alloc, list->capacity * sizeof(char*));
    Bits_memcpy(newlist->elements, list->elements, list->capacity * sizeof(char*));
    newlist->alloc = alloc;
    return newlist;
}
