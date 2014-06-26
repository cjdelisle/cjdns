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
#include "interface/RainflyClient.h"
#include "benc/String.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/json/JsonBencSerializer.h"
#include "crypto/random/Random.h"
#include "io/ArrayReader.h"
#include "util/events/Timeout.h"
#include "util/Assert.h"
#include "wire/Message.h"
#include "wire/Error.h"

#include <crypto_sign_ed25519.h>

struct RainflyClient_Key
{
    uint8_t bytes[32];
};
Assert_compileTime(sizeof(struct RainflyClient_Key) == 32);

struct RainflyClient_Sig
{
    uint8_t bytes[64];
};
Assert_compileTime(sizeof(struct RainflyClient_Sig) == 64);

struct RainflyClient_Lookup_pvt;
struct RainflyClient_Lookup_pvt
{
    struct RainflyClient_Lookup pub;
    uint32_t cookie;
    int server;
    int triedServers;

    /** The index in hotKeys of the first key pushed to the request. */
    int firstKey;

    struct RainflyClient_pvt* ctx;
    struct Timeout* tryNextTimeout;
    struct RainflyClient_Lookup_pvt* next;

    Identity
};

struct RainflyClient_pvt
{
    struct RainflyClient pub;
    struct Interface* iface;
    struct Log* logger;
    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Random* rand;

    /** My address. */
    struct Sockaddr* addr;

    int serverCount;
    struct Sockaddr** servers;

    int keyCount;
    struct RainflyClient_Key* keys;

    /** Bitmap of available hot keys. */
    uint64_t keyMap;

    struct RainflyClient_Key* hotKeys;

    struct Timeout* hotKeysTimeout;

    struct RainflyClient_Lookup_pvt* lookups;

    Identity
};

enum RequestType
{
    RequestType_PING = 0x00,
    RequestType_HOT_KEYS = 0x01,
    RequestType_LOOKUP = 0x02
};

static String** readNames(struct Message* msg, int count, struct Allocator* alloc)
{
    String** out = Allocator_calloc(alloc, sizeof(uintptr_t), count+1);
    int totalLen = 0;
    for (int i = 0; i < count; i++) {
        if (msg->length < 1) {
            return NULL;
        }
        int len = Message_pop8(msg, NULL);
        out[i] = String_newBinary(NULL, len, alloc);
        Message_pop(msg, out[i]->bytes, len, NULL);
        totalLen += len + 1;
    }

    while (totalLen++ % 8) {
        if (Message_pop8(msg, NULL)) {
            return NULL;
        }
    }
    return out;
}

static void writeNames(struct Message* msg, String** names)
{
    int totalLength = 0;
    int i;
    for (i = 0; names && names[i]; ++i) {
        totalLength += names[i]->len + 1;
    }
    while (totalLength++ % 8) {
        Message_push8(msg, 0, NULL);
    }
    while (--i >= 0) {
        Message_push(msg, names[i]->bytes, names[i]->len, NULL);
        Message_push8(msg, names[i]->len, NULL);
    }
}

static int compare(String* a, String* b)
{
    if (a->len != b->len) {
        return (a->len > b->len) ? 1 : -1;
    }
    for (int i = 0; i < (int)a->len; i++) {
        if (a->bytes[i] != b->bytes[i]) {
            return ((uint8_t)a->bytes[i]) > ((uint8_t)b->bytes[i]) ? 1 : -1;
        }
    }
    return 0;
}

