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
#include "interface/Iface.h"
#include "net/InterfaceController.h"
#include "net/PeerLink.h"
#include "memory/Allocator.h"
#include "net/SwitchPinger.h"
#include "wire/PFChan.h"
#include "net/EventEmitter.h"
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
#include "wire/Headers.h"

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
    return Sockaddr_hash(*key);
}
static inline int Map_EndpointsBySockaddr_compare(struct Sockaddr** keyA, struct Sockaddr** keyB)
{
    return Sockaddr_compare(*keyA, *keyB);
}
// ---------------- EndMap ----------------

#define ArrayList_TYPE struct InterfaceController_Iface_pvt
#define ArrayList_NAME OfIfaces
#include "util/ArrayList.h"

struct InterfaceController_pvt;

struct InterfaceController_Iface_pvt
{
    struct InterfaceController_Iface pub;
    String* name;
    int beaconState;
    struct Map_EndpointsBySockaddr peerMap;
    struct InterfaceController_pvt* ic;
    struct Allocator* alloc;
    Identity
};

struct Peer
{
    /** The interface which is registered with the switch. */
    struct Iface switchIf;

    struct Allocator* alloc;

    struct CryptoAuth_Session* caSession;

    struct PeerLink* peerLink;

    /** The interface which this peer belongs to. */
    struct InterfaceController_Iface_pvt* ici;

    /** The address within the interface of this peer. */
    struct Sockaddr* lladdr;

    struct Address addr;

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

    struct Allocator* const alloc;

    struct CryptoAuth* const ca;

    /** Switch for adding nodes when they are discovered. */
    struct SwitchCore* const switchCore;

    struct Random* const rand;

    struct Log* const logger;

    struct EventBase* const eventBase;

    /** For communicating with the Pathfinder. */
    struct Iface eventEmitterIf;

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

    /** Temporary allocator for allocating timeouts for sending beacon messages. */
    struct Allocator* beaconTimeoutAlloc;

    /** A password which is generated per-startup and sent out in beacon messages. */
    uint8_t beaconPassword[Headers_Beacon_PASSWORD_LEN];

    struct Headers_Beacon beacon;

    Identity
};

static void sendPeer(uint32_t pathfinderId,
                     enum PFChan_Core ev,
                     struct Peer* peer)
{
    struct InterfaceController_pvt* ic = Identity_check(peer->ici->ic);
    struct Allocator* alloc = Allocator_child(ic->alloc);
    struct Message* msg = Message_new(PFChan_Node_SIZE, 512, alloc);
    struct PFChan_Node* node = (struct PFChan_Node*) msg->bytes;
    Bits_memcpy(node->ip6, peer->addr.ip6.bytes, 16);
    Bits_memcpy(node->publicKey, peer->addr.key, 32);
    node->path_be = Endian_hostToBigEndian64(peer->addr.path);
    node->metric_be = 0xffffffff;
    node->version_be = Endian_hostToBigEndian32(peer->addr.protocolVersion);
    if (ev != PFChan_Core_PEER_GONE) {
        Assert_true(peer->addr.protocolVersion);
    }
    Message_push32(msg, pathfinderId, NULL);
    Message_push32(msg, ev, NULL);
    Iface_send(&ic->eventEmitterIf, msg);
    Allocator_free(alloc);
}

static void onPingResponse(struct SwitchPinger_Response* resp, void* onResponseContext)
{
    if (SwitchPinger_Result_OK != resp->res) {
        return;
    }
    struct Peer* ep = Identity_check((struct Peer*) onResponseContext);
    struct InterfaceController_pvt* ic = Identity_check(ep->ici->ic);

    ep->addr.protocolVersion = resp->version;

    if (Defined(Log_DEBUG)) {
        String* addr = Address_toString(&ep->addr, resp->ping->pingAlloc);
        if (!Version_isCompatible(Version_CURRENT_PROTOCOL, resp->version)) {
            Log_debug(ic->logger, "got switch pong from node [%s] with incompatible version",
                                  addr->bytes);
        } else if (ep->addr.path != resp->label) {
            uint8_t sl[20];
            AddrTools_printPath(sl, resp->label);
            Log_debug(ic->logger, "got switch pong from node [%s] mismatch label [%s]",
                                  addr->bytes, sl);
        } else {
            Log_debug(ic->logger, "got switch pong from node [%s]", addr->bytes);
        }
    }

    if (!Version_isCompatible(Version_CURRENT_PROTOCOL, resp->version)) {
        return;
    }

    if (ep->state == InterfaceController_PeerState_ESTABLISHED) {
        sendPeer(0xffffffff, PFChan_Core_PEER, ep);
    }

    ep->timeOfLastPing = Time_currentTimeMilliseconds(ic->eventBase);

    if (Defined(Log_DEBUG)) {
        String* addr = Address_toString(&ep->addr, resp->ping->pingAlloc);
        Log_debug(ic->logger, "Received [%s] from lazy endpoint [%s]",
                  SwitchPinger_resultString(resp->res)->bytes, addr->bytes);
    }
}

