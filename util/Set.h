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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef Set_H
#define Set_H

#include "memory/Allocator.h"
#include "util/Bits.h"
#include "util/UniqueName.h"

#include "util/Linker.h"
Linker_require("util/Set.c");

/*
 * This struct is never defined anywhere.
 * It's used in place to void* so that casts must be explicit to reduce likelyhood of error.
 */
struct Set;
struct Set_Iter;

typedef int (* Set_Compare_t)(const void* a, const void* b);
typedef uint32_t (* Set_HashCode_t)(const void* a);

int Set_add(struct Set* as, void* val);
void* Set_remove(struct Set* as, void* val);
void* Set_get(struct Set* as, void* val);
struct Set* Set_new(struct Allocator* alloc, Set_HashCode_t, Set_Compare_t);
void Set_iter(struct Set* _set, struct Set_Iter* iter);
void Set_iterNext(struct Set_Iter* iter);
int Set_addCopy(struct Set* _set, void* val, uint32_t size);

#define Set_FOREACH(name, set, out) \
    struct Set_ ## name ## _Iter UniqueName_get();             \
    for (Set_ ## name ## _iter(set, &UniqueName_last());       \
        ((out) = UniqueName_last().val);                       \
        Set_ ## name ## _iterNext(&UniqueName_last()))
// CHECKFILES_IGNORE expecting a {

#endif // Used multiple times...

#ifndef Set_NOCREATE

#ifndef Set_TYPE
    #error Set_TYPE must be specified
#endif
#ifndef Set_NAME
    #ifndef Set_FULLNAME
        #error Set_NAME must be specified
    #endif
#else
    #define Set_FULLNAME Set_GLUE(Set, Set_NAME)
#endif

#define Set_FUNCTION(name) \
    Set_GLUE(Set_FULLNAME, name)
#define Set_GLUE(x,y) Set_GLUE2(x,y)
#define Set_GLUE2(x,y) x ## _ ## y

struct Set_FULLNAME {
    int size;
};

struct Set_FUNCTION(Iter)
{
    Set_TYPE* val;
    void* internal;
};

#ifdef Set_COMPARE
#ifndef Set_HASHCODE
    #error cannot specify Set_COMPARE without Set_HASHCODE
#endif
static inline uint32_t Set_FUNCTION(_hashCode)(const void* a)
{
    return Set_HASHCODE((Set_TYPE*)a);
}
static inline int Set_FUNCTION(_compare)(const void* a, const void* b)
{
    return Set_COMPARE((Set_TYPE*)a, (Set_TYPE*)b);
}
#else
#ifdef Set_HASHCODE
    #error cannot specify Set_HASHCODE without Set_COMPARE
#endif
#include "util/Hash.h"
static inline uint32_t Set_FUNCTION(_hashCode)(const void* a)
{
    return Hash_compute((uint8_t*) a, sizeof(Set_TYPE));
}
static inline int Set_FUNCTION(_compare)(const void* a, const void* b)
{
    return Bits_memcmp(a, b, sizeof(Set_TYPE));
}
#endif

static inline struct Set_FULLNAME* Set_FUNCTION(new)(struct Allocator* alloc)
{
    return (struct Set_FULLNAME*) Set_new(alloc, Set_FUNCTION(_hashCode), Set_FUNCTION(_compare));
}

static inline Set_TYPE* Set_FUNCTION(get)(struct Set_FULLNAME* list, Set_TYPE* val)
{
    return (Set_TYPE*) Set_get((struct Set*) list, (void*)val);
}

static inline int Set_FUNCTION(addCopy)(struct Set_FULLNAME* list, Set_TYPE* val)
{
    return Set_addCopy((struct Set*) list, (void*) val, sizeof(Set_TYPE));
}

static inline int Set_FUNCTION(add)(struct Set_FULLNAME* list, Set_TYPE* val)
{
    return Set_add((struct Set*) list, (void*) val);
}

static inline void Set_FUNCTION(iter)(struct Set_FULLNAME* set, struct Set_FUNCTION(Iter)* iter)
{
    Set_iter((struct Set*) set, (struct Set_Iter*) iter);
}

static inline void Set_FUNCTION(iterNext)(struct Set_FUNCTION(Iter)* iter)
{
    Set_iterNext((struct Set_Iter*) iter);
}

static inline Set_TYPE* Set_FUNCTION(remove)(struct Set_FULLNAME* list, Set_TYPE* val)
{
    return (Set_TYPE*) Set_remove((struct Set*) list, (void*) val);
}

#undef Set_TYPE
#undef Set_NAME
#undef Set_FULLNAME
#undef Set_INITIAL_CAPACITY
#undef Set_COMPARE
#undef Set_FUNCTION
#undef Set_FULLNAME
#undef Set_GLUE
#undef Set_GLUE2
#undef Set_HASHCODE
#undef Set_COMPARE

#endif // not defined Set_NOCREATE
