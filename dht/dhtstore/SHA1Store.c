#include <time.h>

#include "crypto/Crypto.h"
#include "dht/DHTConstants.h"
#include "dht/dhtstore/DHTStoreConstants.h"
#include "dht/dhtstore/DHTStoreModule.h"
#include "dht/dhtstore/DHTStoreTools.h"
#include "dht/dhtstore/SHA1Store.h"
#include "libbenc/benc.h"

struct SHA1Store_Context {
    /** A means of aquiring memory to store the sha1 entries. */
    const struct MemAllocator* storeAllocator;

    /** Secret random number used for generating tokens. */
    char secret[20];
};

/*-------------------- Prototypes --------------------*/
static int64_t getDate(struct DHTStoreEntry* entry);
static String* genToken(const struct DHTMessage* requestMessage,
                        void* vcontext,
                        const struct MemAllocator* allocator);
static struct DHTStoreEntry* handlePutRequest(const Dict* requestMessage,
                                              Dict* replyMessage,
                                              void* vcontext,
                                              const struct MemAllocator* messageAllocator);

/*-------------------- Public --------------------*/

void SHA1Store_register(struct DHTStoreRegistry* registry, const struct MemAllocator* storeAllocator)
{
    struct SHA1Store_Context* context = storeAllocator->clone(sizeof(struct SHA1Store_Context),
                                                              storeAllocator,
                                                              &(struct SHA1Store_Context) {
        .storeAllocator = storeAllocator
    });

    // Generate session secret.
    Crypto_randomize(&(String) { .bytes = context->secret, .len = 20 });

    struct DHTStoreModule module =
    {
        // Fields
        .name = "SHA1Store",
        .getQuery = { .bytes = "get_sha1", .len = 8 },
        .putQuery = { .bytes = "put_sha1", .len = 8 },
        .keySize = 20,
        .signatureSize = 0,

        .context = context,

        // Functions
        .getDate = getDate,
        .genToken = genToken,
        .handlePutRequest = handlePutRequest,
        .getSignature = NULL
    };
    registry->registerModule(&module, registry);
}

/*-------------------- Internal --------------------*/

static int64_t getDate(struct DHTStoreEntry* entry)
{
    entry = entry;
    return time(NULL) / 60;
}

static String* genToken(const struct DHTMessage* requestMessage,
                        void* vcontext,
                        const struct MemAllocator* allocator)
{
    const Dict* requestArgs = benc_lookupDictionary(requestMessage->asDict, &DHTConstants_arguments);
    const String* id = benc_lookupString(requestArgs, &DHTConstants_myId);
    const String* target = benc_lookupString(requestArgs, &DHTConstants_targetId);

    struct SHA1Store_Context* context = (struct SHA1Store_Context*) vcontext;
    String* token = DHTStoreTools_generateToken(target, id, NULL, context->secret, allocator);
    return token;
}

static struct DHTStoreEntry* handlePutRequest(const Dict* requestMessage,
                                              Dict* replyMessage,
                                              void* vcontext,
                                              const struct MemAllocator* messageAllocator)
{
    const Dict* requestArgs = benc_lookupDictionary(requestMessage, &DHTConstants_arguments);
    const String* value = benc_lookupString(requestArgs, DHTStoreConstants_VALUE);
    if (value == NULL) {
        // No value given.
        // TODO: Some error reporting?
        return NULL;
    }

    String* hash = Crypto_sha1sum(value, messageAllocator);
    const String* token = benc_lookupString(requestMessage, &DHTConstants_authToken);
    const String* id = benc_lookupString(requestArgs, &DHTConstants_myId);

    struct SHA1Store_Context* context = (struct SHA1Store_Context*) vcontext;

    if (!DHTStoreTools_isTokenValid(token, hash, id, NULL, context->secret)) {
        // Bad token, craft an error reply.
        DHTStoreTools_craftErrorReply(requestMessage,
                                      replyMessage,
                                      203,
                                      "put_sha1 request with wrong token",
                                      messageAllocator);
        return NULL;
    }

    // Need a persistant allocator. The messageAllocator will be freed when the message is finished with.
    struct MemAllocator* entryAllocator = context->storeAllocator->child(context->storeAllocator);
    struct DHTStoreEntry* entry = entryAllocator->malloc(sizeof(struct DHTStoreEntry), entryAllocator);
    entry->key = entryAllocator->clone(20, entryAllocator, hash->bytes);
    entry->value = entryAllocator->clone(value->len, entryAllocator, value->bytes);
    entry->length = value->len;
    entry->allocator = entryAllocator;

    // Now send the key as the reply.
    Dict* replyArgs = benc_lookupDictionary(replyMessage, &DHTConstants_reply);
    if (replyArgs == NULL) {
        replyArgs = benc_newDictionary(messageAllocator);
        benc_putDictionary(replyMessage, &DHTConstants_reply, replyArgs, messageAllocator);
    }
    benc_putString(replyArgs, DHTStoreConstants_KEY, hash, messageAllocator);

    return entry;
}