/*
 * Send a ping packet to one of the endpoints.
 */
static void sendPing(struct Peer* ep)
{
    struct InterfaceController_pvt* ic = Identity_check(ep->ici->ic);

    ep->pingCount++;

    struct SwitchPinger_Ping* ping =
        SwitchPinger_newPing(ep->addr.path,
                             String_CONST(""),
                             ic->timeoutMilliseconds,
                             onPingResponse,
                             ep->alloc,
                             ic->switchPinger);

    if (Defined(Log_DEBUG)) {
        uint8_t key[56];
        Base32_encode(key, 56, ep->caSession->herPublicKey, 32);
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

static void iciPing(struct InterfaceController_Iface_pvt* ici, struct InterfaceController_pvt* ic)
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
        }

        uint8_t keyIfDebug[56];
        if (Defined(Log_DEBUG)) {
            Base32_encode(keyIfDebug, 56, ep->caSession->herPublicKey, 32);
        }

        if (ep->isIncomingConnection && now > ep->timeOfLastMessage + ic->forgetAfterMilliseconds) {
            Log_debug(ic->logger, "Unresponsive peer [%s.k] has not responded in [%u] "
                                  "seconds, dropping connection",
                                  keyIfDebug, ic->forgetAfterMilliseconds / 1024);
            sendPeer(0xffffffff, PFChan_Core_PEER_GONE, ep);
            Allocator_free(ep->alloc);
            continue;
        }

        bool unresponsive = (now > ep->timeOfLastMessage + ic->unresponsiveAfterMilliseconds);
        if (unresponsive) {
            // our link to the peer is broken...

            // Lets skip 87% of pings when they're really down.
            if (ep->pingCount % 8) {
                ep->pingCount++;
                continue;
            }

            sendPeer(0xffffffff, PFChan_Core_PEER_GONE, ep);
            ep->state = InterfaceController_PeerState_UNRESPONSIVE;
            SwitchCore_setInterfaceState(&ep->switchIf,
                                         SwitchCore_setInterfaceState_ifaceState_DOWN);
        }

        Log_debug(ic->logger,
                  "Pinging %s peer [%s.k] lag [%u]",
                  (unresponsive ? "unresponsive" : "lazy"),
                  keyIfDebug,
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
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, i);
        iciPing(ici, ic);
    }
}

/** If there's already an endpoint with the same public key, merge the new one with the old one. */
static void moveEndpointIfNeeded(struct Peer* ep)
{
    struct InterfaceController_Iface_pvt* ici = ep->ici;
    Log_debug(ici->ic->logger, "Checking for old sessions to merge with.");
    for (uint32_t i = 0; i < ici->peerMap.count; i++) {
        struct Peer* thisEp = ici->peerMap.values[i];
        if (thisEp != ep && !Bits_memcmp(thisEp->addr.key, ep->addr.key, 32)) {
            Log_info(ici->ic->logger, "Moving endpoint to merge new session with old.");

            ep->addr.path = thisEp->addr.path;
            SwitchCore_swapInterfaces(&thisEp->switchIf, &ep->switchIf);

            Assert_true(ep->switchIf.connectedIf->send);
            Assert_true(thisEp->switchIf.connectedIf->send);
            Allocator_free(thisEp->alloc);
            Assert_true(!thisEp->switchIf.connectedIf->send);
            Assert_true(ep->switchIf.connectedIf->send);
            return;
        }
    }
}

// Incoming message which has passed through the cryptoauth and needs to be forwarded to the switch.
static Iface_DEFUN receivedPostCryptoAuth(struct Message* msg,
                                          struct Peer* ep,
                                          struct InterfaceController_pvt* ic)
{
    ep->bytesIn += msg->length;

