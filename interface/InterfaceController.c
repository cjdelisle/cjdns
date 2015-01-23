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
#include "crypto/AddressCalc.h"
#include "crypto/CryptoAuth_pvt.h"
#include "interface/InterfaceController.h"
#include "dht/dhtcore/RumorMill.h"
#include "dht/dhtcore/Router.h"
#include "memory/Allocator.h"
#include "net/SwitchPinger.h"
#include "util/Base32.h"
#include "util/Bits.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "util/version/Version.h"
#include "util/AddrTools.h"
#include "util/Defined.h"
#include "util/Checksum.h"
#include "util/Hex.h"
#include "wire/Error.h"
#include "wire/Message.h"

#include <stddef.h> // offsetof

/** After this number of milliseconds, a node will be regarded as unresponsive. */
#define UNRESPONSIVE_AFTER_MILLISECONDS (20*1024)

/**
 * After this number of milliseconds without a valid incoming message,
 * a peer is "lazy" and should be pinged.
 */
#define PING_AFTER_MILLISECONDS (3*1024)

/** How often to ping "lazy" peers, "unresponsive" peers are only pinged 20% of the time. */
#define PING_INTERVAL_MILLISECONDS 1024

/** The number of milliseconds to wait for a ping response. */
#define TIMEOUT_MILLISECONDS (2*1024)

/**
 * The number of seconds to wait before an unresponsive peer
 * making an incoming connection is forgotten.
 */
#define FORGET_AFTER_MILLISECONDS (256*1024)

/** Wait 32 seconds between sending beacon messages. */
#define BEACON_INTERVAL 32768


// ---------------- Map ----------------
#define Map_NAME EndpointsBySockaddr
#define Map_ENABLE_HANDLES
#define Map_KEY_TYPE struct Sockaddr*
#define Map_VALUE_TYPE struct Peer*
#define Map_USE_HASH
#define Map_USE_COMPARATOR
#include "util/Map.h"
static inline uint32_t Map_EndpointsBySockaddr_hash(struct Sockaddr** key)
{
    return Checksum_engine((uint8_t*) &(*key)[1], (*key)->addrLen - Sockaddr_OVERHEAD);
}
static inline int Map_EndpointsBySockaddr_compare(struct Sockaddr** keyA, struct Sockaddr** keyB)
{
    return Bits_memcmp((uint8_t*) *keyA, (uint8_t*) *keyB, (*keyA)->addrLen);
}
// ---------------- EndMap ----------------

#define ArrayList_TYPE struct Iface
#define ArrayList_NAME OfIfaces
#include "util/ArrayList.h"

struct InterfaceController_pvt;

struct Iface
{
    String* name;
    int beaconState;
    struct Map_EndpointsBySockaddr peerMap;
    struct InterfaceController_pvt* ic;
    struct Interface* addrIface;
    struct Allocator* alloc;
    Identity
};

struct Peer
{
    /** The interface which is registered with the switch. */
    struct Interface switchIf;

    /** Between CryptoAuth and external, needed to add address to message. */
    struct Interface externalIf;

    /** The internal (wrapped by CryptoAuth) interface. */
    struct Interface* cryptoAuthIf;

    /** The interface which this peer belongs to. */
    struct Iface* ici;

    /** The address within the interface of this peer. */
    struct Sockaddr* lladdr;

    /** The label for this endpoint, needed to ping the endpoint. */
    uint64_t switchLabel;

    /** Milliseconds since the epoch when the last *valid* message was received. */
    uint64_t timeOfLastMessage;

    /** Time when the last switch ping response was received from this node. */
    uint64_t timeOfLastPing;

    /** A counter to allow for 3/4 of all pings to be skipped when a node is definitely down. */
    uint32_t pingCount;

    /** The handle which can be used to look up this endpoint in the endpoint set. */
    uint32_t handle;

    /** True if we should forget about the peer if they do not respond. */
    bool isIncomingConnection;

    /**
     * If InterfaceController_PeerState_UNAUTHENTICATED, no permanent state will be kept.
     * During transition from HANDSHAKE to ESTABLISHED, a check is done for a registeration of a
     * node which is already registered in a different switch slot, if there is one and the
     * handshake completes, it will be moved.
     */
    enum InterfaceController_PeerState state;

    // traffic counters
    uint64_t bytesOut;
    uint64_t bytesIn;

    Identity
};

struct InterfaceController_pvt
{
    /** Public functions and fields for this ifcontroller. */
    struct InterfaceController pub;

    struct Allocator* const allocator;

    struct CryptoAuth* const ca;

    /** Switch for adding nodes when they are discovered. */
    struct SwitchCore* const switchCore;

    struct Router* const router;

    struct Random* const rand;

    struct RumorMill* const rumorMill;

    struct Log* const logger;

    struct EventBase* const eventBase;

    /** After this number of milliseconds, a neoghbor will be regarded as unresponsive. */
    uint32_t unresponsiveAfterMilliseconds;

    /** The number of milliseconds to wait before pinging. */
    uint32_t pingAfterMilliseconds;

