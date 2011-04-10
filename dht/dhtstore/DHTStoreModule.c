#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <assert.h>

#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "memory/MemAllocator.h"
#include "libbenc/benc.h"
#include "dht/dhtstore/DHTStoreModule.h"
#include "dht/dhtstore/DHTStoreConstants.h"

/*--------------------Configuration--------------------*/

/**
 * Each entry will cost 572 bytes each on a 64 bit
 * system and 560 bytes on a 32 bit system.
 *
 * 40MB will provide enough space for 73,326 entries.
 */
#define STORAGE_SPACE_MEGABYTES 40

/**
 * The number of minutes before an entry should be removed.
 * Let's go with 8 hours but this can be changed later.
 * This number cannot be larger than 65535.
 */
#define EXPIRE_AFTER (8 * 60)

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
struct DHTStoreEntryHeader
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
    uint32_t hitCount;

    /** The time when this entry was inserted as a number of minutes since the epoch. */
    uint32_t whenPut;

    /** The module which handles this data. */
    uint8_t type;

    /** Since the compiler will pad this anyway, make it explicit so I remember it. */
    uint8_t unused[3];

    /**
     * The entry data, placed in a larger buffer which unlike the header buffer
     * is not expected to fit in the processor cache.
     */
    struct DHTStoreEntry* entry;
};

/**
 * The context for the module.
 */
struct DHTStoreModule_Context
{
    /** The module. */
    struct DHTModule module;

