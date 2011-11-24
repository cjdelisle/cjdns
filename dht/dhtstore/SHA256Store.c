#include "crypto/Crypto.h"
#include "dht/DHTConstants.h"
#include "dht/dhtstore/DHTStoreConstants.h"
#include "dht/dhtstore/DHTStoreModule.h"
#include "dht/dhtstore/DHTStoreTools.h"
#include "dht/dhtstore/SHA256Store.h"
#include "libbenc/benc.h"

struct Context {
    /** A means of aquiring memory to store the sha256 entries. */
    const struct MemAllocator* storeAllocator;

    /** Secret random number used for generating tokens. */
    char secret[20];
};

/*-------------------- Prototypes --------------------*/
static void handleGetRequest(struct DHTMessage* replyMessage,
                             const struct DHTStoreEntry* entry,
                             void* vcontext);
static struct DHTStoreEntry* handlePutRequest(const struct DHTMessage* incomingMessage,
                                              Dict** replyMessagePtr,
                                              const struct DHTStoreTool* storeTool,
                                              void* vcontext,
                                              const struct MemAllocator* messageAllocator);

/*-------------------- Public --------------------*/

void SHA256Store_register(struct DHTStoreRegistry* registry, const struct MemAllocator* storeAllocator)
{
    struct Context* context =
        storeAllocator->clone(sizeof(struct Context), storeAllocator, &(struct Context) {
            .storeAllocator = storeAllocator
        });

    // Generate session secret.
    Crypto_randomize(&(String) { .bytes = context->secret, .len = 20 });

    struct DHTStoreModule module =
    {
        // Fields
        .name = "SHA256Store",
        .getQuery = { .bytes = "get_sha256", .len = 8 },
        .putQuery = { .bytes = "put_sha256", .len = 8 },
        .keySize = 32,

        .context = context,

        // Functions
        .handleGetRequest = handleGetRequest,
        .handlePutRequest = handlePutRequest,
    };
    registry->registerModule(&module, registry);
}

/*-------------------- Internal --------------------*/

static String* genToken(const struct DHTMessage* requestMessage,
                        void* vcontext,
                        const struct MemAllocator* allocator)
{
    const Dict* requestArgs = benc_lookupDictionary(requestMessage->asDict, &DHTConstants_arguments);
    const String* id = benc_lookupString(requestArgs, &DHTConstants_myId);
    const String* target = benc_lookupString(requestArgs, &DHTConstants_targetId);

    struct Context* context = (struct Context*) vcontext;
    return DHTStoreTools_generateToken(target, id, NULL, context->secret, allocator);
}

/** If the entry is NULL then nothing was found, just attach a token and return. */
static void handleGetRequest(struct DHTMessage* replyMessage,
                             const struct DHTStoreEntry* entry,
                             void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;

    // Get the arguments dictionary.
    Dict* args = benc_lookupDictionary(replyMessage->asDict, &DHTConstants_reply);
    if (args == NULL) {
        args = benc_newDictionary(replyMessage->allocator);
        benc_putDictionary(replyMessage->asDict, &DHTConstants_reply, args, replyMessage->allocator);
    }

    // Generate and attach a token.
    String* token = genToken(replyMessage->replyTo, context, replyMessage->allocator);
    benc_putString(args, &DHTConstants_authToken, token, replyMessage->allocator);

    if (entry != NULL) {
        // Send the value.
        String* value = benc_newBinaryString(entry->value, entry->length, replyMessage->allocator);
        benc_putString(args, DHTStoreConstants_VALUE, value, replyMessage->allocator);
    }
}

static struct DHTStoreEntry* handlePutRequest(const struct DHTMessage* incomingMessage,
                                              Dict** replyMessagePtr,
                                              const struct DHTStoreTool* storeTool,
                                              void* vcontext,
                                              const struct MemAllocator* messageAllocator)
{
    // unused.
    storeTool = storeTool;

    Dict* requestMessage = incomingMessage->asDict;
    const Dict* requestArgs = benc_lookupDictionary(requestMessage, &DHTConstants_arguments);
    const String* value = benc_lookupString(requestArgs, DHTStoreConstants_VALUE);
    if (value == NULL) {
        // No value given.
        // TODO: Some error reporting?
        return NULL;
    }

    // After this, we promise to send _some_ reply.
    *replyMessagePtr = benc_newDictionary(messageAllocator);

    String* hash = Crypto_sha256sum(value, messageAllocator);
    const String* token = benc_lookupString(requestMessage, &DHTConstants_authToken);
    const String* id = benc_lookupString(requestArgs, &DHTConstants_myId);

    struct Context* context = (struct Context*) vcontext;

    if (!DHTStoreTools_isTokenValid(token, hash, id, NULL, context->secret)) {
        // Bad token, craft an error reply.
        DHTStoreTools_craftErrorReply(requestMessage,
                                      *replyMessagePtr,
                                      203,
                                      "put_sha256 request with wrong token",
                                      messageAllocator);
        return NULL;
    }

    // Need a persistant allocator. The messageAllocator will be freed when the message is finished with.
    struct MemAllocator* entryAllocator = context->storeAllocator->child(context->storeAllocator);
    struct DHTStoreEntry* entry = entryAllocator->malloc(sizeof(struct DHTStoreEntry), entryAllocator);
    entry->key = entryAllocator->clone(32, entryAllocator, hash->bytes);
    entry->value = entryAllocator->clone(value->len, entryAllocator, value->bytes);
    entry->length = value->len;
    entry->allocator = entryAllocator;

    // Now send the key as the reply.
    Dict* replyArgs = benc_lookupDictionary(*replyMessagePtr, &DHTConstants_reply);
    if (replyArgs == NULL) {
        replyArgs = benc_newDictionary(messageAllocator);
        benc_putDictionary(*replyMessagePtr, &DHTConstants_reply, replyArgs, messageAllocator);
    }
    benc_putString(replyArgs, DHTStoreConstants_KEY, hash, messageAllocator);

    return entry;
}