static void handleLookupReply(struct Message* msg, struct RainflyClient_pvt* ctx)
{
    Log_debug(ctx->logger, "lookup reply!");

    if (msg->length < 10) {
        Log_debug(ctx->logger, "runt");
        return;
    }

    uint32_t cookie = Message_pop8(msg, NULL);
    cookie <<= 16;
    cookie |= Message_pop16(msg, NULL);

    uint32_t height = Message_pop32(msg, NULL);
    String** namesAndValue = readNames(msg, 3, msg->alloc);

    if (!namesAndValue) {
        Log_debug(ctx->logger, "runt");
        return;
    }

    int count = msg->length / 64;
    if (msg->length != (count * 64)) {
        Log_debug(ctx->logger, "invalid message size");
        return;
    }

    struct RainflyClient_Sig* sigs = Allocator_malloc(msg->alloc, msg->length);
    for (int i = count-1; i >= 0; i--) {
        Message_pop(msg, sigs[i].bytes, 64, NULL);
    }

    Message_shift(msg, -msg->length, NULL);

    writeNames(msg, namesAndValue);
    Message_push32(msg, height, NULL);

    // find the matching lookup
    struct RainflyClient_Lookup_pvt* lookup = ctx->lookups;
    while (lookup && lookup->cookie != cookie) {
        lookup = lookup->next;
    }
    if (!lookup) {
        Log_debug(ctx->logger, "Response to unasked question");
        return;
    }

    enum RainflyClient_ResponseCode code = RainflyClient_ResponseCode_NO_ERROR;
    String* domain = lookup->pub.domain;
    if (!String_equals(namesAndValue[0], domain)) {
        if (compare(namesAndValue[0], namesAndValue[1]) == 1) {
            // they returned the last entry in the set
            // it must fall outside of range
            if (compare(domain, namesAndValue[0]) != 1 && compare(namesAndValue[1], domain) != 1) {
                Log_debug(ctx->logger, "Invalid NXDOMAIN with last entry [%s] -> [%s] req: [%s]",
                          namesAndValue[0]->bytes, namesAndValue[1]->bytes, domain->bytes);
                return;
            }
        } else if (compare(domain, namesAndValue[0]) != 1
            || compare(namesAndValue[1], domain) != 1)
        {
            Log_debug(ctx->logger, "Invalid NXDOMAIN, domain not in range [%s] -> [%s] req: [%s]",
                      namesAndValue[0]->bytes, namesAndValue[1]->bytes, domain->bytes);
            return;
        }
        Log_debug(ctx->logger, "Domain [%s] Nonexistant [%s] -> [%s]",
                  domain->bytes, namesAndValue[0]->bytes, namesAndValue[1]->bytes);
        code = RainflyClient_ResponseCode_NXDOMAIN;
    }

    int validSigs = 0;
    uint8_t* scratch = Allocator_malloc(msg->alloc, msg->length + 64);
    for (int i = 0; i < count; i++) {
        unsigned long long x = 32;
        Message_push(msg, sigs[i].bytes, 64, NULL);
        uint8_t* key = ctx->hotKeys[(lookup->firstKey + i) % ctx->keyCount].bytes;
        if (!crypto_sign_ed25519_open(scratch, &x, msg->bytes, msg->length, key)
            && (int)x == msg->length - 64)
        {
            Log_debug(ctx->logger, "valid signature [%d] on [%s]", i, namesAndValue[0]->bytes);
            validSigs++;
        } else if (Bits_isZero(sigs[i].bytes, 64)) {
            Log_debug(ctx->logger, "unknown signature [%d] on [%s]", i, namesAndValue[0]->bytes);
        } else {
            Log_debug(ctx->logger, "invalid signature [%d] on [%s]", i, namesAndValue[0]->bytes);
        }
        Message_shift(msg, -64, NULL);
    }

    if (validSigs < ctx->pub.minSignatures) {
        Log_debug(ctx->logger, "Not enough signatures");
        return;
    }

    struct Reader* r = ArrayReader_new(namesAndValue[2]->bytes, namesAndValue[2]->len, msg->alloc);
    Dict d;
    if (JsonBencSerializer_get()->parseDictionary(r, msg->alloc, &d)) {
        Log_debug(ctx->logger, "Error parsing json");
        if (!code) {
            code = RainflyClient_ResponseCode_SERVER_ERROR;
        }
    }

    if (lookup->pub.onReply) {
        lookup->pub.onReply(&lookup->pub, &d, code);
    }

    Allocator_free(lookup->pub.alloc);
}

static void handleHotKeysReply(struct Message* msg, struct RainflyClient_pvt* ctx)
{
    Log_debug(ctx->logger, "hotkeys reply");

    if (msg->length < 7) {
        Log_debug(ctx->logger, "runt");
        return;
    }

    uint8_t keyCount = Message_pop8(msg, NULL);
    uint16_t start = Message_pop16(msg, NULL);
    if (keyCount > ctx->keyCount) {
        Log_debug(ctx->logger, "keycount too high");
        return;
    }
    if (start > keyCount) {
        Log_debug(ctx->logger, "start too high");
        return;
    }

    uint32_t bitField = Message_pop32(msg, NULL);

    if ((Bits_popCountx32(bitField) * (64+32)) != msg->length) {
        Log_debug(ctx->logger, "incorrect message size");
        return;
    }

    uint32_t i = start;
    do {
        i = (i - 1) % keyCount;
        if (!(bitField & 1)) {
            Log_debug(ctx->logger, "missing signature [%d]", i);
            bitField >>= 1;
            continue;
        }
        bitField >>= 1;

        // This needs to be 96 bytes otherwise crypto_sign_ed25519_open() will buffer overflow.
        // We only use the first 32
        uint8_t hotKey[96];
        uint8_t sig[96];
        Message_pop(msg, sig, 96, NULL);
        unsigned long long x = 32;
        if (!crypto_sign_ed25519_open(hotKey, &x, sig, 96, ctx->keys[i].bytes) && x == 32) {
            Log_debug(ctx->logger, "valid signature [%d]", i);
            Bits_memcpyConst(ctx->hotKeys[i].bytes, hotKey, 32);
            ctx->keyMap |= (1 << i);
        } else {
            Log_debug(ctx->logger, "invalid signature [%d]", i);
        }
    } while (i != start);
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct RainflyClient_pvt* ctx =
        Identity_check((struct RainflyClient_pvt*)iface->receiverContext);

    if (msg->length < ctx->addr->addrLen + 4) {
        Log_debug(ctx->logger, "runt");
        return Error_NONE;
    }

    Message_shift(msg, -ctx->addr->addrLen, NULL);

    uint8_t operation = Message_pop8(msg, NULL);
    if (!(operation & (1<<7))) {
        Log_debug(ctx->logger, "query");
    }

    operation &= ((1<<7)-1);

    Log_debug(ctx->logger, "Got a reply [%d]", operation);

    switch (operation) {
        case RequestType_PING: return Error_NONE;
        case RequestType_HOT_KEYS: handleHotKeysReply(msg, ctx); return Error_NONE;
        case RequestType_LOOKUP: handleLookupReply(msg, ctx); return Error_NONE;
        default: Log_debug(ctx->logger, "Got a message with unrecognized type [%d]", operation);
    }

    return Error_NONE;
}