    /** The array of entry headers. */
    struct DHTStoreEntryHeader* headers;

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
 * @param maxEntries the maximum number of entries which are allowed
 *                   before old entries will be replaced or entries will be rejected.
 * @param registry the DHT module registry.
 * @param allocator the means to acquire memory for local storage.
 * @return a DHT store registry which needs to have storage type modules registered with it.
 */
struct DHTStoreRegistry* DHTStoreModule_register(uint32_t maxEntries,
                                                 struct DHTModuleRegistry* registry,
                                                 const struct MemAllocator* allocator)
{
    struct DHTStoreEntryHeader* headers =
        allocator->malloc(sizeof(struct DHTStoreEntryHeader) * maxEntries, allocator);
    struct DHTStoreModule_Context* context =
        allocator->malloc(sizeof(struct DHTStoreModule_Context), allocator);

    memcpy(context, &(struct DHTStoreModule_Context) {
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
    }, sizeof(struct DHTStoreModule_Context));

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
 *                   if null then only the first 20 bytes will be set and the return value will be
 *                   less than or equal to 20.
 * @return the number of bytes in the key. If less than 20 bytes then the message did
 *         not have a proper target entry. If there is no key then the pointers will not be set.
 */
static uint32_t getKeyBytes(struct DHTMessage* message, char** top20, char** additional)
{
    const Dict* args = benc_lookupDictionary(message->asDict, &DHTConstants_arguments);
    uint32_t out = 0;
    if (args != NULL) {
        const String* target = benc_lookupString(args, &DHTConstants_targetId);
        if (target != NULL && target->len == 20) {
            *top20 = target->bytes;
            out += 20;
        }
        if (additional == NULL) {
            return out;
        }
        const String* k = benc_lookupString(args, DHTStoreConstants_KEY);
        if (k != NULL) {
            *additional = k->bytes;
            out += k->len;
        }
    }
    return out;
}

static int32_t isMatch(const char* top20Bytes,
                       const char* additionalBytes,
                       uint32_t numberOfBytes,
                       struct DHTStoreEntry* entry)
{
    return numberOfBytes >= 20
        && (memcmp(top20Bytes, entry->key, 20) == 0)
        && (numberOfBytes == 20 || (memcmp(additionalBytes, entry->key + 20, numberOfBytes - 20)));
}

/**
 * Lookup an entry in the map.
 * Also does house keeping on the map to make sure that
 * often used entries are looked at first.
 *
 * @param top20Bytes the first 20 bytes of the key.
 * @param additionalBytes more optional bytes.
 * @param numberOfBytes the total byte count for the lookup key.
 * @param handler the module which should serve this entry lookup.
 * @param headers a pointer to the array of entry headers.
 * @param entryHeaderCount the number of entry headers to look at.
 * @return the first entry header found which matches the given key
 *         or NULL if no matches.
 */
static struct DHTStoreEntryHeader* lookup(const char* top20Bytes,
                                          const char* additionalBytes,
                                          uint32_t numberOfBytes,
                                          const struct DHTStoreModule* handler,
                                          struct DHTStoreEntryHeader* headers,
                                          uint32_t entryHeaderCount)
{
    uint64_t top64 = *((uint64_t*) top20Bytes);
    uint32_t i;
    struct DHTStoreEntryHeader* leastUsed = headers;
    for (i = 0; i < entryHeaderCount; i++) {
        if (headers[i].top64 == top64
            && headers[i].type == handler->type
            && isMatch(top20Bytes, additionalBytes, numberOfBytes, headers[i].entry))
        {
            /* If we looked at another header which was used less than this,
             * swap them so this one will be looked at first. */
            if (leastUsed->hitCount < headers[i].hitCount) {
                struct DHTStoreEntryHeader temp;
                memcpy(&temp, leastUsed, sizeof(struct DHTStoreEntryHeader));
                memcpy(leastUsed, headers + i, sizeof(struct DHTStoreEntryHeader));
                memcpy(headers + i, &temp, sizeof(struct DHTStoreEntryHeader));
            }
            headers[i].hitCount++;

            return headers + i;
        }
        if (headers[i].hitCount < leastUsed->hitCount) {
            leastUsed = headers + i;
        }
    }
    return NULL;
}

/**
 * Compare two entries.
 *
 * @param entryA the first entry to compare.
 * @param entryB the second entry to compare.
 * @return less than 0 if entryA is superior, greater than 0 if entryB is superior, 0 if equal.
 */
static int32_t compareValue(struct DHTStoreEntryHeader* entryA,
                            struct DHTStoreEntryHeader* entryB)
{
    return (entryA->hitCount + entryA->whenPut) - (entryB->hitCount + entryB->whenPut);
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
static struct DHTStoreModule* getModule(const String* queryType,
                                        struct DHTStoreModuleWrapper* firstModule,
                                        int32_t getPutBoth)
{
    struct DHTStoreModuleWrapper* module = firstModule;
    uint8_t number = 0;
    while (module != NULL) {
        if ((getPutBoth > -1 && benc_stringEquals(queryType, &(module->module.putQuery)))
            || (getPutBoth < 1 && benc_stringEquals(queryType, &(module->module.getQuery))))
        {
            break;
        }
        module = module->next;
        number++;
    }
    if (module == NULL) {
        return NULL;
    }
    module->module.type = number;
    return &(module->module);
}

/**
 * Insert the entry into the list.
 * This will check if there is space in the list and if not it will evict the oldest entry.
 * This function makes no guarantees about age of an entry before it is evicted.
 * The entry must first be allocated by the handler, this function only inserts it into the list.
 * Evicted entries will have their allocator freed.
 *
 * @param top20Bytes the high 20 bytes of the key for the entry to put.
 * @param additionalBytes the rest of the entry key.
 * @param entryToInsert the entry to put into the entry list.
 * @param handler the module which handles the entry (needed to check if thie is a duplicate).
 * @param context the DHTStoreModule_Context where the entry header is stored.
 */
static void insert(const char* top20Bytes,
                   const char* additionalBytes,
                   struct DHTStoreEntry* entryToInsert,
                   const struct DHTStoreModule* handler,
                   struct DHTStoreModule_Context* context)
{
    uint64_t top64;
    memcpy(&top64, top20Bytes, 8);

    struct DHTStoreEntryHeader* leastValued = context->headers;
    struct DHTStoreEntryHeader* current;

    uint32_t i;
    for (i = 0; i < context->nextEmptySlot; i++) {
        current = context->headers + i;

        if (current->top64 == top64
            && current->type == handler->type
            && isMatch(top20Bytes, additionalBytes, handler->keySize, current->entry))
        {
            /* If there is already an entry, delete that entry and change
             * the pointer to the newly inserted one.
             */
            current->entry->allocator->free(current->entry->allocator);
            current->entry = entryToInsert;
            current->whenPut = handler->getDate(entryToInsert);
            return;
        }

        if (compareValue(current, leastValued) > 0) {
            leastValued = current;
        }
    }

    if (context->nextEmptySlot >= context->headerArrayLength) {
        /* We are out of space, the least valued entry must go. */
        leastValued->entry->allocator->free(leastValued->entry->allocator);
        current = leastValued;
    } else {
        current = context->headers + context->nextEmptySlot;
        context->nextEmptySlot++;
    }

    current->top64 = top64;
    current->hitCount = 0;
    current->whenPut = handler->getDate(entryToInsert);
    current->type = handler->type;
    current->entry = entryToInsert;
}

/**
 * The only requests which are handled on the outbound are get requests.
 * Put requests never make it to the core.
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
        // We are only concerned with outgoing replies.
        return 0;
    }

    struct DHTStoreModule_Context* context = (struct DHTStoreModule_Context*) vcontext;

    String* queryType = benc_lookupString(message->replyTo->asDict, &DHTConstants_query);

    struct DHTStoreModule* handler =
        getModule(queryType, context->storageRegistry.firstModule, ONLY_GET_REQUESTS);
    if (handler == NULL) {
        return 0;
    }

    char* top20Bytes = NULL;
    char* additionalBytes = NULL;
    uint32_t keyLength = getKeyBytes(message->replyTo, &top20Bytes, &additionalBytes);

    if (keyLength < 20 || keyLength > handler->keySize || top20Bytes == NULL) {
        return 0;
    }

    struct DHTStoreEntryHeader* header = lookup(top20Bytes,
                                                additionalBytes,
                                                keyLength,
                                                handler,
                                                context->headers,
                                                context->nextEmptySlot);

    // Get the arguments dictionary.
    Dict* args = benc_lookupDictionary(message->asDict, &DHTConstants_reply);
    if (args == NULL) {
        args = benc_newDictionary(message->allocator);
        benc_putDictionary(message->asDict, &DHTConstants_reply, args, message->allocator);
    }

    // Generate and attach a token.
    String* token = handler->genToken(message->replyTo, handler->context, message->allocator);
    benc_putString(args, &DHTConstants_authToken, token, message->allocator);

    if (header != NULL) {
        // Send the value.
        String* value = benc_newBinaryString(header->entry->value,
                                             header->entry->length,
                                             message->allocator);
        benc_putString(args, DHTStoreConstants_VALUE, value, message->allocator);

        // If it's a mutable entry...
        if (handler->getSignature != NULL) {
            // Add the signature.
            benc_putString(args,
                           DHTStoreConstants_SIGNATURE,
                           handler->getSignature(header->entry),
                           message->allocator);

            // And the datestamp.
            benc_putInteger(args,
                            DHTStoreConstants_DATE,
                            handler->getDate(header->entry),
                            message->allocator);
        }
    }
    return 0;
}

static int handleIncoming(struct DHTMessage* message,
                          void* vcontext)
{
    // Only interested in queries, specifically put queries.
    String* messageType = benc_lookupString(message->asDict, &DHTConstants_messageType);
    if (!benc_stringEquals(messageType, &DHTConstants_query)) {
        return 0;
    }

    struct DHTStoreModule_Context* context = (struct DHTStoreModule_Context*) vcontext;

    String* queryType = benc_lookupString(message->asDict, &DHTConstants_arguments);

    struct DHTStoreModule* handler =
        getModule(queryType, context->storageRegistry.firstModule, ONLY_PUT_REQUESTS);
    if (handler == NULL) {
        return 0;
    }

    Dict* replyDict = benc_newDictionary(message->allocator);

    // Do the put operation, token will be checked in the module.
    struct DHTStoreEntry* entry =
        handler->handlePutRequest(message->asDict, replyDict, handler->context, message->allocator);

    if (entry != NULL) {
        insert(entry->key, entry->key + 20, entry, handler, context);
    }

    // If this is false then the handler had nothing to say.
    if (benc_entryCount(replyDict) > 0) {
        struct DHTMessage* reply =
            message->allocator->clone(sizeof(struct DHTMessage), message->allocator, message);

        reply->length = 0;
        reply->asDict = replyDict;
        reply->replyTo = message;

        DHTModules_handleOutgoing(reply, context->registry);
    }

    // We have sent our reply, we can kill the incoming message now.
    return -1;
}


#undef STORAGE_SPACE_MEGABYTES
#undef EXPIRE_AFTER
#undef ONLY_GET_REQUESTS
#undef ONLY_PUT_REQUESTS
