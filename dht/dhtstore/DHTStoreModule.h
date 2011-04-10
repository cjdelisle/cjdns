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

    /** The length of a signature if mutable data. If static data then 0. */
    const uint16_t signatureSize;

    void* const context;

    int64_t (* const getDate)(struct DHTStoreEntry* entry);

    void (* const prepareGetReply)(const Dict* requestMessage,
                                   Dict* responseDict,
                                   struct DHTStoreEntry* entry,
                                   void* vcontext,
                                   const struct MemAllocator* allocator);

    String* (* const genToken)(const struct DHTMessage* requestMessage,
                               void* vcontext,
                               const struct MemAllocator* allocator);

    struct DHTStoreEntry* (* const handlePutRequest)(const Dict* requestMessage,
                                                     Dict* replyMessage,
                                                     void* vcontext,
                                                     const struct MemAllocator* messageAllocator);

    /**
     * If the entry is static and has no signature, this function pointer should be set to NULL.
     *
     * @param ventry the storage entry cast to a generic void pointer.
     * @return the signature extracted from the entry.
     */
    String* (* const getSignature)(struct DHTStoreEntry* entry);

    /**
     * This is set when loading the module so that it can be referenced by a unique number.
     * It should not be set by the module itself and will be cleared if it is.
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