static void sendHotKeysRequest(void* vRainflyClient)
{
    struct RainflyClient_pvt* ctx = Identity_check((struct RainflyClient_pvt*)vRainflyClient);

    int time;
    if (Bits_popCountx32(ctx->keyMap) < ctx->pub.minSignatures && ctx->keyCount) {
        time = Random_uint32(ctx->rand) % 5000;
    } else {
        time = (Random_uint32(ctx->rand) % 600000) + 120000;
    }
    Timeout_resetTimeout(ctx->hotKeysTimeout, time);

    if (!ctx->keyCount) {
        Log_debug(ctx->logger, "No keys to try");
        return;
    }
    if (!ctx->serverCount) {
        Log_debug(ctx->logger, "No servers to try");
        return;
    }
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct Message* msg = Message_new(0, 1536, alloc);
    int start = Random_uint32(ctx->rand) % ctx->keyCount;
    int j = 0;
    int i = start;
    do {
        Message_push(msg, ctx->keys[i].bytes, 32, NULL);
        i = (i + 1) % ctx->keyCount;
    } while (i != start && ++j < 10);

    // only 24 bits of space to put some information so put the beginning key and the current
    // number of keys so we can reconstruct which keys we sent when we get the response.
    Message_push16(msg, start, NULL);
    Message_push8(msg, ctx->keyCount, NULL);

    Message_push8(msg, RequestType_HOT_KEYS, NULL);

    int server = Random_uint32(ctx->rand) % ctx->serverCount;
    Message_push(msg, ctx->servers[server], ctx->addr->addrLen, NULL);

    Log_debug(ctx->logger, "Sending hotkeys request to [%s]",
              Sockaddr_print(ctx->servers[server], alloc));

    Interface_sendMessage(ctx->iface, msg);
    Allocator_free(alloc);
}

static int lookupOnFree(struct Allocator_OnFreeJob* onFree)
{
    struct RainflyClient_Lookup_pvt* lookup =
        Identity_check((struct RainflyClient_Lookup_pvt*)onFree->userData);

    struct RainflyClient_Lookup_pvt* l = lookup->ctx->lookups;
    if (l == lookup) {
        lookup->ctx->lookups = lookup->next;
        return 0;
    }

    while (l->next != lookup) {
        l = l->next;
    }
    l->next = lookup->next;

    return 0;
}

static void tryNextServer(void* vLookup)
{
    struct RainflyClient_Lookup_pvt* lookup =
        Identity_check((struct RainflyClient_Lookup_pvt*)vLookup);
    struct RainflyClient_pvt* ctx = Identity_check((struct RainflyClient_pvt*)lookup->ctx);

    if (lookup->triedServers++ > ctx->pub.maxTries) {
        if (lookup->pub.onReply) {
            lookup->pub.onReply(&lookup->pub, NULL, RainflyClient_ResponseCode_SERVER_ERROR);
        }
        Allocator_free(lookup->pub.alloc);
        return;
    }

    struct Allocator* alloc = Allocator_child(lookup->pub.alloc);
    struct Message* msg = Message_new(0, 1536, alloc);
    int start = lookup->firstKey = Random_uint32(ctx->rand) % ctx->keyCount;
    int j = 0;
    int i = start;
    do {
        Message_push(msg, ctx->hotKeys[i].bytes, 32, NULL);
        i = (i + 1) % ctx->keyCount;
    } while (i != start && ++j < 10);

    Log_debug(ctx->logger, "looking up [%s]", lookup->pub.domain->bytes);
    writeNames(msg, (String*[]) { lookup->pub.domain, NULL });

    uint32_t typeAndIdent = lookup->cookie | (RequestType_LOOKUP << 24);
    Message_push32(msg, typeAndIdent, NULL);

    int server = lookup->server++ % ctx->serverCount;
    Message_push(msg, ctx->servers[server], ctx->addr->addrLen, NULL);

    Log_debug(ctx->logger, "Sending lookup request to [%s]",
              Sockaddr_print(ctx->servers[server], alloc));

    Interface_sendMessage(ctx->iface, msg);
    Allocator_free(alloc);
}

