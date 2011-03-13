#include <string.h>
#include <stdio.h>
#include <netinet/in.h>

#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "memory/MemAllocator.h"
#include "libbenc/benc.h"
#include "dht/dhtstore/DHTStoreModule.h"

/*--------------------Configuration--------------------*/

/**
 * Each entry will cost 572 bytes each on a 64 bit
 * system and 560 bytes on a 32 bit system.
 *
 * 40MB will provide enough space for 73,326 entries.
 */
#define STORAGE_SPACE_MEGABYTES  40

/**
 * The number of seconds before an entry should be removed.
 * Let's go with 1 hour but this can be changed later.
 * This number cannot be larger than 65535.
 */
#define EXPIRE_AFTER                     3600

/**
 * Entries for each of the individual store modules.
 * Each entry is linked to the next and load and store requests
 * walk the list of modules until they find the correct module
 * then if a store request, the content is validated and stored,
 * if a load request then the number of the module is used for looking up
 * the correct entry header.
 */
struct DHTStoreModuleWrapper
{
    /** The module which is wrapped. */
    struct DHTStoreModule module;

    /** The next module in the linked list. */
    struct DHTStoreModuleWrapper* next;
};

/**
 * An entry stub which is used for fast comparison with minimal memory lookups.
 * Each entry will take 24 bytes of space on a 64 bit architecture and 20 bytes on 32 bit.
 * Which means 43,690 entries per megabyte.
 * These will hopefully be pulled to the processor cache.
 */
struct DHTStoreModule_EntryHeader
{
    /**
     * The top 64 bits of the entry key, used for high speed memory comparison.
     * When a match of the top 64 is found, the entry will be loaded and the rest will
     * be checked to make sure it is a valid entry.
     */
    uint64_t top64;

    /**
     * This will be bumped every time the entry is loaded and entries will be swapped
     * so that the most commonly used entries will end up at the beginning of the buffer.
     */
    uint32_t hitCounter;

    /**
     * The time when this entry was inserted as a number
     * of minutes since the application was started.
     */
    uint16_t whenPut;

    /** The module which handles this data. */
    uint8_t type;

    /** Since the compiler will pad this anyway, make it explicit so I remember there is another byte. */
    uint8_t unused;

    /**
     * The entry data, placed in a larger buffer which unlike the header buffer
     * is not expected to fit in the processor cache. Type specific to the module which handles it.
     */
    void* entry;
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
 * The context for the module.
 */
struct DHTStoreModule_Context
{
    /** The module. */
    struct DHTModule module;

    /** The array of entry headers. */
    struct DHTStoreModule_EntryHeader* headers;

    /** The index of the first empty slot in the entry array. */
    uint32_t nextEmptySlot;

    /**
     * The length of the entry array
     * (the index of the first entry which would be out of bounds)
     */
    uint32_t headerArrayLength;

    /** The registry used to send messages in response to lookups and puts. */
    struct DHTModuleRegistry* registry;

    /** The registry containing all of the storage modules. */
    struct DHTStoreRegistry storageRegistry;
};

static const char* MODULE_NAME = "DHTStoreModule";

/*--------------------Prototypes--------------------*/
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext);
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);
static void registerModule(const struct DHTStoreModule* toRegister,
                           struct DHTStoreRegistry* thisRegistry);

/*--------------------Interface--------------------*/

/**
 * Create a new DHT Store module.
 *
 * @param allocator the means to acquire memory for local storage.
 * @param maxEntries the maximum number of entries which are allowed
 *                   before old entries will be replaced or entries will be rejected.
 * @param registry the DHT module registry.
 * @return a DHT store registry which needs to have storage type modules registered with it.
 */
struct DHTStoreRegistry* DHTStoreModule_register(struct MemAllocator* allocator,
                                                 uint32_t maxEntries,
                                                 struct DHTModuleRegistry* registry)
{
    struct DHTStoreModule_EntryHeader* headers =
        allocator->malloc(sizeof(struct DHTStoreModule_EntryHeader) * maxEntries, allocator);
    struct DHTStoreModule_Context* context =
        allocator->malloc(sizeof(struct DHTStoreModule_Context), allocator);

    struct DHTStoreModule_Context localContext = {
        .module = {
            .name = MODULE_NAME,
            .context = context,
            .handleIncoming = handleIncoming,
            .handleOutgoing = handleOutgoing
        },
        .headers = headers,
        .nextEmptySlot = 0,
        .headerArrayLength = maxEntries,
        .registry = registry,
        .storageRegistry = {
            .registerModule = registerModule,
            .allocator = allocator
        }
    };
    memcpy(context, &localContext, sizeof(struct DHTStoreModule_Context));