    int caState = CryptoAuth_getState(ep->caSession);
    if (ep->state < InterfaceController_PeerState_ESTABLISHED) {
        // EP states track CryptoAuth states...
        ep->state = caState;
        SwitchCore_setInterfaceState(&ep->switchIf, SwitchCore_setInterfaceState_ifaceState_UP);

        Bits_memcpy(ep->addr.key, ep->caSession->herPublicKey, 32);
        Address_getPrefix(&ep->addr);

        if (caState == CryptoAuth_ESTABLISHED) {
            moveEndpointIfNeeded(ep);
            //sendPeer(0xffffffff, PFChan_Core_PEER, ep);// version is not known at this point.
        } else {
            // prevent some kinds of nasty things which could be done with packet replay.
            // This is checking the message switch header and will drop it unless the label
            // directs it to *this* router.
            if (msg->length < 8 || msg->bytes[7] != 1) {
                Log_info(ic->logger, "DROP message because CA is not established.");
                return 0;
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
        SwitchCore_setInterfaceState(&ep->switchIf, SwitchCore_setInterfaceState_ifaceState_UP);
    } else {
        ep->timeOfLastMessage = Time_currentTimeMilliseconds(ic->eventBase);
    }

    Identity_check(ep);
    Assert_true(!(msg->capacity % 4));
    return Iface_next(&ep->switchIf, msg);
}

// This is directly called from SwitchCore, message is not encrypted.
static Iface_DEFUN sendFromSwitch(struct Message* msg, struct Iface* switchIf)
{
    struct Peer* ep = Identity_check((struct Peer*) switchIf);

    ep->bytesOut += msg->length;

    int msgs = PeerLink_send(msg, ep->peerLink);

    for (int i = 0; i < msgs; i++) {
        msg = PeerLink_poll(ep->peerLink);
        Assert_true(!CryptoAuth_encrypt(ep->caSession, msg));

        Assert_true(!(((uintptr_t)msg->bytes) % 4) && "alignment fault");

        // push the lladdr...
        Message_push(msg, ep->lladdr, ep->lladdr->addrLen, NULL);

        // very noisy
        if (Defined(Log_DEBUG) && false) {
            char* printedAddr =
                Hex_print(&ep->lladdr[1], ep->lladdr->addrLen - Sockaddr_OVERHEAD, msg->alloc);
            Log_debug(ep->ici->ic->logger, "Outgoing message to [%s]", printedAddr);
        }

        Iface_send(&ep->ici->pub.addrIf, msg);
    }
    return NULL;
}

static int closeInterface(struct Allocator_OnFreeJob* job)
{
    struct Peer* toClose = Identity_check((struct Peer*) job->userData);

    sendPeer(0xffffffff, PFChan_Core_PEER_GONE, toClose);

    int index = Map_EndpointsBySockaddr_indexForHandle(toClose->handle, &toClose->ici->peerMap);
    Assert_true(index >= 0 && toClose->ici->peerMap.values[index] == toClose);
    Map_EndpointsBySockaddr_remove(index, &toClose->ici->peerMap);
    return 0;
}

/**
 * Expects [ struct LLAddress ][ beacon ]
 */
static Iface_DEFUN handleBeacon(struct Message* msg, struct InterfaceController_Iface_pvt* ici)
{
    struct InterfaceController_pvt* ic = ici->ic;
    if (!ici->beaconState) {
        // accepting beacons disabled.
        Log_debug(ic->logger, "[%s] Dropping beacon because beaconing is disabled",
                  ici->name->bytes);
        return NULL;
    }

    if (msg->length < Headers_Beacon_SIZE) {
        Log_debug(ic->logger, "[%s] Dropping runt beacon", ici->name->bytes);
        return NULL;
    }

    struct Sockaddr* lladdrInmsg = (struct Sockaddr*) msg->bytes;

    // clear the bcast flag
    lladdrInmsg->flags = 0;

    Message_shift(msg, -lladdrInmsg->addrLen, NULL);

    struct Headers_Beacon beacon;
    Message_pop(msg, &beacon, Headers_Beacon_SIZE, NULL);

    if (Defined(Log_DEBUG)) {
        char* content = Hex_print(&beacon, Headers_Beacon_SIZE, msg->alloc);
        Log_debug(ici->ic->logger, "RECV BEACON CONTENT[%s]", content);
    }

    struct Address addr;
    Bits_memset(&addr, 0, sizeof(struct Address));
    Bits_memcpy(addr.key, beacon.publicKey, 32);
    addr.protocolVersion = Endian_bigEndianToHost32(beacon.version_be);
    Address_getPrefix(&addr);

    String* printedAddr = NULL;
    if (Defined(Log_DEBUG)) {
        printedAddr = Address_toString(&addr, msg->alloc);
    }

    if (addr.ip6.bytes[0] != 0xfc || !Bits_memcmp(ic->ca->publicKey, addr.key, 32)) {
        Log_debug(ic->logger, "handleBeacon invalid key [%s]", printedAddr->bytes);
        return NULL;
    }

    if (!Version_isCompatible(addr.protocolVersion, Version_CURRENT_PROTOCOL)) {
        if (Defined(Log_DEBUG)) {
            Log_debug(ic->logger, "[%s] DROP beacon from [%s] which was version [%d] "
                      "our version is [%d] making them incompatable", ici->name->bytes,
                      printedAddr->bytes, addr.protocolVersion, Version_CURRENT_PROTOCOL);
        }
        return NULL;
    }

    String* beaconPass = String_newBinary(beacon.password, Headers_Beacon_PASSWORD_LEN, msg->alloc);
    int epIndex = Map_EndpointsBySockaddr_indexForKey(&lladdrInmsg, &ici->peerMap);
    if (epIndex > -1) {
        // The password might have changed!
        struct Peer* ep = ici->peerMap.values[epIndex];
        CryptoAuth_setAuth(beaconPass, NULL, ep->caSession);
        return NULL;
    }

    struct Allocator* epAlloc = Allocator_child(ici->alloc);
    struct Peer* ep = Allocator_calloc(epAlloc, sizeof(struct Peer), 1);
    struct Sockaddr* lladdr = Sockaddr_clone(lladdrInmsg, epAlloc);
    ep->alloc = epAlloc;
    ep->ici = ici;
    ep->lladdr = lladdr;
    int setIndex = Map_EndpointsBySockaddr_put(&lladdr, &ep, &ici->peerMap);
    ep->handle = ici->peerMap.handles[setIndex];
    ep->isIncomingConnection = true;
    Bits_memcpy(&ep->addr, &addr, sizeof(struct Address));
    Identity_set(ep);
    Allocator_onFree(epAlloc, closeInterface, ep);

    ep->peerLink = PeerLink_new(ic->eventBase, epAlloc);
    ep->caSession = CryptoAuth_newSession(ic->ca, epAlloc, beacon.publicKey, false, "outer");
    CryptoAuth_setAuth(beaconPass, NULL, ep->caSession);

    ep->switchIf.send = sendFromSwitch;

    if (SwitchCore_addInterface(ic->switchCore, &ep->switchIf, epAlloc, &ep->addr.path)) {
        Log_debug(ic->logger, "handleBeacon() SwitchCore out of space");
        Allocator_free(epAlloc);
        return NULL;
    }

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds(ic->eventBase) - ic->pingAfterMilliseconds - 1;

    Log_info(ic->logger, "Added peer [%s] from beacon",
        Address_toString(&ep->addr, msg->alloc)->bytes);

    // This should be safe because this is an outgoing request and we're sure the node will not
    // be relocated by moveEndpointIfNeeded()
    sendPeer(0xffffffff, PFChan_Core_PEER, ep);
    return NULL;
}

/**
 * Incoming message from someone we don't know, maybe someone responding to a beacon?
 * expects: [ struct LLAddress ][ content ]
 */
static Iface_DEFUN handleUnexpectedIncoming(struct Message* msg,
                                            struct InterfaceController_Iface_pvt* ici)
{
    struct InterfaceController_pvt* ic = ici->ic;

    struct Sockaddr* lladdr = (struct Sockaddr*) msg->bytes;
    Message_shift(msg, -lladdr->addrLen, NULL);
    if (msg->length < CryptoHeader_SIZE) {
        return NULL;
    }
    struct Allocator* epAlloc = Allocator_child(ici->alloc);
    lladdr = Sockaddr_clone(lladdr, epAlloc);

    Assert_true(!((uintptr_t)msg->bytes % 4) && "alignment fault");

    struct Peer* ep = Allocator_calloc(epAlloc, sizeof(struct Peer), 1);
    Identity_set(ep);
    ep->alloc = epAlloc;
    ep->ici = ici;
    ep->lladdr = lladdr;
    ep->alloc = epAlloc;
    ep->peerLink = PeerLink_new(ic->eventBase, epAlloc);
    struct CryptoHeader* ch = (struct CryptoHeader*) msg->bytes;
    ep->caSession = CryptoAuth_newSession(ic->ca, epAlloc, ch->publicKey, true, "outer");
    if (CryptoAuth_decrypt(ep->caSession, msg)) {
        // If the first message is a dud, drop all state for this peer.
        // probably some random crap that wandered in the socket.
        Allocator_free(epAlloc);
        return NULL;
    }
    Assert_true(!Bits_isZero(ep->caSession->herPublicKey, 32));
    Assert_true(Map_EndpointsBySockaddr_indexForKey(&lladdr, &ici->peerMap) == -1);
    int index = Map_EndpointsBySockaddr_put(&lladdr, &ep, &ici->peerMap);
    Assert_true(index >= 0);
    ep->handle = ici->peerMap.handles[index];
    Allocator_onFree(epAlloc, closeInterface, ep);
    ep->state = InterfaceController_PeerState_UNAUTHENTICATED;
    ep->isIncomingConnection = true;
    ep->switchIf.send = sendFromSwitch;

    if (SwitchCore_addInterface(ic->switchCore, &ep->switchIf, epAlloc, &ep->addr.path)) {
        Log_debug(ic->logger, "handleUnexpectedIncoming() SwitchCore out of space");
        Allocator_free(epAlloc);
        return NULL;
    }

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds(ic->eventBase) - ic->pingAfterMilliseconds - 1;

    Bits_memcpy(ep->addr.key, ep->caSession->herPublicKey, 32);
    Bits_memcpy(ep->addr.ip6.bytes, ep->caSession->herIp6, 16);
    Log_info(ic->logger, "Added peer [%s] from incoming message",
        Address_toString(&ep->addr, msg->alloc)->bytes);

    return receivedPostCryptoAuth(msg, ep, ic);
}

static Iface_DEFUN handleIncomingFromWire(struct Message* msg, struct Iface* addrIf)
{
    struct InterfaceController_Iface_pvt* ici =
        Identity_containerOf(addrIf, struct InterfaceController_Iface_pvt, pub.addrIf);

    struct Sockaddr* lladdr = (struct Sockaddr*) msg->bytes;
    if (msg->length < Sockaddr_OVERHEAD || msg->length < lladdr->addrLen) {
        Log_debug(ici->ic->logger, "DROP runt");
        return NULL;
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
    CryptoAuth_resetIfTimeout(ep->caSession);
    if (CryptoAuth_decrypt(ep->caSession, msg)) {
        return NULL;
    }
    PeerLink_recv(msg, ep->peerLink);
    return receivedPostCryptoAuth(msg, ep, ici->ic);
}

struct InterfaceController_Iface* InterfaceController_newIface(struct InterfaceController* ifc,
                                                               String* name,
                                                               struct Allocator* alloc)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) ifc);