    /** The number of milliseconds to let a ping go before timing it out. */
    uint32_t timeoutMilliseconds;

    /** After this number of milliseconds, an incoming connection is forgotten entirely. */
    uint32_t forgetAfterMilliseconds;

    /** How often to send beacon messages (milliseconds). */
    uint32_t beaconInterval;

    /** The timeout event to use for pinging potentially unresponsive neighbors. */
    struct Timeout* const pingInterval;

    /** For pinging lazy/unresponsive nodes. */
    struct SwitchPinger* const switchPinger;

    struct ArrayList_OfIfaces* icis;

    /** A password which is generated per-startup and sent out in beacon messages. */
    uint8_t beaconPassword[Headers_Beacon_PASSWORD_LEN];

    struct Headers_Beacon beacon;

    Identity
};

//---------------//

static inline struct InterfaceController_pvt* ifcontrollerForPeer(struct Peer* ep)
{
    return Identity_check(ep->ici->ic);
}

static void onPingResponse(struct SwitchPinger_Response* resp, void* onResponseContext)
{
    if (SwitchPinger_Result_OK != resp->res) {
        return;
    }
    struct Peer* ep =
        Identity_check((struct Peer*) onResponseContext);
    struct InterfaceController_pvt* ic = ifcontrollerForPeer(ep);

    struct Address addr;
    Bits_memset(&addr, 0, sizeof(struct Address));
    Bits_memcpyConst(addr.key, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
    addr.path = ep->switchLabel;
    addr.protocolVersion = resp->version;

    if (Defined(Log_DEBUG)) {
        uint8_t addrStr[60];
        Address_print(addrStr, &addr);
        uint8_t key[56];
        Base32_encode(key, 56, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
        if (!Version_isCompatible(Version_CURRENT_PROTOCOL, resp->version)) {
            Log_debug(ic->logger, "got switch pong from node [%s] with incompatible version [%d]",
                      key, resp->version);
        } else {
            Log_debug(ic->logger, "got switch pong from node [%s] with version [%d]",
                      key, resp->version);
        }
    }

    if (!Version_isCompatible(Version_CURRENT_PROTOCOL, resp->version)) {
        return;
    }

    if (!ep->timeOfLastPing) {
        // We've never heard from this machine before (or we've since forgotten about it)
        // This is here because we want the tests to function without the janitor present.
        // Other than that, it just makes a slightly more synchronous/guaranteed setup.
        Router_sendGetPeers(ic->router, &addr, 0, 0, ic->allocator);
    }

    struct Node_Link* link = Router_linkForPath(ic->router, resp->label);
    if (!link || !Node_getBestParent(link->child)) {
        RumorMill_addNode(ic->rumorMill, &addr);
    } else {
        Log_debug(ic->logger, "link exists");
    }

    ep->timeOfLastPing = Time_currentTimeMilliseconds(ic->eventBase);

    if (Defined(Log_DEBUG)) {
        // This will be false if it times out.
        //Assert_true(label == ep->switchLabel);
        uint8_t path[20];
        AddrTools_printPath(path, resp->label);
        uint8_t sl[20];
        AddrTools_printPath(sl, ep->switchLabel);
        Log_debug(ic->logger, "Received [%s] from lazy endpoint [%s]  [%s]",
                  SwitchPinger_resultString(resp->res)->bytes, path, sl);
    }
}

/*
 * Send a ping packet to one of the endpoints.
 */
static void sendPing(struct Peer* ep)
{
    struct InterfaceController_pvt* ic = ifcontrollerForPeer(ep);

    ep->pingCount++;

    struct SwitchPinger_Ping* ping =
        SwitchPinger_newPing(ep->switchLabel,
                             String_CONST(""),
                             ic->timeoutMilliseconds,
                             onPingResponse,
                             ic->allocator,
                             ic->switchPinger);

    if (Defined(Log_DEBUG)) {
        uint8_t key[56];
        Base32_encode(key, 56, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
        if (!ping) {
            Log_debug(ic->logger, "Failed to ping [%s.k], out of ping slots", key);
        } else {
            Log_debug(ic->logger, "SwitchPing [%s.k]", key);
        }
    }

    if (ping) {
        ping->onResponseContext = ep;
    }
}

static void iciPing(struct Iface* ici, struct InterfaceController_pvt* ic)
{
    if (!ici->peerMap.count) { return; }
    uint64_t now = Time_currentTimeMilliseconds(ic->eventBase);

    // scan for endpoints have not sent anything recently.
    uint32_t startAt = Random_uint32(ic->rand) % ici->peerMap.count;
    for (uint32_t i = startAt, count = 0; count < ici->peerMap.count;) {
        i = (i + 1) % ici->peerMap.count;
        count++;

        struct Peer* ep = ici->peerMap.values[i];

        if (now < ep->timeOfLastMessage + ic->pingAfterMilliseconds) {
            if (now < ep->timeOfLastPing + ic->pingAfterMilliseconds) {
                // Possibly an out-of-date node which is mangling packets, don't ping too often
                // because it causes the RumorMill to be filled with this node over and over.
                continue;
            }

            struct Node_Link* link = Router_linkForPath(ic->router, ep->switchLabel);
            // It exists, it's parent is the self-node, and it's label is equal to the switchLabel.
            if (link
                && Node_getBestParent(link->child)
                && Node_getBestParent(link->child)->parent->address.path == 1
                && Node_getBestParent(link->child)->cannonicalLabel == ep->switchLabel)
            {
                continue;
            }
        }

        #ifdef Log_DEBUG
              uint8_t key[56];
              Base32_encode(key, 56, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
        #endif

        if (ep->isIncomingConnection && now > ep->timeOfLastMessage + ic->forgetAfterMilliseconds) {
            Log_debug(ic->logger, "Unresponsive peer [%s.k] has not responded in [%u] "
                                  "seconds, dropping connection",
                                  key, ic->forgetAfterMilliseconds / 1024);
            Allocator_free(ep->externalIf.allocator);
            continue;
        }

        bool unresponsive = (now > ep->timeOfLastMessage + ic->unresponsiveAfterMilliseconds);
        if (unresponsive) {
            // our link to the peer is broken...
            Router_disconnectedPeer(ic->router, ep->switchLabel);

            // Lets skip 87% of pings when they're really down.
            if (ep->pingCount % 8) {
                ep->pingCount++;
                continue;
            }

            ep->state = InterfaceController_PeerState_UNRESPONSIVE;
        }

        Log_debug(ic->logger,
                  "Pinging %s peer [%s.k] lag [%u]",
                  (unresponsive ? "unresponsive" : "lazy"),
                  key,
                  (uint32_t)((now - ep->timeOfLastMessage) / 1024));

        sendPing(ep);

        // we only ping one node
        return;
    }
}

/**
 * Check the table for nodes which might need to be pinged, ping a node if necessary.
 * If a node has not responded in unresponsiveAfterMilliseconds then mark them as unresponsive
 * and if the connection is incoming and the node has not responded in forgetAfterMilliseconds
 * then drop them entirely.
 * This is called every PING_INTERVAL_MILLISECONDS but pingCallback is a misleading name.
 */
static void pingCallback(void* vic)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) vic);
    for (int i = 0; i < ic->icis->length; i++) {
        struct Iface* ici = ArrayList_OfIfaces_get(ic->icis, i);
        iciPing(ici, ic);
    }
}

/** If there's already an endpoint with the same public key, merge the new one with the old one. */
static void moveEndpointIfNeeded(struct Peer* ep)
{
    struct Iface* ici = ep->ici;
    Log_debug(ici->ic->logger, "Checking for old sessions to merge with.");
    uint8_t* key = CryptoAuth_getHerPublicKey(ep->cryptoAuthIf);
    for (uint32_t i = 0; i < ici->peerMap.count; i++) {
        struct Peer* thisEp = ici->peerMap.values[i];
        uint8_t* thisKey = CryptoAuth_getHerPublicKey(thisEp->cryptoAuthIf);
        if (thisEp != ep && !Bits_memcmp(thisKey, key, 32)) {
            Log_info(ici->ic->logger, "Moving endpoint to merge new session with old.");

            // flush out the new entry if needed.
            Router_disconnectedPeer(ici->ic->router, ep->switchLabel);
            ep->switchLabel = thisEp->switchLabel;
            SwitchCore_swapInterfaces(&thisEp->switchIf, &ep->switchIf);
            Allocator_free(thisEp->externalIf.allocator);
            return;
        }
    }
}

// Incoming message which has passed through the cryptoauth and needs to be forwarded to the switch.
static uint8_t receivedAfterCryptoAuth(struct Message* msg, struct Interface* cryptoAuthIf)
{
    struct Peer* ep = Identity_check((struct Peer*) cryptoAuthIf->receiverContext);
    struct InterfaceController_pvt* ic = ifcontrollerForPeer(ep);

    // nonce added by the CryptoAuth session.
    Message_pop(msg, NULL, 4, NULL);

    ep->bytesIn += msg->length;

    int caState = CryptoAuth_getState(cryptoAuthIf);
    if (ep->state < InterfaceController_PeerState_ESTABLISHED) {
        // EP states track CryptoAuth states...
        ep->state = caState;
        if (caState == CryptoAuth_ESTABLISHED) {
            moveEndpointIfNeeded(ep);
        } else {
            // prevent some kinds of nasty things which could be done with packet replay.
            // This is checking the message switch header and will drop it unless the label
            // directs it to *this* router.
            if (msg->length < 8 || msg->bytes[7] != 1) {
                Log_info(ic->logger, "DROP message because CA is not established.");
                return Error_NONE;
            } else {
                // When a "server" gets a new connection from a "client" the router doesn't
                // know about that client so if the client sends a packet to the server, the
                // server will be unable to handle it until the client has sent inter-router
                // communication to the server. Here we will ping the client so when the
                // server gets the ping response, it will insert the client into its table
                // and know its version.

                // prevent DoS by limiting the number of times this can be called per second
                // limit it to 7, this will affect innocent packets but it doesn't matter much
                // since this is mostly just an optimization and for keeping the tests happy.
                if ((ep->pingCount + 1) % 7) {
                    sendPing(ep);
                }
            }
        }
    } else if (ep->state == InterfaceController_PeerState_UNRESPONSIVE
        && caState == CryptoAuth_ESTABLISHED)
    {
        ep->state = InterfaceController_PeerState_ESTABLISHED;
    } else {
        ep->timeOfLastMessage = Time_currentTimeMilliseconds(ic->eventBase);
    }

    Identity_check(ep);
    Assert_true(!(msg->capacity % 4));
    return Interface_receiveMessage(&ep->switchIf, msg);
}

// This is directly called from SwitchCore, message is not encrypted.
static uint8_t sendFromSwitch(struct Message* msg, struct Interface* switchIf)
{
    struct Peer* ep = Identity_check((struct Peer*) switchIf);

    ep->bytesOut += msg->length;

    struct InterfaceController_pvt* ic = ifcontrollerForPeer(ep);
    uint8_t ret;
    uint64_t now = Time_currentTimeMilliseconds(ic->eventBase);
    if (now - ep->timeOfLastMessage > ic->unresponsiveAfterMilliseconds) {
        // TODO(cjd): This is a hack because if the time of last message exceeds the
        //            unresponsive time, we need to send back an error and that means
        //            mangling the message which would otherwise be in the queue.
        struct Allocator* tempAlloc = Allocator_child(ic->allocator);
        struct Message* toSend = Message_clone(msg, tempAlloc);
        ret = Interface_sendMessage(ep->cryptoAuthIf, toSend);
        Allocator_free(tempAlloc);
    } else {
        ret = Interface_sendMessage(ep->cryptoAuthIf, msg);
    }

    // TODO(cjd): this is not quite right
    // We don't always trust the UDP interface to be accurate
    // short spurious failures and packet-backup should not cause us to treat a link as dead
    if (ret == Error_UNDELIVERABLE) {
        ret = 0;
    }

    // If this node is unresponsive then return an error.
    if (ret || now - ep->timeOfLastMessage > ic->unresponsiveAfterMilliseconds) {
        return ret ? ret : Error_UNDELIVERABLE;
    } else {
        /* Way way way too much noise
        Log_debug(ic->logger,  "Sending to neighbor, last message from this node was [%u] ms ago.",
                  (now - ep->timeOfLastMessage));
        */
    }

    return Error_NONE;
}

static int closeInterface(struct Allocator_OnFreeJob* job)
{
    struct Peer* toClose =
        Identity_check((struct Peer*) job->userData);

    struct InterfaceController_pvt* ic = ifcontrollerForPeer(toClose);

    // flush the peer from the table...
    Router_disconnectedPeer(ic->router, toClose->switchLabel);

    int index = Map_EndpointsBySockaddr_indexForHandle(toClose->handle, &toClose->ici->peerMap);
    Assert_true(index >= 0);
    Map_EndpointsBySockaddr_remove(index, &toClose->ici->peerMap);
    return 0;
}

static uint8_t sendAfterCryptoAuth(struct Message* msg, struct Interface* externalIf)
{
    struct Peer* ep =
        Identity_check((struct Peer*) &(
            ((uint8_t*)externalIf)[-offsetof(struct Peer, externalIf)]));

    Assert_true(!(((uintptr_t)msg->bytes) % 4) && "alignment fault");

    // push the lladdr...
    Message_push(msg, ep->lladdr, ep->lladdr->addrLen, NULL);

    // very noisy
    if (Defined(Log_DEBUG) && false) {
        char* printedAddr =
            Hex_print(&ep->lladdr[1], ep->lladdr->addrLen - Sockaddr_OVERHEAD, msg->alloc);
        Log_debug(ep->ici->ic->logger, "Outgoing message to [%s]", printedAddr);
    }

    return Interface_sendMessage(ep->ici->addrIface, msg);
}

/**
 * Expects [ struct LLAddress ][ beacon ]
 */
static uint8_t handleBeacon(struct Message* msg, struct Iface* ici)
{
    struct InterfaceController_pvt* ic = ici->ic;
    if (!ici->beaconState) {
        // accepting beacons disabled.
        Log_debug(ic->logger, "[%s] Dropping beacon because beaconing is disabled",
                  ici->name->bytes);
        return 0;
    }

    if (msg->length < Headers_Beacon_SIZE) {
        Log_debug(ic->logger, "[%s] Dropping runt beacon", ici->name->bytes);
        return 0;
    }

    struct Sockaddr* lladdrInmsg = (struct Sockaddr*) msg->bytes;
    Message_shift(msg, -lladdrInmsg->addrLen, NULL);

    struct Headers_Beacon beacon;
    Message_pop(msg, &beacon, Headers_Beacon_SIZE, NULL);

    if (Defined(Log_DEBUG)) {
        char* content = Hex_print(&beacon, Headers_Beacon_SIZE, msg->alloc);
        Log_debug(ici->ic->logger, "RECV BEACON CONTENT[%s]", content);
    }

    struct Address addr;
    Bits_memset(&addr, 0, sizeof(struct Address));
    Bits_memcpyConst(addr.key, beacon.publicKey, 32);
    addr.protocolVersion = Endian_bigEndianToHost32(beacon.version_be);
    Address_getPrefix(&addr);
    String* printedAddr = Address_toString(&addr, msg->alloc);

    if (addr.ip6.bytes[0] != 0xfc || !Bits_memcmp(ic->ca->publicKey, addr.key, 32)) {
        Log_debug(ic->logger, "handleBeacon invalid key [%s]", printedAddr->bytes);
        return 0;
    }

    if (!Version_isCompatible(addr.protocolVersion, Version_CURRENT_PROTOCOL)) {
        if (Defined(Log_DEBUG)) {
            Log_debug(ic->logger, "[%s] DROP beacon from [%s] which was version [%d] "
                      "our version is [%d] making them incompatable", ici->name->bytes,
                      printedAddr->bytes, addr.protocolVersion, Version_CURRENT_PROTOCOL);
        }
        return 0;
    }

    String* beaconPass = String_newBinary(beacon.password, Headers_Beacon_PASSWORD_LEN, msg->alloc);
    int epIndex = Map_EndpointsBySockaddr_indexForKey(&lladdrInmsg, &ici->peerMap);
    if (epIndex > -1) {
        // The password might have changed!
        struct Peer* ep = ici->peerMap.values[epIndex];
        CryptoAuth_setAuth(beaconPass, 1, ep->cryptoAuthIf);
        return 0;
    }

    struct Allocator* epAlloc = Allocator_child(ici->alloc);
    struct Peer* ep = Allocator_calloc(epAlloc, sizeof(struct Peer), 1);
    struct Sockaddr* lladdr = Sockaddr_clone(lladdrInmsg, epAlloc);
    ep->ici = ici;
    ep->lladdr = lladdr;
    int setIndex = Map_EndpointsBySockaddr_put(&lladdr, &ep, &ici->peerMap);
    ep->handle = ici->peerMap.handles[setIndex];
    ep->isIncomingConnection = true;
    Identity_set(ep);
    Allocator_onFree(epAlloc, closeInterface, ep);

    ep->externalIf.sendMessage = sendAfterCryptoAuth;
    ep->externalIf.allocator = epAlloc;

    ep->cryptoAuthIf =
        CryptoAuth_wrapInterface(&ep->externalIf, beacon.publicKey, NULL, false, "outer", ic->ca);
    ep->cryptoAuthIf->receiveMessage = receivedAfterCryptoAuth;
    ep->cryptoAuthIf->receiverContext = ep;
    CryptoAuth_setAuth(beaconPass, 1, ep->cryptoAuthIf);

    ep->switchIf.sendMessage = sendFromSwitch;
    ep->switchIf.allocator = epAlloc;

    int ret = SwitchCore_addInterface(&ep->switchIf, 0, &ep->switchLabel, ic->switchCore);
    if (ret == SwitchCore_addInterface_OUT_OF_SPACE) {
        Log_debug(ic->logger, "handleBeacon SwitchCore out of space");
        Allocator_free(epAlloc);
        return 0;
    } else if (ret) {
        Log_debug(ic->logger, "handleBeacon SwitchCore something went wrong ret[%d]", ret);
        Allocator_free(epAlloc);
        return 0;
    }
    addr.path = ep->switchLabel;

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds(ic->eventBase) - ic->pingAfterMilliseconds - 1;

    Log_info(ic->logger, "Added peer [%s] from beacon", printedAddr->bytes);

    Router_sendGetPeers(ic->router, &addr, 0, 0, ic->allocator);
    return 0;
}

/**
 * Incoming message from someone we don't know, maybe someone responding to a beacon?
 * expects: [ struct LLAddress ][ content ]
 */
static uint8_t handleUnexpectedIncoming(struct Message* msg, struct Iface* ici)
{
    struct InterfaceController_pvt* ic = ici->ic;

    struct Allocator* epAlloc = Allocator_child(ici->alloc);

    struct Sockaddr* lladdr = (struct Sockaddr*) msg->bytes;
    Message_shift(msg, -lladdr->addrLen, NULL);
    lladdr = Sockaddr_clone(lladdr, epAlloc);

    Assert_true(!((uintptr_t)msg->bytes % 4) && "alignment fault");

    struct Peer* ep = Allocator_calloc(epAlloc, sizeof(struct Peer), 1);
    ep->ici = ici;
    ep->lladdr = lladdr;
    Assert_true(Map_EndpointsBySockaddr_indexForKey(&lladdr, &ici->peerMap) == -1);
    int index = Map_EndpointsBySockaddr_put(&lladdr, &ep, &ici->peerMap);
    Assert_true(index >= 0);
    ep->handle = ici->peerMap.handles[index];
    Identity_set(ep);
    Allocator_onFree(epAlloc, closeInterface, ep);

    ep->state = InterfaceController_PeerState_UNAUTHENTICATED;
    ep->isIncomingConnection = true;

    ep->externalIf.sendMessage = sendAfterCryptoAuth;
    ep->externalIf.allocator = epAlloc;

    ep->cryptoAuthIf =
        CryptoAuth_wrapInterface(&ep->externalIf, NULL, NULL, true, "outer", ic->ca);

    ep->cryptoAuthIf->receiveMessage = receivedAfterCryptoAuth;
    ep->cryptoAuthIf->receiverContext = ep;

    ep->switchIf.sendMessage = sendFromSwitch;
    ep->switchIf.allocator = epAlloc;

    int ret = SwitchCore_addInterface(&ep->switchIf, 0, &ep->switchLabel, ic->switchCore);
    if (ret) {
        Allocator_free(epAlloc);
        return 0;
    }

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds(ic->eventBase) - ic->pingAfterMilliseconds - 1;

    Log_info(ic->logger, "Adding peer with unknown key");

    if (Interface_receiveMessage(&ep->externalIf, msg)) {
        // If the first message is a dud, drop all state for this peer.
        // probably some random crap that wandered in the socket.
        Allocator_free(epAlloc);
    }

    return 0;
}

static uint8_t handleIncomingFromWire(struct Message* msg, struct Interface* iface)
{
    struct Iface* ici = Identity_check((struct Iface*) iface->receiverContext);

    struct Sockaddr* lladdr = (struct Sockaddr*) msg->bytes;
    if (msg->length < Sockaddr_OVERHEAD || msg->length < lladdr->addrLen) {
        Log_debug(ici->ic->logger, "DROP runt");
        return 0;
    }

    Assert_true(!((uintptr_t)msg->bytes % 4) && "alignment fault");
    Assert_true(!((uintptr_t)lladdr->addrLen % 4) && "alignment fault");

    // noisy
    if (Defined(Log_DEBUG) && false) {
        char* printedAddr = Hex_print(&lladdr[1], lladdr->addrLen - Sockaddr_OVERHEAD, msg->alloc);
        Log_debug(ici->ic->logger, "Incoming message from [%s]", printedAddr);
    }

    if (lladdr->flags & Sockaddr_flags_BCAST) {
        return handleBeacon(msg, ici);
    }

    int epIndex = Map_EndpointsBySockaddr_indexForKey(&lladdr, &ici->peerMap);
    if (epIndex == -1) {
        return handleUnexpectedIncoming(msg, ici);
    }

    struct Peer* ep = Identity_check((struct Peer*) ici->peerMap.values[epIndex]);
    Message_shift(msg, -lladdr->addrLen, NULL);
    return Interface_receiveMessage(&ep->externalIf, msg);
}

/**
 * Register an Ethernet-like interface.
 * Ethernet-like means the interface is capable of sending messages to one or more nodes
 * and differentiates between them using an address.
 *
 * @param ifc the interface controller
 * @param addrIface the interface
 * @param name a name for the interface, must be globally unique
 * @param alloc an allocator, the interface will be removed when this is freed.
 * @return the number of the interface in the interface table.
 */
int InterfaceController_regIface(struct InterfaceController* ifc,
                                 struct Interface* addrIface,
                                 String* name,
                                 struct Allocator* alloc)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) ifc);

    struct Iface* ici =
        Allocator_calloc(alloc, sizeof(struct Iface), 1);
    ici->name = String_clone(name, alloc);
    ici->peerMap.allocator = alloc;
    ici->ic = ic;
    ici->addrIface = addrIface;
    ici->alloc = alloc;

    Identity_set(ici);

    addrIface->receiveMessage = handleIncomingFromWire;
    addrIface->receiverContext = ici;

    return ArrayList_OfIfaces_add(ic->icis, ici);
}

