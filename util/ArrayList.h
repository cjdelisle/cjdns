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
#ifndef ArrayList_H

#include "memory/Allocator.h"

#include "util/Linker.h"
Linker_require("util/ArrayList.c")

struct ArrayList
{
    int length;
};

void* ArrayList_new(struct Allocator* alloc, int initialCapacity);
int ArrayList_add(void* list, void* val);
void* ArrayList_get(void* list, int number);
int ArrayList_put(void* list, int number, void* val);

#define ArrayList_H
#endif // Used multiple times...

#ifndef ArrayList_TYPE
    #error ArrayList_TYPE must be specified
#endif
#ifndef ArrayList_NAME
    #error ArrayList_NAME must be specified
#endif
#ifndef ArrayList_INITIAL_CAPACITY
    #define ArrayList_INITIAL_CAPACITY 8
#endif

#define ArrayList_FUNCTION(name) ArrayList ## _ ## ArrayList_NAME ## _ ## name

struct ArrayList_NAME { struct ArrayList list };

static inline struct ArrayList_NAME* ArrayList_FUNCTION(new)(struct Allocator* alloc)
{
    return (struct ArrayList_NAME*) ArrayList_new(alloc, ArrayList_INITIAL_CAPACITY);
}

static inline ArrayList_TYPE* ArrayList_FUNCTION(get)(struct ArrayList_NAME* list, int number)
{
    return (ArrayList_TYPE*) ArrayList_get((void*) list, number);
}

static inline int ArrayList_FUNCTION(put)(struct ArrayList_NAME* list,
                                          int number,
                                          ArrayList_TYPE* val)
{
    return ArrayList_put((void*) list, number, val);
}

static inline int ArrayList_FUNCTION(add)(struct ArrayList_NAME* list, void* val)
{
    return ArrayList_put((void*) list, list->length, val);
}

#undef ArrayList_TYPE
#undef ArrayList_NAME
#undef ArrayList_INITIAL_CAPACITY
#undef ArrayList_FUNCTION