    struct InterfaceController_Iface_pvt* ici =
        Allocator_calloc(alloc, sizeof(struct InterfaceController_Iface_pvt), 1);
    ici->name = String_clone(name, alloc);
    ici->peerMap.allocator = alloc;
    ici->ic = ic;
    ici->alloc = alloc;
    ici->pub.addrIf.send = handleIncomingFromWire;
    ici->pub.ifNum = ArrayList_OfIfaces_add(ic->icis, ici);

    Identity_set(ici);

    return &ici->pub;
}

static int freeAlloc(struct Allocator_OnFreeJob* job)
{
    struct Allocator* alloc = (struct Allocator*) job->userData;
    Allocator_free(alloc);
    return 0;
}

static void sendBeacon(struct InterfaceController_Iface_pvt* ici, struct Allocator* tempAlloc)
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

    Iface_send(&ici->pub.addrIf, msg);
}

static void beaconInterval(void* vInterfaceController)
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) vInterfaceController);

    struct Allocator* alloc = Allocator_child(ic->alloc);
    for (int i = 0; i < ic->icis->length; i++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, i);
        sendBeacon(ici, alloc);
    }
    Allocator_free(alloc);

    if (ic->beaconTimeoutAlloc) {
        Allocator_free(ic->beaconTimeoutAlloc);
    }
    ic->beaconTimeoutAlloc = Allocator_child(ic->alloc);
    Timeout_setTimeout(
        beaconInterval, ic, ic->beaconInterval, ic->eventBase, ic->beaconTimeoutAlloc);
}