static int freeAlloc(struct Allocator_OnFreeJob* job)
{
    struct Allocator* alloc = (struct Allocator*) job->userData;
    Allocator_free(alloc);
    return 0;
}

static void sendBeacon(struct Iface* ici, struct Allocator* tempAlloc)
{
    if (ici->beaconState < InterfaceController_beaconState_newState_SEND) {
        Log_debug(ici->ic->logger, "sendBeacon(%s) -> beaconing disabled", ici->name->bytes);
        return;
    }

    Log_debug(ici->ic->logger, "sendBeacon(%s)", ici->name->bytes);

    struct Message* msg = Message_new(0, 128, tempAlloc);
    Message_push(msg, &ici->ic->beacon, Headers_Beacon_SIZE, NULL);

    if (Defined(Log_DEBUG)) {
        char* content = Hex_print(msg->bytes, msg->length, tempAlloc);
        Log_debug(ici->ic->logger, "SEND BEACON CONTENT[%s]", content);
    }

    struct Sockaddr sa = {
        .addrLen = Sockaddr_OVERHEAD,
        .flags = Sockaddr_flags_BCAST
    };
    Message_push(msg, &sa, Sockaddr_OVERHEAD, NULL);

    int ret;
    if ((ret = Interface_sendMessage(ici->addrIface, msg)) != 0) {
        Log_info(ici->ic->logger, "Got error [%d] sending beacon to [%s]", ret, ici->name->bytes);
    }
}

