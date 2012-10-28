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
#ifdef Map_USE_HASH
    #define Map_getKeySuffix(key) (Map_FUNCTION(hash)(key))
#else
    #define Map_getKeySuffix(key) (((uint32_t*) key)[(sizeof(Map_KEY_TYPE) / 4) - 1])
#endif

#define Map_GLUE(x, y) Map_GLUE2(x, y)
#define Map_GLUE2(x, y) x ## y


struct Map_CONTEXT
{
    #ifdef Map_ENABLE_KEYS
        uint32_t* keySuffixes;
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
    return allocator->clone(sizeof(struct Map_CONTEXT), allocator, &(struct Map_CONTEXT) {
        .allocator = allocator
    });
}

/**
 * This is a very hot loop,
 * a large amount of code relies on this being fast so it is a good target for optimization.
 */
#ifdef Map_ENABLE_KEYS
static inline int Map_FUNCTION(indexForKey)(Map_KEY_TYPE* key, struct Map_CONTEXT* map)
{
    uint32_t keySuffix = Map_getKeySuffix(key);
    for (uint32_t i = 0; i < map->count; i++) {
        if (map->keySuffixes[i] == keySuffix
            && memcmp(key, &map->keys[i], sizeof(Map_KEY_TYPE)) == 0)
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
    uint32_t* handles = map->handles;
    for (uint32_t lim = map->count; lim != 0; lim >>= 1) {
        uint32_t currentHandle = handles[lim >> 1];
        if (handle >= currentHandle) {
            if (currentHandle == handle) {
                return lim >> 1;
            }
            handles = &handles[(lim >> 1) + 1];
            lim--;
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
    if (index >= 0 && index < (int) map->count) {
        #ifdef Map_ENABLE_HANDLES
            // If we use handels then we need to keep the map sorted.
            #ifdef Map_ENABLE_KEYS
                memmove(&map->keySuffixes[index + 1],
                        &map->keySuffixes[index],
                        (map->count - index) * sizeof(uint32_t));

                memmove(&map->keys[index + 1],
                        &map->keys[index],
                        (map->count - index) * sizeof(Map_KEY_TYPE));
            #endif
            memmove(&map->handles[index],
                    &map->handles[index + 1],
                    (map->count - index) * sizeof(uint32_t));

            memmove(&map->values[index],
                    &map->values[index + 1],
                    (map->count - index) * sizeof(Map_VALUE_TYPE));

            map->count--;
        #else
            // No handles, we can just fold the top entry down on one to remove.
            map->count--;
            map->keySuffixes[index] = map->keySuffixes[map->count];
            Bits_memcpyConst(&map->keys[index], &map->keys[map->count], sizeof(Map_KEY_TYPE));
            Bits_memcpyConst(&map->values[index], &map->values[map->count], sizeof(Map_VALUE_TYPE));
        #endif
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
            map->keySuffixes =
                map->allocator->realloc(map->keySuffixes,
                                        sizeof(uint32_t) * (map->count + 10),
                                        map->allocator);
            map->keys =
                map->allocator->realloc(map->keys,
                                        sizeof(Map_KEY_TYPE) * (map->count + 10),
                                        map->allocator);
        #endif

        #ifdef Map_ENABLE_HANDLES
            map->handles =
                map->allocator->realloc(map->handles,
                                        sizeof(uint32_t) * (map->count + 10),
                                        map->allocator);
        #endif

        map->values =
            map->allocator->realloc(map->values,
                                    sizeof(Map_VALUE_TYPE) * (map->count + 10),
                                    map->allocator);

        map->capacity += 10;
    }

    int i = 0;

    #ifdef Map_ENABLE_KEYS
        i = Map_FUNCTION(indexForKey)(key, map);
    #endif

    if (i < 0) {
        i = map->count;
        map->count++;
    }

    #ifdef Map_ENABLE_KEYS
        map->keySuffixes[i] = Map_getKeySuffix(key);
        Bits_memcpyConst(&map->keys[i], key, sizeof(Map_KEY_TYPE));
    #endif
    #ifdef Map_ENABLE_HANDLES
        map->handles[i] = map->nextHandle++;
    #endif
    Bits_memcpyConst(&map->values[i], value, sizeof(Map_VALUE_TYPE));

    return i;
}

#undef Map
#undef Map_Entry
#undef Map_FUNCTION
#undef Map_getKeySuffix
#undef Map_USE_HASH
#undef Map_NAME
#undef Map_VALUE_TYPE
#undef Map_ENABLE_HANDLES
#undef Map_KEY_TYPE
#undef Map_ENABLE_KEYS
