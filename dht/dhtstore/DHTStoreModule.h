#ifndef DHT_STORE_MODULE_H
#define DHT_STORE_MODULE_H

#include "dht/DHTModules.h"
#include "libbenc/benc.h"


struct DHTStoreEntry
{
    /** The entire lookup key. */
    char* key;

    char* value;

    /** Additional data whose type is specific to the module which handles it. */
    void* additional;

    /** An allocator which may be freed when this entry is nolonger valid. */
    struct MemAllocator* allocator;

    uint16_t length;
};

/**
 * A tool which when passed to the DHT storage modules allows them to lookup entries of their type.
 */
struct DHTStoreTool;
struct DHTStoreTool
{
    /**
     * A function which when passed the tool and a lookup key will do a lookup.
     *
     * @param key the key for the entry to lookup.
     * @param storeTool this tool.
     * @return a store entry if one exists, otherwise NULL.
     */
    struct DHTStoreEntry* (* const lookup)(const String* key,
                                           const struct DHTStoreTool* storeTool);

    /** An internal context used by the lookup tool. This is a DHTStoreToolContext struct. */
    void* storeToolContext;
};

/**
 * Each type of storage must register one of these with the DHTStorageModule.
 * This is a storage provider but not a storage client. The storage itself must
 * be used for the client.
 */
struct DHTStoreModule
{
    /** The name of the module, mostly for debugging. */
    const char* name;

    /** The query type which is used to get data in this storage type. */
    const String getQuery;

    /** The query type which is used to put data in this storage type. */
    const String putQuery;

    /** The length of the key for looking up the entry. */
    const uint16_t keySize;

    void* const context;

    /**
     * If the module has a way to determine that an entry is a reannouncement of an old entry then
     * this function should get the date when the entry was first announced. Otherwise this should be NULL.
     */
    int64_t (* const getDate)(struct DHTStoreEntry* entry);

    void (* const handleGetRequest)(struct DHTMessage* replyMessage,
                                    const struct DHTStoreEntry* entry,
                                    void* vcontext);

    struct DHTStoreEntry* (* const handlePutRequest)(const struct DHTMessage* incomingMessage,
                                                     Dict** replyMessagePointer,
                                                     const struct DHTStoreTool* storeTool,
                                                     void* vcontext,
                                                     const struct MemAllocator* messageAllocator);

    /**
     * This is set when loading the module so that it can be referenced by a unique number.
     * It should not be set by the module itself and will be cleared if it is.
     * TODO: Remove this and make a wrapper with it in the DHTStoreModule.
     */
    uint8_t type;
};

/**
 * A registry where modules are registered for handling each different type of storage.
 */
struct DHTStoreRegistry
{
    /**
     * Register a new store module.
     *
     * @param toRegister the module to register.
     * @param thisRegistry the DHTStoreRegistry which contains the function.
     */
    void (* const registerModule)(const struct DHTStoreModule* toRegister,
                                  struct DHTStoreRegistry* thisRegistry);

    /**
     * Memory allocator for getting space for entries and keys.
     * This should be used by all store modules for private data.
     */
    const struct MemAllocator* const allocator;

    /** The first module in a linked list. */
    struct DHTStoreModuleWrapper* firstModule;
};

/**
 * Create a new DHT Store module.
 *
 * @param maxEntries the maximum number of entries which are allowed
 *                   before old entries will be replaced or entries will be rejected.
 * @param registry the DHT module registry.
 * @param allocator the means to acquire memory for local storage.
 * @return a DHT store registry which needs to have storage type modules registered with it.
 */
struct DHTStoreRegistry* DHTStoreModule_register(uint32_t maxEntries,
                                                 struct DHTModuleRegistry* registry,
                                                 const struct MemAllocator* allocator);

#endif /* DHT_STORE_MODULE_H */