static void beaconInterval(void* vIfController)
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) vIfController);

    struct Allocator* alloc = Allocator_child(ic->allocator);
    for (int i = 0; i < ic->icis->length; i++) {
        struct Iface* ici = ArrayList_OfIfaces_get(ic->icis, i);
        sendBeacon(ici, alloc);
    }
    Allocator_free(alloc);

    Timeout_setTimeout(beaconInterval, ic, ic->beaconInterval, ic->eventBase, ic->allocator);
}

int InterfaceController_beaconState(struct InterfaceController* ifc,
                                    int interfaceNumber,
                                    int newState)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) ifc);
    struct Iface* ici = ArrayList_OfIfaces_get(ic->icis, interfaceNumber);
    if (!ici) {
        return InterfaceController_beaconState_NO_SUCH_IFACE;
    }
    char* val = NULL;
    switch (newState) {
        default: return InterfaceController_beaconState_INVALID_STATE;
        case InterfaceController_beaconState_newState_OFF: val = "OFF"; break;
        case InterfaceController_beaconState_newState_ACCEPT: val = "ACCEPT"; break;
        case InterfaceController_beaconState_newState_SEND: val = "SEND"; break;
    }
    Log_debug(ic->logger, "InterfaceController_beaconState(%s, %s)", ici->name->bytes, val);
    ici->beaconState = newState;
    if (newState == InterfaceController_beaconState_newState_SEND) {
        // Send out a beacon right away so we don't have to wait.
        struct Allocator* alloc = Allocator_child(ici->alloc);
        sendBeacon(ici, alloc);
        Allocator_free(alloc);
    }
    return 0;
}