////////////////////


struct RainflyClient_Lookup* RainflyClient_lookup(struct RainflyClient* client, String* domain)
{
    struct RainflyClient_pvt* ctx = Identity_check((struct RainflyClient_pvt*)client);

    if (!ctx->keyCount || ctx->keyCount < ctx->pub.minSignatures) {
        Log_debug(ctx->logger, "Not enough keys to get required signatues");
        return NULL;
    }
    if (!ctx->serverCount) {
        Log_debug(ctx->logger, "No servers to try");
        return NULL;
    }

    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct RainflyClient_Lookup_pvt* out =
        Allocator_calloc(alloc, sizeof(struct RainflyClient_Lookup_pvt), 1);
    out->pub.alloc = alloc;
    out->next = ctx->lookups;
    if (out->next) {
        out->cookie = out->next->cookie++ & 0x00ffffff;
    }
    ctx->lookups = out;
    out->ctx = ctx;
    Identity_set(out);

    out->pub.domain = String_newBinary(NULL, domain->len+2, alloc);
    Bits_memcpy(&out->pub.domain->bytes[2], domain->bytes, domain->len);
    out->pub.domain->bytes[0] = 'h';
    out->pub.domain->bytes[1] = '/';

    out->server = Random_uint32(ctx->rand) % ctx->serverCount;

    out->tryNextTimeout = Timeout_setTimeout(tryNextServer, out, 3000, ctx->eventBase, alloc);

    Allocator_onFree(alloc, lookupOnFree, out);

    tryNextServer(out);

    return &out->pub;
}

int RainflyClient_addKey(struct RainflyClient* client, uint8_t key[32])
{
    struct RainflyClient_pvt* ctx = Identity_check((struct RainflyClient_pvt*)client);
    if (ctx->keyCount > 63) {
        return RainflyClient_addKey_TOO_MANY_KEYS;
    }
    ctx->keyCount++;
    ctx->keys = Allocator_realloc(ctx->alloc, ctx->keys, ctx->keyCount * 32);
    ctx->hotKeys = Allocator_realloc(ctx->alloc, ctx->hotKeys, ctx->keyCount * 32);
    Bits_memcpyConst(&ctx->keys[ctx->keyCount-1], key, 32);
    return 0;
}

int RainflyClient_addServer(struct RainflyClient* client, struct Sockaddr* addr)
{
    struct RainflyClient_pvt* ctx = Identity_check((struct RainflyClient_pvt*)client);
    if (addr->addrLen != ctx->addr->addrLen
        || Sockaddr_getFamily(addr) != Sockaddr_getFamily(ctx->addr))
    {
        return RainflyClient_addServer_WRONG_ADDRESS_TYPE;
    }
    ctx->serverCount++;
    ctx->servers =
        Allocator_realloc(ctx->alloc, ctx->servers, ctx->serverCount * sizeof(uintptr_t));
    ctx->servers[ctx->serverCount-1] = Sockaddr_clone(addr, ctx->alloc);
    return 0;
}

struct RainflyClient* RainflyClient_new(struct AddrInterface* iface,
                                        struct EventBase* base,
                                        struct Random* rand,
                                        struct Log* logger)
{
    struct RainflyClient_pvt* ctx =
        Allocator_clone(iface->generic.allocator, (&(struct RainflyClient_pvt) {
            .pub = {
                .minSignatures = RainflyClient_DEFAULT_MIN_SIGNATURES,
                .maxTries = RainflyClient_DEFAULT_MAX_TRIES,
            },
            .iface = &iface->generic,
            .logger = logger,
            .addr = iface->addr,
            .alloc = iface->generic.allocator,
            .rand = rand,
            .eventBase = base,
        }));
    Identity_set(ctx);

    ctx->hotKeysTimeout = Timeout_setInterval(sendHotKeysRequest, ctx, 1000, base, ctx->alloc);

    iface->generic.receiveMessage = receiveMessage;
    iface->generic.receiverContext = ctx;

    return &ctx->pub;
}