int InterfaceController_beaconState(struct InterfaceController* ifc,
                                    int interfaceNumber,
                                    int newState)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) ifc);
    struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, interfaceNumber);
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
                                      String* login,
                                      String* user,
                                      struct Allocator* alloc)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) ifc);

    Assert_true(herPublicKey);
    Assert_true(password);

    struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, interfaceNumber);

    if (!ici) {
        return InterfaceController_bootstrapPeer_BAD_IFNUM;
    }

    Log_debug(ic->logger, "bootstrapPeer total [%u]", ici->peerMap.count);

    uint8_t ip6[16];
    AddressCalc_addressForPublicKey(ip6, herPublicKey);
    if (!AddressCalc_validAddress(ip6) || !Bits_memcmp(ic->ca->publicKey, herPublicKey, 32)) {
        return InterfaceController_bootstrapPeer_BAD_KEY;
    }

    struct Allocator* epAlloc = Allocator_child(ici->alloc);

    struct Sockaddr* lladdr = Sockaddr_clone(lladdrParm, epAlloc);

    // TODO(cjd): eps are created in 3 places, there should be a factory function.
    struct Peer* ep = Allocator_calloc(epAlloc, sizeof(struct Peer), 1);
    int index = Map_EndpointsBySockaddr_put(&lladdr, &ep, &ici->peerMap);
    Assert_true(index >= 0);
    ep->alloc = epAlloc;
    ep->handle = ici->peerMap.handles[index];
    ep->lladdr = lladdr;
    ep->ici = ici;
    ep->isIncomingConnection = false;
    Bits_memcpy(ep->addr.key, herPublicKey, 32);
    Address_getPrefix(&ep->addr);
    Identity_set(ep);
    Allocator_onFree(epAlloc, closeInterface, ep);
    Allocator_onFree(alloc, freeAlloc, epAlloc);

    ep->peerLink = PeerLink_new(ic->eventBase, epAlloc);
    ep->caSession = CryptoAuth_newSession(ic->ca, epAlloc, herPublicKey, false, "outer");
    CryptoAuth_setAuth(password, login, ep->caSession);
    if (user) {
        ep->caSession->displayName = String_clone(user, epAlloc);
    }

    ep->switchIf.send = sendFromSwitch;

    if (SwitchCore_addInterface(ic->switchCore, &ep->switchIf, epAlloc, &ep->addr.path)) {
        Log_debug(ic->logger, "bootstrapPeer() SwitchCore out of space");
        Allocator_free(epAlloc);
        return InterfaceController_bootstrapPeer_OUT_OF_SPACE;
    }

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds(ic->eventBase) - ic->pingAfterMilliseconds - 1;

    if (Defined(Log_INFO)) {
        struct Allocator* tempAlloc = Allocator_child(alloc);
        String* addrStr = Address_toString(&ep->addr, tempAlloc);
        Log_info(ic->logger, "Adding peer [%s] from bootstrapPeer()", addrStr->bytes);
        Allocator_free(tempAlloc);
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
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, i);
        count += ici->peerMap.count;
    }

    struct InterfaceController_PeerStats* stats =
        Allocator_calloc(alloc, sizeof(struct InterfaceController_PeerStats), count);

    int xcount = 0;
    for (int j = 0; j < ic->icis->length; j++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, j);
        for (int i = 0; i < (int)ici->peerMap.count; i++) {
            struct Peer* peer = Identity_check((struct Peer*) ici->peerMap.values[i]);
            struct InterfaceController_PeerStats* s = &stats[xcount];
            xcount++;
            Bits_memcpy(&s->addr, &peer->addr, sizeof(struct Address));
            s->bytesOut = peer->bytesOut;
            s->bytesIn = peer->bytesIn;
            s->timeOfLastMessage = peer->timeOfLastMessage;
            s->state = peer->state;
            s->isIncomingConnection = peer->isIncomingConnection;
            if (peer->caSession->displayName) {
                s->user = String_clone(peer->caSession->displayName, alloc);
            }
            struct ReplayProtector* rp = &peer->caSession->replayProtector;
            s->duplicates = rp->duplicates;
            s->lostPackets = rp->lostPackets;
            s->receivedOutOfRange = rp->receivedOutOfRange;

            struct PeerLink_Kbps kbps;
            PeerLink_kbps(peer->peerLink, &kbps);
            s->sendKbps = kbps.sendKbps;
            s->recvKbps = kbps.recvKbps;
        }
    }

    Assert_true(xcount == count);

    *statsOut = stats;
    return count;
}