int InterfaceController_bootstrapPeer(struct InterfaceController* ifc,
                                      int interfaceNumber,
                                      uint8_t* herPublicKey,
                                      const struct Sockaddr* lladdrParm,
                                      String* password,
                                      struct Allocator* alloc)
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifc);

    Assert_true(herPublicKey);
    Assert_true(password);

    struct Iface* ici = ArrayList_OfIfaces_get(ic->icis, interfaceNumber);

    if (!ici) {
        return InterfaceController_bootstrapPeer_BAD_IFNUM;
    }

    Log_debug(ic->logger, "bootstrapPeer total [%u]", ici->peerMap.count);

    uint8_t ip6[16];
    AddressCalc_addressForPublicKey(ip6, herPublicKey);
    if (!AddressCalc_validAddress(ip6) ||
        !Bits_memcmp(ic->ca->publicKey, herPublicKey, 32))
    {
        return InterfaceController_bootstrapPeer_BAD_KEY;
    }

    struct Allocator* epAlloc = Allocator_child(ici->alloc);

    struct Sockaddr* lladdr = Sockaddr_clone(lladdrParm, epAlloc);

    struct Peer* ep = Allocator_calloc(epAlloc, sizeof(struct Peer), 1);
    int index = Map_EndpointsBySockaddr_put(&lladdr, &ep, &ici->peerMap);
    Assert_true(index >= 0);
    ep->handle = ici->peerMap.handles[index];
    ep->lladdr = lladdr;
    ep->ici = ici;
    ep->isIncomingConnection = false;
    Identity_set(ep);
    Allocator_onFree(epAlloc, closeInterface, ep);
    Allocator_onFree(alloc, freeAlloc, epAlloc);

    ep->externalIf.sendMessage = sendAfterCryptoAuth;
    ep->externalIf.allocator = epAlloc;

    ep->cryptoAuthIf = CryptoAuth_wrapInterface(&ep->externalIf,
                                                herPublicKey,
                                                NULL,
                                                false,
                                                "outer",
                                                ic->ca);

    ep->cryptoAuthIf->receiveMessage = receivedAfterCryptoAuth;
    ep->cryptoAuthIf->receiverContext = ep;
    CryptoAuth_setAuth(password, 1, ep->cryptoAuthIf);

    ep->switchIf.sendMessage = sendFromSwitch;
    ep->switchIf.allocator = epAlloc;

    int ret = SwitchCore_addInterface(&ep->switchIf, 0, &ep->switchLabel, ic->switchCore);
    if (ret) {
        Allocator_free(epAlloc);
        return (ret == SwitchCore_addInterface_OUT_OF_SPACE)
            ? InterfaceController_bootstrapPeer_OUT_OF_SPACE
            : InterfaceController_bootstrapPeer_INTERNAL;
    }

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds(ic->eventBase) - ic->pingAfterMilliseconds - 1;

    if (Defined(Log_INFO)) {
        uint8_t printAddr[60];
        AddrTools_printIp(printAddr, ip6);
        Log_info(ic->logger, "Adding peer [%s]", printAddr);
    }

    // We can't just add the node directly to the routing table because we do not know
    // the version. We'll send it a switch ping and when it responds, we will know it's
    // key (if we don't already) and version number.
    sendPing(ep);

    return 0;
}