    DHTModules_register(&(context->module), registry);

    return &(context->storageRegistry);
}

/*--------------------Internals--------------------*/

static void registerModule(const struct DHTStoreModule* toRegister,
                           struct DHTStoreRegistry* thisRegistry)
{
    struct DHTStoreModuleWrapper* module =
        thisRegistry->allocator->malloc(sizeof(struct DHTStoreModuleWrapper), thisRegistry->allocator);
    module->next = NULL;
    memcpy(&(module->module), toRegister, sizeof(struct DHTStoreModule));

    if (thisRegistry->firstModule == NULL) {
        thisRegistry->firstModule = module;
        return;
    }

    struct DHTStoreModuleWrapper* thisMod = thisRegistry->firstModule;
    while (thisMod->next != NULL) {
        thisMod = thisMod->next;
    }
    thisMod->next = module;
}

/**
 * Get the first (mandatory) 20 bytes of the key to lookup.
 *
 * @param message the message to get the lookup key from.
 * @param top20 a pointer which will be set to the location of the top 20 bytes of the key.
 * @param additional a pointer which will be set to the location of additional bytes of the key.
 * @return the number of bytes in the key. If less than 20 bytes then the message did
 *         not have a proper target entry. If there is no key then the pointers will not be set.
 */
static uint32_t getKeyBytes(struct DHTMessage* message, char** top20, char** additional)
{
    const Dict* query = benc_lookupDictionary(message->asDict, &DHTConstants_query);
    uint32_t out = 0;
    if (query != NULL) {
        const String* target = benc_lookupString(query, &DHTConstants_target);
        if (target != NULL && target->len == 20) {
            *top20 = target->bytes;
            out += 20;
        }
        const String* k = benc_lookupString(query, &DHTConstants_key);
        if (k != NULL) {
            *additional = k->bytes;
            out += k->len;
        }
    }
    return out;
}

/**
 * Compare the given entry to the provided bytes to see if the hey is the same.
 *
 * @param header the header of the entry to compare to.
 * @param top20Bytes the high 20 bytes of the key.
 * @param additionalBytes additional bytes to compare if desired.
 * @param numberOfBytes the total number of bytes to compare which must exceed 19.
 * @return true if all numberOfBytes bytes are equal.
 */
uint32_t isMatch(struct DHTStoreModule_EntryHeader* header,
                 char* top20Bytes,
                 char* additionalBytes,
                 uint32_t numberOfBytes)
{
    /**
     *  0 1 2 3  0 1 2 3  0 1 2 3  0 1 2 3  0 1 2 3  0 1 2 3  0 1 2 3  0 1 2 3  0 1 2 3  0 1 2 3...
     * [ header->top64  ][               header->entry->key                   ][  header->entry->moreKey
     * [                 top20Bytes                ][        additionalBytes....
     */
    return  (memcmp(header->entry->key, top20Bytes + 8, 12) == 0)
         && numberOfBytes <= 20 ? true : (memcmp(header->entry->key + 12,
                                          additionalBytes,
                                          numberOfBytes < 32 ? numberOfBytes - 20 : 32 - 20) == 0)
         && numberOfBytes <= 32 ? true : (memcmp(header->entry->moreKey,
                                                 additionalBytes + 12,
                                                 numberOfBytes - 32) == 0);
}

/**
 * Lookup an entry in the map.
 * Also does house keeping on the map to make sure that
 * often used entries are looked at first.
 *
 * @param top20Bytes the first 20 bytes of the key.
 * @param additionalBytes more optional bytes.
 * @param numberOfBytes the total byte count for the lookup key.
 * @param type the type of entry to lookup.
 * @param headers a pointer to the array of entry headers.
 * @param length the number of entry headers to look at.
 * @return the first entry header found which matches the given key
 *         or NULL if no matches.
 */
static struct DHTStoreModule_EntryHeader* lookup(char* top20Bytes,
                                                 char* additionalBytes,
                                                 uint32_t numberOfBytes,
                                                 struct DHTStoreModule* handler,
                                                 struct DHTStoreModule_EntryHeader* headers,
                                                 uint32_t length)
{
    uint64_t top64 = *((uint64_t*) top20Bytes);
    uint32_t i;
    struct DHTStoreModule_EntryHeader* leastUsed = headers[0];
    for (i = 0; i < length; i++) {
        if (headers[i]->top64 == top64
            && headers[i]->type == handler->type
            && handler->isMatch(top20Bytes, additionalBytes, numberOfBytes, headers[i]->entry))
        {
            /* If we looked at another header which was used less than this,
             * swap them so this one will be looked at first. */
            if (leastUsed->hitCounter < headers[i]->hitCounter) {
                swap(leastUsed, headers[i]);
            }
            headers[i]->hitCounter++;

            return headers[i];
        }
        if (headers[i]->hitCount < leastUsed->hitCount) {
            leastUsed = headers[i];
        }
    }
    return NULL;
}

