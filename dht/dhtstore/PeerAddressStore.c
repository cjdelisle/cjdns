#include <stdio.h>
#include <time.h>

#include "crypto/Crypto.h"
#include "dht/DHTConstants.h"
#include "dht/dhtstore/DHTStoreConstants.h"
#include "dht/dhtstore/DHTStoreModule.h"
#include "dht/dhtstore/DHTStoreTools.h"
#include "dht/dhtstore/SHA1Store.h"
#include "libbenc/benc.h"

/** Representation of a peer which has announced itself to us. */
struct Peer
{
    /** The number of minutes since the torrent was announced when this peer was announced. */
    uint16_t timeAdded;

    /** The ip address with port concatinated. */
    char peerAddress[6];
};

/** Representation of a torrent which we know of peers for. */
struct Torrent;
struct Torrent
{
    /** Number of minutes since the epoch when this torrent was first announced. */
    uint32_t timeAnnounced;

    /** The number of peers who have announced this torrent. */
    uint32_t peerCount;

    /** Pointer to the buffer of peers. */
    struct Peer* peers;
};

struct Context
{
    /** A means of aquiring memory to store the sha1 entries. */
    const struct MemAllocator* storeAllocator;

    /** Secret random number used for generating tokens. */
    char secret[20];

    /** Pointer to the first torrent in the list. */
    struct Torrent* firstTorrent;
};

// Not that it will fluctuate but KEY_SIZE is more self explanitory in the code than 20
#define KEY_SIZE 20

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

void PeerAddressStore_register(struct DHTStoreRegistry* registry, const struct MemAllocator* storeAllocator)
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
        .name = "PeerAddressStore",
        .getQuery = { .bytes = "get_peers", .len = 9 },
        .putQuery = { .bytes = "announce_peer", .len = 13 },
        .keySize = KEY_SIZE,

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
    const String* target = benc_lookupString(requestArgs, &DHTConstants_infoHash);

    struct Context* context = (struct Context*) vcontext;
    return DHTStoreTools_generateToken(target, id, requestMessage->peerAddress, context->secret, allocator);
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
        // To keep packet size down we should not send more than about 512 bytes of peers.
        // "6:xxxxxx" 8 bytes per peer which allows for 64 peers.
        #define MAX_PEERS 64
        struct Torrent* torrent = (struct Torrent*) entry->additional;
        uint32_t peerCount = torrent->peerCount;
        uint32_t index = rand();
        List* peers = NULL;

        // Start at a random place in the array and loop around like a circular buffer.
        for (uint32_t count = 0; count < MAX_PEERS && count < peerCount; count++) {
            index = index % peerCount;
            String* peer =
                benc_newBinaryString(torrent->peers[index].peerAddress, 6, replyMessage->allocator);
            peers = benc_addString(peers, peer, replyMessage->allocator);
            index++;
        }

        // Send the value.
        if (peers != NULL) {
            benc_putList(args, &DHTConstants_values, peers, replyMessage->allocator);
        }
    }
}

static struct DHTStoreEntry* handlePutRequest(const struct DHTMessage* incomingMessage,
                                              Dict** replyMessagePtr,
                                              const struct DHTStoreTool* storeTool,
                                              void* vcontext,
                                              const struct MemAllocator* messageAllocator)
{
    if (incomingMessage->addressLength != 6) {
        printf("Got an announce_peer request with address length %d, ipv6 is not supported :(",
               (int) incomingMessage->addressLength);
        return NULL;
    }

    // After this, a reply is guaranteed.
    *replyMessagePtr = benc_newDictionary(messageAllocator);

    Dict* requestMessage = incomingMessage->asDict;
    const Dict* requestArgs = benc_lookupDictionary(requestMessage, &DHTConstants_arguments);
    const String* token = benc_lookupString(requestArgs, &DHTConstants_authToken);
    const String* id = benc_lookupString(requestArgs, &DHTConstants_myId);
    const String* target = benc_lookupString(requestArgs, &DHTConstants_infoHash);
    const Integer* port = benc_lookupInteger(requestArgs, &DHTConstants_port);
    struct Context* context = (struct Context*) vcontext;

    if (port == NULL || *port < 1 || *port > 65535) {
        DHTStoreTools_craftErrorReply(requestMessage,
                                      *replyMessagePtr,
                                      203,
                                      "Announce_peer with forbidden port number",
                                      messageAllocator);
        return NULL;
    }

    if (target == NULL || target->len != 20) {
        DHTStoreTools_craftErrorReply(requestMessage,
                                      *replyMessagePtr,
                                      203,
                                      "Announce_peer with no info_hash",
                                      messageAllocator);
        return NULL;
    }

    if (!DHTStoreTools_isTokenValid(token, target, id, incomingMessage->peerAddress, context->secret))
    {
        DHTStoreTools_craftErrorReply(requestMessage,
                                      *replyMessagePtr,
                                      203,
                                      "Announce_peer with wrong token",
                                      messageAllocator);
        return NULL;
    }

    // Lets find out if we have a entry for this hash.
    struct DHTStoreEntry* entry = storeTool->lookup(target, storeTool);
    struct MemAllocator* entryAllocator;
    struct Torrent* torrent;
    struct Peer* peer = NULL;

    if (entry == NULL) {
        entryAllocator = context->storeAllocator->child(context->storeAllocator);
        entry = entryAllocator->malloc(sizeof(struct DHTStoreEntry), entryAllocator);
        entry->allocator = entryAllocator;
        entry->key = entryAllocator->clone(KEY_SIZE, entryAllocator, target->bytes);
        torrent = entryAllocator->malloc(sizeof(struct Torrent), entryAllocator);
        peer = entryAllocator->malloc(sizeof(struct Peer), entryAllocator);
        memcpy(peer->peerAddress, incomingMessage->peerAddress, 6);
        entry->additional = torrent;
        torrent->peers = peer;
        torrent->peerCount = 1;
        torrent->timeAnnounced = time(NULL) / 60;
        peer->timeAdded = 0;
    } else {
        entryAllocator = entry->allocator;
        torrent = (struct Torrent*) entry->additional;
        for (uint32_t i = 0; i < torrent->peerCount; i++) {
            if (memcmp(torrent->peers[i].peerAddress, incomingMessage->peerAddress, 6) == 0) {
                peer = torrent->peers + i;
                break;
            }
        }

        // Peer not added to torrent yet.
        if (peer == NULL) {
            torrent->peers =
                entryAllocator->realloc(torrent->peers,
                                        (torrent->peerCount + 1) * sizeof(struct Peer),
                                        entryAllocator);
            peer = torrent->peers + torrent->peerCount;
        }

        memcpy(peer->peerAddress, incomingMessage->peerAddress, 6);
        peer->timeAdded = (time(NULL) / 60) - torrent->timeAnnounced;
    }

    return entry;
}
