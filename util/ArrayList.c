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

#include <stddef.h>

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

void* ArrayList_get(void* vlist, int number)
{
    struct ArrayList_pvt* list = Identity_check((struct ArrayList_pvt*) vlist);
    if (number >= list->length || number < 0) { return NULL; }
    return list->elements[number];
}

void* ArrayList_shift(void* vlist)
{
    struct ArrayList_pvt* list = Identity_check((struct ArrayList_pvt*) vlist);
    if (!list->length) { return NULL; }
    void* out = list->elements[0];
    list->length--;
    if (list->length) {
        Bits_memmove(list->elements, &list->elements[1], sizeof(char*) * list->length);
    }
    return out;
}

int ArrayList_put(void* vlist, int number, void* val)
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
