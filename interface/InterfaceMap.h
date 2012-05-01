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
#ifndef InterfaceMap_H
#define InterfaceMap_H

struct InterfaceMap_Entry;
struct InterfaceMap
{
    uint32_t* keyPrefixes;
    uint8_t* keys;
    struct Interface** interfaces;
    uint32_t* lastMessageTimes;

    uint32_t count;
    uint32_t capacity;

    size_t keySize;

    struct Allocator* allocator;
};

static inline struct InterfaceMap* InterfaceMap_new(size_t keySize,
                                                    struct Allocator* allocator)
{
    return allocator->clone(sizeof(struct InterfaceMap), allocator, &(struct InterfaceMap) {
        .keySize = keySize,
        .allocator = allocator
    });
}

/**
 * This is a very hot loop,
 * a large amount of code relies on this being fast so it is a good target for optimization.
 */
static inline int InterfaceMap_indexOf(uint8_t* key, struct InterfaceMap* map)
{
    uint32_t keyPrefix;
    Bits_memcpyConst(&keyPrefix, key, 4);
    for (uint32_t i = 0; i < map->count; i++) {
        if (map->keyPrefixes[i] == keyPrefix
            && memcmp(key, &map->keys[map->keySize * i], map->keySize) == 0)
        {
            return i;
        }
    }
    return -1;
}

/**
 * @param key the key of the entry to remove.
 * @param map the map to remove from.
 * @return 0 if the entry is removed, -1 if it could not be found.
 */
static inline int InterfaceMap_remove(int index, struct InterfaceMap* map)
{
    if (index >= 0 && index < (int) map->count) {
        map->count--;
        if (index < (int) map->count) {
            map->keyPrefixes[index] = map->keyPrefixes[map->count];
            Bits_memcpy(&map->keys[map->keySize * index],
                        &map->keys[map->keySize * map->count],
                        map->keySize);
            map->interfaces[index] = map->interfaces[map->count];
            map->lastMessageTimes[index] = map->lastMessageTimes[map->count];
        }
        return 0;
    }
    return -1;
}

static inline int InterfaceMap_put(uint8_t* key,
                                   struct Interface* interface,
                                   uint32_t timeOfLastMessage,
                                   struct InterfaceMap* map)
{
    if (map->count == map->capacity) {
        map->keyPrefixes =
            map->allocator->realloc(map->keyPrefixes,
                                    sizeof(uint32_t) * (map->count + 10),
                                    map->allocator);
        map->keys =
            map->allocator->realloc(map->keys,
                                    map->keySize * (map->count + 10),
                                    map->allocator);
        map->interfaces =
            map->allocator->realloc(map->interfaces,
                                    sizeof(char*) * (map->count + 10),
                                    map->allocator);
        map->lastMessageTimes =
            map->allocator->realloc(map->lastMessageTimes,
                                    sizeof(uint32_t) * (map->count + 10),
                                    map->allocator);
        map->capacity += 10;
    }

    int i = InterfaceMap_indexOf(key, map);
    if (i < 0) {
        i = map->count;
        map->count++;
    }

    Bits_memcpyConst(&map->keyPrefixes[i], key, 4);
    Bits_memcpy(&map->keys[map->keySize * i], key, map->keySize);
    map->interfaces[i] = interface;
    map->lastMessageTimes[i] = timeOfLastMessage;

    return i;
}

#endif
