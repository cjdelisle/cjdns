#include "crypto/Crypto.h"
#include "libbenc/benc.h"
#include "dht/dhtstore/DhtStoreTools.h"

struct SHA1Store_Context {
    struct MemAllocator* storeAllocator;

    /** Secret random number used for generating tokens. */
    char secret[20];
};

/*-------------------- Prototypes --------------------*/
static int32_t isValid(const benc_bstr_t* key, const bobj_t* argumentsDict);
static benc_bstr_t* extractKeyFromPut(const bobj_t* argumentsDict, struct MemAllocator* allocator);
static benc_bstr_t* extractSignatureFromPut(const bobj_t* argumentsDict);
static int64_t extractDateFromPut(const bobj_t* argumentsDict);

/*-------------------- Public --------------------*/

void SHA1Store_register(struct DHTStoreRegistry* registry)
{
    struct DHTStoreModule module = {
        .name = "SHA1Store",
        .getQuery = BSTRING("get_sha1"),
        .putQuery = BSTRING("put_sha1"),
        .keySize = 20,
        .signatureSize = 0,
        .isValid = NULL,
        .extractKeyFromPut = extractKeyFromPut,
        .extractSignatureFromPut = NULL,
        .extractDateFromPut = NULL
    };
    registry->registerModule(module, registry);
}

/*-------------------- Internal --------------------*/

/**
 * Storage entry.
 */
struct Sha1Store_Entry
{
    /** The entire lookup key. */
    char key[20];

    /** The entry value. */
    bobj_t value;

    /** An allocator which may be freed when this entry is nolonger valid. */
    struct MemAllocator* allocator;
};

static int32_t isMatch(const char* top20Bytes,
                       const char* additionalBytes,
                       uint32_t providedBytes,
                       void* ventry)
{
    return providedBytes == 20 && memcmp(top20Bytes, ((struct Sha1Store_Entry*) ventry)->key, 20) == 0;
}

static void prepareGetResponse(bobj_t* argumentsDict, void* ventry, struct MemAllocator* allocator)
{
    struct Sha1Store_Entry* entry = (struct Sha1Store_Entry*) ventry;
    benc_putEntry(argumentsDict, DHTStoreConstants_VALUE, &entry->value, allocator);
}

static void* handlePutRequest(const bobj_t* requestMessageDict,
                              bobj_t* responseMessageDict,
                              void* vcontext,
                              struct MemAllocator* messageAllocator)
{
    const Dict* requestArguments = benc_lookupDictionary(requestMessageDict, &DHTConstants_query);
    const String* value = benc_lookupString(requestArguments, DHTStoreConstants_VALUE);
    if (value == NULL) {
        /* TODO: Some error reporting? */
        return NULL;
    }
    const String* hash = Crypto_sha1sum(value, messageAllocator);

    const String* token = benc_lookupString(requestMessage, &DHTConstants_token);
    const String* id = benc_lookupString(requestArguments, &DHTConstants_myId);

    struct SHA1Store_Context* context = (struct SHA1Store_Context*) vcontext;
    if (!DHTTools_isTokenValid(token, hash, id, NULL, context->secret)) {
        
        return NULL;
    }

    struct MemAllocator* entryAllocator = context->allocator->child(context->allocator);
    struct Sha1Store_Entry* entry = entryAllocator->malloc(sizeof(struct Sha1Store_Entry), entryAllocator);
    memcpy(entry->key, hash->bytes, 20);
    char* data = entryAllocator->malloc(valueString->len, entryAllocator);
    entry->value.len = value->len;
    entry->value.bytes = data;
    entry->allocator = entryAllocator;
    memcpy(data, valueString->bytes, valueString->len);

    benc_putString(responseArgumentsDict, DHTStoreConstants_KEY, hash);
    return entry;
}

static void deleteEntry(void* ventry)
{
    struct MemAllocator* allocator = ((struct Sha1Store_Entry*) ventry)->allocator;
    allocator->free(allocator);
}

static char* getKey(void* ventry)
{
    return ((struct Sha1Store_Entry*) ventry)->key;
}