void InterfaceController_resetPeering(struct InterfaceController* ifController,
                                      uint8_t herPublicKey[32])
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifController);

    for (int j = 0; j < ic->icis->length; j++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, j);
        for (int i = 0; i < (int)ici->peerMap.count; i++) {
            struct Peer* peer = ici->peerMap.values[i];
            if (!herPublicKey || !Bits_memcmp(herPublicKey, peer->caSession->herPublicKey, 32)) {
                CryptoAuth_reset(peer->caSession);
            }
        }
    }
}

int InterfaceController_disconnectPeer(struct InterfaceController* ifController,
                                       uint8_t herPublicKey[32])
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifController);

    for (int j = 0; j < ic->icis->length; j++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, j);
        for (int i = 0; i < (int)ici->peerMap.count; i++) {
            struct Peer* peer = ici->peerMap.values[i];
            if (!Bits_memcmp(herPublicKey, peer->caSession->herPublicKey, 32)) {
                Allocator_free(peer->alloc);
                return 0;
            }
        }
    }
    return InterfaceController_disconnectPeer_NOTFOUND;
}

static Iface_DEFUN incomingFromEventEmitterIf(struct Message* msg, struct Iface* eventEmitterIf)
{
    struct InterfaceController_pvt* ic =
         Identity_containerOf(eventEmitterIf, struct InterfaceController_pvt, eventEmitterIf);
    Assert_true(Message_pop32(msg, NULL) == PFChan_Pathfinder_PEERS);
    uint32_t pathfinderId = Message_pop32(msg, NULL);
    Assert_true(!msg->length);

    for (int j = 0; j < ic->icis->length; j++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, j);
        for (int i = 0; i < (int)ici->peerMap.count; i++) {
            struct Peer* peer = Identity_check((struct Peer*) ici->peerMap.values[i]);
            if (peer->state != InterfaceController_PeerState_ESTABLISHED) { continue; }
            sendPeer(pathfinderId, PFChan_Core_PEER, peer);
        }
    }
    return NULL;
}