/**
 * Get the module for doing an operation.
 *
 * @param queryType the type of query which is being made eg: "get_sha1" or "putm_dsap256"
 * @param firstModule the head of the list of modules.
 * @param getPutBoth if negative then the module is returned only if the request is a get request.
 *                   if positive then the module is returned only if the request is a put request.
 *                   if 0 then the module is returned as lonfg as it matches, nomatter the request type.
 * @return the module which matches or NULL of none match.
 */
#define ONLY_GET_REQUESTS -1
#define ONLY_PUT_REQUESTS 1
static struct DHTStoreModule* getModule(const benc_bstr_t* queryType,
                                        const struct DHTStoreModuleWrapper* firstModule
                                        int32_t getPutBoth)
{
    const struct DHTStoreModuleWrapper* module = firstModule;
    uint8_t number = 0;
    while (module != NULL) {
        if ((getPutBoth > -1 && benc_stringEquals(queryType, module->module.putQuery))
            || (getPutBoth < 1 && benc_stringEquals(queryType, module->module.getQuery)))
        {
            break;
        }
        module = module->next;
    }
    if (module == NULL) {
        return NULL;
    }
    module->module->number = number;
    return module == NULL ? NULL : module->module;
}

/**
 * The only requests which are handled on the outbound are get requests.
 * All put requests never make it to the core.
 *
 * Get requests come in, they are passed through and bounced off of the core.
 * The core adds a list of nearest nodes since unrecognized requests are taken
 * to mean find_node. As the core is sending the response, this function intercepts
 * it and if there is an entry stored, the entry is added to the outgoing packet.
 */
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext)
{
    if (message->replyTo == NULL) {
        /* We are only concerned with outgoing replies. */
        return 0;
    }

    struct DHTStoreModule_Context* context = (struct DHTStoreModule_Context) vcontext;

    struct DHTStoreModule* handler =
        getModule(message->queryType, context->storageRegistry.firstModule, ONLY_GET_REQUESTS);
    if (handler == NULL)) {
        return 0;
    }

    char* top20Bytes = NULL;
    char* additionalBytes = NULL;
    uint32_t keyLength = getKeyBytes(message->replyTo, &top20Bytes, &additionalBytes);

    if (keyLength < 20 || keyLength > handler->keySize || top20Bytes == NULL) {
        return NULL;
    }

    struct DHTStoreModule_EntryHeader* header = lookup(top20Bytes,
                                                       additionalBytes,
                                                       keyLength,
                                                       handler,
                                                       context->headers,
                                                       context->nextEmptySlot);

    /* No entry found, send nodes without entry. */
    if (header == NULL) {
        return 0;
    }

    /* Send the value. */
    bobj_t* value = benc_newBinaryString(header->entry->value, header->entry->length, message->allocator);
    benc_putEntry(message->arguments, DHTStoreConstants_VALUE, value, message->allocator);

    /* If it's a mutable entry... */
    if (handler->signatureSize > 0) {
        /* Add the signature. */
        bobj_t* signature =
            benc_newBinaryString(header->entry->signature, handler->signatureSize, message->allocator);
        benc_putEntry(message->arguments, DHTStoreConstants_SIGNATURE, signature, message->allocator);

        /* And the datestamp. */
        bobj_t* date = benc_newInteger(header->entry->date, message->allocator);
        benc_putEntry(message->arguments, DHTStoreConstants_DATE, date, message->allocator);
    }

    return 0;
}

static int handleIncoming(struct DHTMessage* message,
                          void* vcontext)
{
    /* Only interested in queries, specifically put queries. */
    if (!benc_stringEquals(message->messageClass, &DHTConstants_query)) {
        return 0;
    }

    struct DHTStoreModule_Context* context = (struct DHTStoreModule_Context) vcontext;

    struct DHTStoreModule* module =
        getModule(message->queryType, context->storageRegistry.firstModule, ONLY_PUT_REQUESTS);
    if (module == NULL) {
        return 0;
    }

    isValid
}


#undef STORAGE_SPACE_MEGABYTES
#undef EXPIRE_AFTER
#define ONLY_GET_REQUESTS
#define ONLY_PUT_REQUESTS