int InterfaceController_getPeerStats(struct InterfaceController* ifController,
                                     struct Allocator* alloc,
                                     struct InterfaceController_PeerStats** statsOut)
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifController);

    int count = 0;
    for (int i = 0; i < ic->icis->length; i++) {
        struct Iface* ici = ArrayList_OfIfaces_get(ic->icis, i);
        count += ici->peerMap.count;
    }

    struct InterfaceController_PeerStats* stats =
        Allocator_malloc(alloc, sizeof(struct InterfaceController_PeerStats)*count);

    for (int j = 0; j < ic->icis->length; j++) {
        struct Iface* ici = ArrayList_OfIfaces_get(ic->icis, j);
        for (int i = 0; i < (int)ici->peerMap.count; i++) {
            struct Peer* peer = ici->peerMap.values[i];
            struct InterfaceController_PeerStats* s = &stats[i];
            s->pubKey = CryptoAuth_getHerPublicKey(peer->cryptoAuthIf);
            s->bytesOut = peer->bytesOut;
            s->bytesIn = peer->bytesIn;
            s->timeOfLastMessage = peer->timeOfLastMessage;
            s->state = peer->state;
            s->switchLabel = peer->switchLabel;
            s->isIncomingConnection = peer->isIncomingConnection;
            s->user = NULL;
            if (s->isIncomingConnection) {
                s->user = CryptoAuth_getUser(peer->cryptoAuthIf);
            }
            struct ReplayProtector* rp = CryptoAuth_getReplayProtector(peer->cryptoAuthIf);
            s->duplicates = rp->duplicates;
            s->lostPackets = rp->lostPackets;
            s->receivedOutOfRange = rp->receivedOutOfRange;
        }
    }

    *statsOut = stats;
    return count;
}