struct InterfaceController* InterfaceController_new(struct CryptoAuth* ca,
                                                    struct SwitchCore* switchCore,
                                                    struct Log* logger,
                                                    struct EventBase* eventBase,
                                                    struct SwitchPinger* switchPinger,
                                                    struct Random* rand,
                                                    struct Allocator* allocator,
                                                    struct EventEmitter* ee)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct InterfaceController_pvt* out =
        Allocator_malloc(alloc, sizeof(struct InterfaceController_pvt));
    Bits_memcpy(out, (&(struct InterfaceController_pvt) {
        .alloc = alloc,
        .ca = ca,
        .rand = rand,
        .switchCore = switchCore,
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
                                  alloc)
            : NULL

    }), sizeof(struct InterfaceController_pvt));
    Identity_set(out);

    out->icis = ArrayList_OfIfaces_new(alloc);

    out->eventEmitterIf.send = incomingFromEventEmitterIf;
    EventEmitter_regCore(ee, &out->eventEmitterIf, PFChan_Pathfinder_PEERS);

    // Add the beaconing password.
    Random_bytes(rand, out->beacon.password, Headers_Beacon_PASSWORD_LEN);
    String strPass = { .bytes=(char*)out->beacon.password, .len=Headers_Beacon_PASSWORD_LEN };
    int ret = CryptoAuth_addUser(&strPass, String_CONST("Local Peers"), ca);
    if (ret) {
        Log_warn(logger, "CryptoAuth_addUser() returned [%d]", ret);
    }
    Bits_memcpy(out->beacon.publicKey, ca->publicKey, 32);
    out->beacon.version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);

    Timeout_setTimeout(beaconInterval, out, BEACON_INTERVAL, eventBase, alloc);

    return &out->pub;
}
