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
#define ArrayList_H

#include "memory/Allocator.h"

#include "util/Linker.h"
Linker_require("util/ArrayList.c");

/*
 * This struct is never defined anywhere.
 * It's used in place to void* so that casts must be explicit to reduce likelyhood of error.
 */
struct ArrayList;

void* ArrayList_new(struct Allocator* alloc, int initialCapacity);
int ArrayList_add(struct ArrayList* list, void* val);
void* ArrayList_get(struct ArrayList* list, int number);
int ArrayList_put(struct ArrayList* list, int number, void* val);
void* ArrayList_remove(struct ArrayList* list, int num);
void ArrayList_sort(struct ArrayList* list, int (* compare)(const void* a, const void* b));
void* ArrayList_clone(struct ArrayList* vlist, struct Allocator* alloc);

#endif // Used multiple times...

#ifndef ArrayList_NOCREATE

#ifndef ArrayList_TYPE
    #error ArrayList_TYPE must be specified
#endif
#ifndef ArrayList_NAME
    #error ArrayList_NAME must be specified
#endif
#ifndef ArrayList_INITIAL_CAPACITY
    #define ArrayList_INITIAL_CAPACITY 8
#endif

#define ArrayList_FUNCTION(name) \
    ArrayList_GLUE(ArrayList_GLUE(ArrayList, ArrayList_NAME), name)
#define ArrayList_STRUCT ArrayList_GLUE(ArrayList, ArrayList_NAME)
#define ArrayList_GLUE(x,y) ArrayList_GLUE2(x,y)
#define ArrayList_GLUE2(x,y) x ## _ ## y

struct ArrayList_STRUCT {
    int length;
};

static inline struct ArrayList_STRUCT* ArrayList_FUNCTION(new)(struct Allocator* alloc)
{
    return (struct ArrayList_STRUCT*) ArrayList_new(alloc, ArrayList_INITIAL_CAPACITY);
}

static inline ArrayList_TYPE* ArrayList_FUNCTION(get)(struct ArrayList_STRUCT* list, int number)
{
    return (ArrayList_TYPE*) ArrayList_get((struct ArrayList*) list, number);
}

static inline int ArrayList_FUNCTION(put)(struct ArrayList_STRUCT* list,
                                          int number,
                                          ArrayList_TYPE* val)
{
    return ArrayList_put((struct ArrayList*) list, number, val);
}

static inline int ArrayList_FUNCTION(add)(struct ArrayList_STRUCT* list, void* val)
{
    return ArrayList_put((struct ArrayList*) list, list->length, val);
}

static inline ArrayList_TYPE* ArrayList_FUNCTION(shift)(struct ArrayList_STRUCT* list)
{
    return (ArrayList_TYPE*) ArrayList_remove((struct ArrayList*) list, 0);
}

static inline ArrayList_TYPE* ArrayList_FUNCTION(pop)(struct ArrayList_STRUCT* list)
{
    return (ArrayList_TYPE*) ArrayList_remove((struct ArrayList*) list, list->length - 1);
}

static inline ArrayList_TYPE* ArrayList_FUNCTION(remove)(struct ArrayList_STRUCT* list, int num)
{
    return (ArrayList_TYPE*) ArrayList_remove((struct ArrayList*) list, num);
}

#ifdef ArrayList_COMPARE
static inline int ArrayList_FUNCTION(sort_compare)(const void* a, const void* b)
{
    return ArrayList_COMPARE(((ArrayList_TYPE**)a)[0], ((ArrayList_TYPE**)b)[0]);
}
static inline void ArrayList_FUNCTION(sort)(struct ArrayList_STRUCT* list)
{
    ArrayList_sort((struct ArrayList*) list, ArrayList_FUNCTION(sort_compare));
}
#endif

/** Cloning the list does not clone the elements, just the pointers to them. */
static inline void* ArrayList_FUNCTION(clone)(struct ArrayList_STRUCT* l, struct Allocator* alloc)
{
    return ArrayList_clone((struct ArrayList*) l, alloc);
}

#undef ArrayList_TYPE
#undef ArrayList_NAME
#undef ArrayList_INITIAL_CAPACITY
#undef ArrayList_COMPARE
#undef ArrayList_FUNCTION
#undef ArrayList_STRUCT
#undef ArrayList_GLUE
#undef ArrayList_GLUE2

#endif // not defined ArrayList_NOCREATE