int InterfaceController_disconnectPeer(struct InterfaceController* ifController,
                                       uint8_t herPublicKey[32])
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifController);

    for (int j = 0; j < ic->icis->length; j++) {
        struct Iface* ici = ArrayList_OfIfaces_get(ic->icis, j);
        for (int i = 0; i < (int)ici->peerMap.count; i++) {
            struct Peer* peer = ici->peerMap.values[i];
            if (!Bits_memcmp(herPublicKey, CryptoAuth_getHerPublicKey(peer->cryptoAuthIf), 32)) {
                Allocator_free(peer->externalIf.allocator);
                return 0;
            }
        }
    }
    return InterfaceController_disconnectPeer_NOTFOUND;
}

struct InterfaceController* InterfaceController_new(struct CryptoAuth* ca,
                                                    struct SwitchCore* switchCore,
                                                    struct Router* router,
                                                    struct RumorMill* rumorMill,
                                                    struct Log* logger,
                                                    struct EventBase* eventBase,
                                                    struct SwitchPinger* switchPinger,
                                                    struct Random* rand,
                                                    struct Allocator* allocator)
{
    struct InterfaceController_pvt* out =
        Allocator_malloc(allocator, sizeof(struct InterfaceController_pvt));
    Bits_memcpyConst(out, (&(struct InterfaceController_pvt) {
        .allocator = allocator,
        .ca = ca,
        .rand = rand,
        .switchCore = switchCore,
        .router = router,
        .rumorMill = rumorMill,
        .logger = logger,
        .eventBase = eventBase,
        .switchPinger = switchPinger,
        .unresponsiveAfterMilliseconds = UNRESPONSIVE_AFTER_MILLISECONDS,
        .pingAfterMilliseconds = PING_AFTER_MILLISECONDS,
        .timeoutMilliseconds = TIMEOUT_MILLISECONDS,
        .forgetAfterMilliseconds = FORGET_AFTER_MILLISECONDS,
        .beaconInterval = BEACON_INTERVAL,

        .pingInterval = (switchPinger)
            ? Timeout_setInterval(pingCallback,
                                  out,
                                  PING_INTERVAL_MILLISECONDS,
                                  eventBase,
                                  allocator)
            : NULL

    }), sizeof(struct InterfaceController_pvt));
    Identity_set(out);

    out->icis = ArrayList_OfIfaces_new(allocator);

    // Add the beaconing password.
    Random_bytes(rand, out->beacon.password, Headers_Beacon_PASSWORD_LEN);
    String strPass = { .bytes=(char*)out->beacon.password, .len=Headers_Beacon_PASSWORD_LEN };
    int ret = CryptoAuth_addUser(&strPass, 1, String_CONST("Local Peers"), ca);
    if (ret) {
        Log_warn(logger, "CryptoAuth_addUser() returned [%d]", ret);
    }
    Bits_memcpyConst(out->beacon.publicKey, ca->publicKey, 32);
    out->beacon.version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);

    Timeout_setTimeout(beaconInterval, out, BEACON_INTERVAL, eventBase, allocator);

    return &out->pub;
}
