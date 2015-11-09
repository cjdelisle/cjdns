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
#ifndef Map_H
#define Map_H
#endif // This header can be used multiple times as long as the name is different.

#include "util/Bits.h"


#if defined(Map_KEY_TYPE)
    Assert_compileTime(!(sizeof(Map_KEY_TYPE) % 4));
    #define Map_ENABLE_KEYS
#elif !defined(Map_ENABLE_HANDLES)
    #error must define Map_KEY_TYPE or Map_ENABLE_HANDLES or both
#endif
#ifndef Map_VALUE_TYPE
    #error must define Map_VALUE_TYPE
#endif
#ifndef Map_NAME
    #error must give this map type a name by defining Map_NAME
#endif

#define Map_CONTEXT Map_GLUE(Map_, Map_NAME)
#define Map_FUNCTION(name) Map_GLUE(Map_GLUE(Map_GLUE(Map_, Map_NAME),_), name)
#define Map_GLUE(x, y) Map_GLUE2(x, y)
#define Map_GLUE2(x, y) x ## y

#ifdef Map_ENABLE_KEYS
    // Hashcode calculator.
    static inline uint32_t Map_FUNCTION(hash)(Map_KEY_TYPE* key);
    #ifndef Map_USE_HASH
        #include "util/Hash.h"
        // Get the last 4 bytes of the key by default.
        static inline uint32_t Map_FUNCTION(hash)(Map_KEY_TYPE* key)
        {
            return Hash_compute((uint8_t*)key, sizeof(Map_KEY_TYPE));
        }
    #endif


    static inline int Map_FUNCTION(compare)(Map_KEY_TYPE* keyA, Map_KEY_TYPE* keyB);
    #ifndef Map_USE_COMPARATOR
        // Get the last 4 bytes of the key by default.
        static inline int Map_FUNCTION(compare)(Map_KEY_TYPE* keyA, Map_KEY_TYPE* keyB)
        {
            return Bits_memcmp(keyA, keyB, sizeof(Map_KEY_TYPE));
        }
    #endif
#endif

struct Map_CONTEXT
{
    #ifdef Map_ENABLE_KEYS
        uint32_t* hashCodes;
        Map_KEY_TYPE* keys;
    #endif

    #ifdef Map_ENABLE_HANDLES
        uint32_t* handles;
        uint32_t nextHandle;
    #endif

    Map_VALUE_TYPE* values;

    uint32_t count;
    uint32_t capacity;

    struct Allocator* allocator;
};

static inline struct Map_CONTEXT* Map_FUNCTION(new)(struct Allocator* allocator)
{
    return Allocator_clone(allocator, (&(struct Map_CONTEXT) {
        .allocator = allocator
    }));
}

/**
 * This is a very hot loop,
 * a large amount of code relies on this being fast so it is a good target for optimization.
 */
#ifdef Map_ENABLE_KEYS
static inline int Map_FUNCTION(indexForKey)(Map_KEY_TYPE* key, struct Map_CONTEXT* map)
{
    uint32_t hashCode = (Map_FUNCTION(hash)(key));
    for (uint32_t i = 0; i < map->count; i++) {
        if (map->hashCodes[i] == hashCode
            && Map_FUNCTION(compare)(key, &map->keys[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}
#endif

#ifdef Map_ENABLE_HANDLES
static inline int Map_FUNCTION(indexForHandle)(uint32_t handle, struct Map_CONTEXT* map)
{
    uint32_t base = 0;
    for (uint32_t bufferLen = map->count; bufferLen != 0; bufferLen /= 2) {
        uint32_t currentHandle = map->handles[base + (bufferLen / 2)];
        if (handle >= currentHandle) {
            if (currentHandle == handle) {
                return base + (bufferLen / 2);
            }
            base += (bufferLen / 2) + 1;
            bufferLen--;
        }
    }
    return -1;
}
#endif

/**
 * @param key the key of the entry to remove.
 * @param map the map to remove from.
 * @return 0 if the entry is removed, -1 if it could not be found.
 */
static inline int Map_FUNCTION(remove)(int index, struct Map_CONTEXT* map)
{
    if (index >= 0 && index < (int) map->count - 1) {
        #ifdef Map_ENABLE_HANDLES
            // If we use handels then we need to keep the map sorted.
            #ifdef Map_ENABLE_KEYS
                Bits_memmove(&map->hashCodes[index],
                             &map->hashCodes[index + 1],
                             (map->count - index - 1) * sizeof(uint32_t));

                Bits_memmove(&map->keys[index],
                             &map->keys[index + 1],
                             (map->count - index - 1) * sizeof(Map_KEY_TYPE));
            #endif
            Bits_memmove(&map->handles[index],
                         &map->handles[index + 1],
                         (map->count - index - 1) * sizeof(uint32_t));

            Bits_memmove(&map->values[index],
                         &map->values[index + 1],
                         (map->count - index - 1) * sizeof(Map_VALUE_TYPE));

            map->count--;
        #else
            // No handles, we can just fold the top entry down on one to remove.
            map->count--;
            map->hashCodes[index] = map->hashCodes[map->count];
            Bits_memcpy(&map->keys[index], &map->keys[map->count], sizeof(Map_KEY_TYPE));
            Bits_memcpy(&map->values[index], &map->values[map->count], sizeof(Map_VALUE_TYPE));
        #endif
        return 0;
    } else if (index == (int) map->count - 1) {
        map->count--;
        return 0;
    }
    return -1;
}

#ifdef Map_ENABLE_KEYS
static inline int Map_FUNCTION(put)(Map_KEY_TYPE* key,
                                    Map_VALUE_TYPE* value,
                                    struct Map_CONTEXT* map)
#else
static inline int Map_FUNCTION(put)(Map_VALUE_TYPE* value,
                                    struct Map_CONTEXT* map)
#endif
{
    if (map->count == map->capacity) {
        #ifdef Map_ENABLE_KEYS
            map->hashCodes = Allocator_realloc(map->allocator,
                                               map->hashCodes,
                                               sizeof(uint32_t) * (map->count + 10));
            map->keys = Allocator_realloc(map->allocator,
                                          map->keys,
                                          sizeof(Map_KEY_TYPE) * (map->count + 10));
        #endif

        #ifdef Map_ENABLE_HANDLES
            map->handles = Allocator_realloc(map->allocator,
                                             map->handles,
                                             sizeof(uint32_t) * (map->count + 10));
        #endif

        map->values = Allocator_realloc(map->allocator,
                                        map->values,
                                        sizeof(Map_VALUE_TYPE) * (map->count + 10));

        map->capacity += 10;
    }

    int i = -1;

    #ifdef Map_ENABLE_KEYS
        i = Map_FUNCTION(indexForKey)(key, map);
    #endif

    if (i < 0) {
        i = map->count;
        map->count++;
        #ifdef Map_ENABLE_HANDLES
            map->handles[i] = map->nextHandle++;
        #endif
        #ifdef Map_ENABLE_KEYS
            map->hashCodes[i] = (Map_FUNCTION(hash)(key));
            Bits_memcpy(&map->keys[i], key, sizeof(Map_KEY_TYPE));
        #endif
    }

    Bits_memcpy(&map->values[i], value, sizeof(Map_VALUE_TYPE));

    return i;
}

#undef Map
#undef Map_Entry
#undef Map_FUNCTION
#undef Map_USE_HASH
#undef Map_NAME
#undef Map_VALUE_TYPE
#undef Map_ENABLE_HANDLES
#undef Map_KEY_TYPE
#undef Map_ENABLE_KEYS
#undef Map_USE_COMPARATOR
