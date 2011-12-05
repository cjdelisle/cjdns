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

    struct MemAllocator* allocator;
};

static inline struct InterfaceMap* InterfaceMap_new(size_t keySize,
                                                    struct MemAllocator* allocator)
{
    return allocator->clone(sizeof(struct InterfaceMap), allocator, &(struct InterfaceMap) {
        .keySize = keySize
    });
}

/**
 * This is a very hot loop,
 * a large amount of code relies on this being fast so it is a good target for optimization.
 */
static inline int InterfaceMap_indexOf(uint8_t* key, struct InterfaceMap* map)
{
    uint32_t keyPrefix;
    memcpy(&keyPrefix, key, 4);
    for (int i = 0; i < map->count; i++) {
        if (map->keyPrefixes[i] == keyPrefix
            && memcmp(key, map->keys[map->keySize * i], map->keySize) == 0)
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
    if (index >= 0 && index < map->count) {
        map->count--;
        if (index < map->count) {
            map->keyPrefixes[index] = map->keyPrefixes[count];
            memcpy(&map->keys[map->keySize * index],
                   &map->keys[map->keySize * map->count],
                   map->keySize);
            map->interfaces[index] = map->interfaces[count];
            map->lastMessageTimes[index] = map->lastMessageTimes[count];
        }
        return 0;
    }
    return -1;
}

static inline int InterfaceMap_put(uint8_t* key,
                                   struct Interface* interface,
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

    memcpy(&map->keyPrefixes[i], key, 4);
    memcpy(&map->keys[map->keySize * i], key, map->keySize);
    map->interfaces[i] = interface;
    map->lastMessageTimes[i] = timeOfLastMessage;

    return i;
}
