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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "crypto/AddressCalc.h"
#include "crypto/Ca.h"
#include "exception/Err.h"
#include "interface/Iface.h"
#include "net/InterfaceController.h"
#include "memory/Allocator.h"
#include "net/SwitchPinger.h"
#include "util/platform/Sockaddr.h"
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
#include "util/Kbps.h"
#include "wire/Error.h"
#include "wire/Message.h"
#include "wire/Headers.h"
#include "wire/Metric.h"
#include "wire/CryptoHeader.h"

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

/** Every 3 seconds inform the pathfinder of the current link states. */
#define LINKSTATE_UPDATE_INTERVAL 3000

// Recommended by boringtun documentation
#define HANDSHAKE_CYCLE_INTERVAL 100


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
    struct Map_EndpointsBySockaddr peerMap;
    /** The number of the next peer to try pinging, this iterates through the list of peers. */
    uint32_t lastPeerPinged;
    struct InterfaceController_pvt* ic;
    struct Allocator* alloc;
    Identity
};

struct Peer
{
    /** The interface which is registered with the switch. */
    struct Iface switchIf;

    struct Iface plaintext;
    struct Iface ciphertext;

    struct Allocator* alloc;

    Ca_Session_t* caSession;

    struct Kbps sendBw;
    struct Kbps recvBw;

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

    /**
     * The number of lost packets last time we checked.
     * _lastDrops and _lastPackets are the direct readings off of the ReplayProtector
     * so they will be reset to zero when the session resets. lastDrops and lastPackets
     * are monotonic and so probably what you want.
     */
    uint64_t _lastDrops;
    uint64_t _lastPackets;
    uint64_t lastDrops;
    uint64_t lastPackets;

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

    Ca_t* const ca;

    /** Switch for adding nodes when they are discovered. */
    struct SwitchCore* const switchCore;

    struct Random* const rand;

    struct Log* const logger;

    EventBase_t* const eventBase;

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

    // Whether or not to create sessions using the noise protocol
    const bool enableNoise;

    /** The timeout event to use for pinging potentially unresponsive neighbors. */
    struct Timeout* const pingInterval;

    /** The timeout event for updating the link state to the pathfinders. */
    struct Timeout* const linkStateInterval;

    // Timeout for noise re-handshakes
    struct Timeout* const noiseHandshakeInterval;

    /** For pinging lazy/unresponsive nodes. */
    struct SwitchPinger* const switchPinger;

    struct ArrayList_OfIfaces* icis;

    /** Temporary allocator for allocating timeouts for sending beacon messages. */
    struct Allocator* beaconTimeoutAlloc;

    /** A password which is generated per-startup and sent out in beacon messages. */
    uint8_t beaconPassword[Headers_Beacon_PASSWORD_LEN];

    struct Headers_Beacon beacon;

    uint8_t ourPubKey[32];

    Identity
};

static bool knownIncompatibleVersion(uint32_t version)
{
    if (!version) {
        return false;
    } else if (Defined(SUBNODE) && version < 21) {
        // Subnode doesn't talk to peers with less than v21
        return true;
    }
    return !Version_compatibleWithCurrent(version);
}

static void sendPeer(uint32_t pathfinderId,
                     enum PFChan_Core ev,
                     struct Peer* peer,
                     uint16_t latency)
{
    if (!peer->addr.protocolVersion || knownIncompatibleVersion(peer->addr.protocolVersion)) {
        // Don't know the protocol version, never add them
        return;
    }
    struct InterfaceController_pvt* ic = Identity_check(peer->ici->ic);
    struct Allocator* alloc = Allocator_child(ic->alloc);
    Message_t* msg = Message_new(PFChan_Node_SIZE, 512, alloc);
    struct PFChan_Node* node = (struct PFChan_Node*) Message_bytes(msg);
    Bits_memcpy(node->ip6, peer->addr.ip6.bytes, 16);
    Bits_memcpy(node->publicKey, peer->addr.key, 32);
    node->path_be = Endian_hostToBigEndian64(peer->addr.path);
    node->version_be = Endian_hostToBigEndian32(peer->addr.protocolVersion);
    if (ev != PFChan_Core_PEER_GONE) {
        Assert_true(peer->addr.protocolVersion);
        node->metric_be =
            Endian_hostToBigEndian32(Metric_IC_PEER | (latency & Metric_IC_PEER_MASK));
    } else {
        node->metric_be = Endian_hostToBigEndian32(Metric_DEAD_LINK);
    }
    Err_assert(Message_epush32be(msg, pathfinderId));
    Err_assert(Message_epush32be(msg, ev));
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
        if (knownIncompatibleVersion(resp->version)) {
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

    if (knownIncompatibleVersion(resp->version) || ep->addr.path != resp->label) {
        ep->state = InterfaceController_PeerState_INCOMPATIBLE;
        return;
    }

    if (ep->state == InterfaceController_PeerState_ESTABLISHED) {
        sendPeer(0xffffffff, PFChan_Core_PEER, ep, resp->milliseconds);
    }

    ep->timeOfLastPing = Time_currentTimeMilliseconds();

    if (Defined(Log_DEBUG)) {
        String* addr = Address_toString(&ep->addr, resp->ping->pingAlloc);
        Log_debug(ic->logger, "Received [%s] from lazy endpoint [%s], state: [%s]",
                  SwitchPinger_resultString(resp->res)->bytes, addr->bytes,
                  InterfaceController_stateString(ep->state));
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
                             String_CONST("IFACE_CNTRLR"),
                             ic->timeoutMilliseconds,
                             onPingResponse,
                             ep->alloc,
                             ic->switchPinger);

    if (!ping) {
        struct Allocator* alloc = Allocator_child(ep->alloc);
        Log_debug(ic->logger, "Sending switch ping to [%s] failed, out of ping slots",
            Address_toString(&ep->addr, alloc)->bytes);
        Allocator_free(alloc);
    } else {
        Log_debug(ic->logger, "Sending switch ping to [%s]",
            Address_toString(&ep->addr, ping->pingAlloc)->bytes);
    }

    if (ping) {
        ping->onResponseContext = ep;
    }
}

static void linkState(void* vic)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) vic);
    uint32_t msgLen = 64;
    for (int i = 0; i < ic->icis->length; i++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, i);
        msgLen += PFChan_LinkState_Entry_SIZE * ici->peerMap.count;
    }
    struct Allocator* alloc = Allocator_child(ic->alloc);
    Message_t* msg = Message_new(0, msgLen, alloc);

    for (int i = 0; i < ic->icis->length; i++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, i);
        for (uint32_t i = 0; i < ici->peerMap.count; i++) {
            struct Peer* ep = ici->peerMap.values[i];

            RTypes_CryptoStats_t stats;
            Ca_stats(ep->caSession, &stats);
            uint64_t newDrops = 0;
            // Prevents invalid number when the session resets
            if (stats.lost_packets > ep->_lastDrops) {
                newDrops = stats.lost_packets - ep->_lastDrops;
            }
            ep->_lastDrops = stats.lost_packets;
            ep->lastDrops += newDrops;

            uint64_t newPackets = 0;
            if (stats.received_packets > ep->_lastPackets) {
                newPackets = stats.received_packets - ep->_lastPackets;
            }
            ep->_lastPackets = stats.received_packets;
            ep->lastPackets += newPackets;

            struct PFChan_LinkState_Entry e = {
                .peerLabel = ep->addr.path,
                .sumOfPackets = ep->lastPackets,
                .sumOfDrops = ep->lastDrops,
                .sumOfKb = (ep->bytesIn >> 10),
            };
            Err_assert(Message_epush(msg, &e, PFChan_LinkState_Entry_SIZE));
        }
    }

    if (Message_getLength(msg)) {
        Err_assert(Message_epush32be(msg, 0xffffffff));
        Err_assert(Message_epush32be(msg, PFChan_Core_LINK_STATE));
        Iface_send(&ic->eventEmitterIf, msg);
    }
    Allocator_free(alloc);
}

static void iciPing(struct InterfaceController_Iface_pvt* ici, struct InterfaceController_pvt* ic)
{
    if (!ici->peerMap.count) { return; }
    uint64_t now = Time_currentTimeMilliseconds();

    // scan for endpoints have not sent anything recently.
    uint32_t startAt = ici->lastPeerPinged = (ici->lastPeerPinged + 1) % ici->peerMap.count;
    for (uint32_t i = startAt, count = 0; count < ici->peerMap.count;) {
        i = (i + 1) % ici->peerMap.count;
        count++;

        struct Peer* ep = ici->peerMap.values[i];

        if (knownIncompatibleVersion(ep->addr.protocolVersion)) {
            // This is a version mismatch, we have nothing to do with this node
            // but we keep the session in INCOMPATIBLE state to keep track of the
            // fact that we don't want to talk to it.
            ep->state = InterfaceController_PeerState_INCOMPATIBLE;
            continue;
        }

        uint8_t ipIfDebug[40];
        if (Defined(Log_DEBUG)) {
            Address_printIp(ipIfDebug, &ep->addr);
        }

        if (ep->addr.protocolVersion && now < ep->timeOfLastMessage + ic->pingAfterMilliseconds) {
            // It's sending traffic so leave it alone.

            // wait just a minute here !
            // There is a risk that the NodeStore somehow forgets about our peers while the peers
            // are still happily sending traffic. To break this bad cycle lets just send a PEER
            // message once per second for whichever peer is the first that we address.
            if (count == 1 && ep->state == InterfaceController_PeerState_ESTABLISHED) {
                // noisy
                //Log_debug(ic->logger, "Notifying about peer number [%d/%d] [%s]",
                //    i, ici->peerMap.count, ipIfDebug);
                sendPeer(0xffffffff, PFChan_Core_PEER, ep, 0xffff);
            }

            continue;
        }
        if (now < ep->timeOfLastPing + ic->pingAfterMilliseconds) {
            // Possibly an out-of-date node which is mangling packets, don't ping too often
            // because it causes the RumorMill to be filled with this node over and over.
            continue;
        }

        if (ep->isIncomingConnection && now > ep->timeOfLastMessage + ic->forgetAfterMilliseconds) {
            Log_debug(ic->logger, "Unresponsive peer [%s] has not responded in [%u] "
                                  "seconds, dropping connection",
                                  ipIfDebug, ic->forgetAfterMilliseconds / 1024);
            sendPeer(0xffffffff, PFChan_Core_PEER_GONE, ep, 0xffff);
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

            sendPeer(0xffffffff, PFChan_Core_PEER_GONE, ep, 0xffff);
            ep->state = InterfaceController_PeerState_UNRESPONSIVE;
        }

        Log_debug(ic->logger,
                  "Pinging %s peer [%s] lag [%u]",
                  (unresponsive ? "unresponsive" : "lazy"),
                  ipIfDebug,
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
 * This is called every PING_INTERVAL_MILLISECONDS
 */
static void pingCycle(void* vic)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) vic);
    for (int i = 0; i < ic->icis->length; i++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, i);
        iciPing(ici, ic);
    }
}

static Iface_DEFUN afterEncrypt(Message_t* msg, struct Iface* ciphertext)
{
    struct Peer* ep = Identity_containerOf(ciphertext, struct Peer, ciphertext);
    // push the lladdr...
    Err(Message_epush(msg, ep->lladdr, ep->lladdr->addrLen));

    // very noisy
    if (Defined(Log_DEBUG) && false) {
        char* printedAddr =
            Hex_print(&ep->lladdr[1], ep->lladdr->addrLen - Sockaddr_OVERHEAD, Message_getAlloc(msg));
        Log_debug(ep->ici->ic->logger, "Outgoing message to [%s]", printedAddr);
    }

    return Iface_send(&ep->ici->pub.addrIf, msg);
}

// This is directly called from SwitchCore, message is not encrypted.
static Iface_DEFUN sendFromSwitch(Message_t* msg, struct Iface* switchIf)
{
    struct Peer* ep = Identity_check((struct Peer*) switchIf);

    // Once we know it to be an incompetible version, we quarentine it
    if (knownIncompatibleVersion(ep->addr.protocolVersion)) {
        if (Defined(Log_DEBUG)) {
            Log_debug(ep->ici->ic->logger, "[%s] DROP msg to node with incompat version [%d] ",
                Address_toString(&ep->addr, Message_getAlloc(msg))->bytes, ep->addr.protocolVersion);
        }
        ep->state = InterfaceController_PeerState_INCOMPATIBLE;
        return Error(msg, "UNHANDLED");
    }

    ep->bytesOut += Message_getLength(msg);

    Kbps_accumulate(&ep->sendBw, Time_currentTimeMilliseconds(), Message_getLength(msg));

    return Iface_next(&ep->plaintext, msg); // --> afterEncrypt
}

static void closeInterface(struct Allocator_OnFreeJob* job)
{
    struct Peer* toClose = Identity_check((struct Peer*) job->userData);

    int index = Map_EndpointsBySockaddr_indexForHandle(toClose->handle, &toClose->ici->peerMap);
    if (index < 0 || toClose->ici->peerMap.values[index] != toClose) {
        // Happens if the ep was created as a result of handleUnexpectedIncoming
        return;
    }
    if (!Allocator_isFreeing(toClose->ici->ic->alloc)) {
        sendPeer(0xffffffff, PFChan_Core_PEER_GONE, toClose, 0xffff);
    }
    Log_debug(toClose->ici->ic->logger,
        "Closing interface [%d] with handle [%u]", index, toClose->handle);
    Map_EndpointsBySockaddr_remove(index, &toClose->ici->peerMap);
}

static Iface_DEFUN afterDecrypt(Message_t* msg, struct Iface* plaintext);

static struct Peer* mkEp(
    const struct Sockaddr* lladdr,
    struct InterfaceController_Iface_pvt* ici,
    uint8_t publicKey[32],
    bool authNeeded,
    const char* name,
    bool useNoise
) {
    struct Allocator* epAlloc = Allocator_child(ici->alloc);
    struct Peer* ep = Allocator_calloc(epAlloc, sizeof(struct Peer), 1);
    Identity_set(ep);
    ep->ici = ici;
    ep->lladdr = Sockaddr_clone(lladdr, epAlloc);
    ep->alloc = epAlloc;
    ep->state = InterfaceController_PeerState_UNAUTHENTICATED;
    ep->isIncomingConnection = false;
    ep->switchIf.send = sendFromSwitch;
    ep->ciphertext.send = afterEncrypt;
    ep->plaintext.send = afterDecrypt;
    useNoise = useNoise && ici->ic->enableNoise;
    ep->caSession = Ca_newSession(ici->ic->ca, epAlloc, publicKey, authNeeded, name, useNoise);
    Iface_plumb(ep->caSession->ciphertext, &ep->ciphertext);
    Iface_plumb(ep->caSession->plaintext, &ep->plaintext);
    Bits_memcpy(ep->addr.key, publicKey, 32);
    Address_getPrefix(&ep->addr);
    Allocator_onFree(epAlloc, closeInterface, ep);
    return ep;
}

static struct Peer* epFromSess(
    const struct Sockaddr* lladdr,
    struct InterfaceController_Iface_pvt* ici,
    Ca_Session_t* sess,
    Allocator_t* alloc
) {
    struct Peer* ep = Allocator_calloc(alloc, sizeof(struct Peer), 1);
    Identity_set(ep);
    ep->ici = ici;
    ep->lladdr = Sockaddr_clone(lladdr, alloc);
    ep->alloc = alloc;
    ep->state = InterfaceController_PeerState_UNAUTHENTICATED;
    ep->isIncomingConnection = true;
    ep->switchIf.send = sendFromSwitch;
    ep->ciphertext.send = afterEncrypt;
    ep->plaintext.send = afterDecrypt;
    ep->caSession = sess;
    Iface_plumb(ep->caSession->ciphertext, &ep->ciphertext);
    Iface_plumb(ep->caSession->plaintext, &ep->plaintext);
    Ca_getHerPubKey(sess, ep->addr.key);
    ep->addr.protocolVersion = Rffi_CryptoAuth2_cjdnsVer(sess);
    Address_getPrefix(&ep->addr);
    Allocator_onFree(alloc, closeInterface, ep);
    return ep;
}

static Iface_DEFUN handleConnectPeer(
    Message_t* msg,
    struct InterfaceController_pvt* ic)
{
    Log_debug(ic->logger, "Got ConnectPeer message");
    PFChan_Pathfinder_ConnectPeer_t cpt;
    if (Message_getLength(msg) < (int32_t)sizeof cpt) {
        return Error(msg, "RUNT %d", Message_getLength(msg));
    }
    Err(Message_epop(msg, &cpt, sizeof cpt));

    struct Address addr = {0};
    Assert_compileTime(sizeof cpt.pubkey == sizeof addr.key);
    Bits_memcpy(addr.key, cpt.pubkey, sizeof cpt.pubkey);
    addr.protocolVersion = cpt.version;
    Address_getPrefix(&addr);

    String* printedAddr = NULL;
    if (Defined(Log_DEBUG)) {
        printedAddr = Address_toString(&addr, Message_getAlloc(msg));
    }

    if (!AddressCalc_validAddress(addr.ip6.bytes)) {
        char* hex = Hex_print(&cpt, sizeof cpt, Message_getAlloc(msg));
        Log_debug(ic->logger, "handleConnectPeer invalid key [%s] msg: [%s]", printedAddr->bytes, hex);
        return Error(msg, "invalid key [%s]", printedAddr->bytes);
    } else if (!Bits_memcmp(ic->ourPubKey, addr.key, 32)) {
        // receive self, drop silently
        return Error(msg, "connect to self");
    }

    if (knownIncompatibleVersion(addr.protocolVersion)) {
        if (Defined(Log_DEBUG)) {
            Log_debug(ic->logger, "DROP connectPeer for [%s] which was version [%d] "
                      "our version is [%d] making them incompatable",
                      printedAddr->bytes, addr.protocolVersion, Version_CURRENT_PROTOCOL);
        }
        return Error(msg, "incompatible version");
    }

    struct Sockaddr* lladdr = NULL;
    const char fourInSixPrefix[] = "\0\0\0\0\0\0\0\0\0\0\xff\xff";
    if (Bits_memcmp(&cpt.ip, fourInSixPrefix, sizeof fourInSixPrefix)) {
        // ipv4 address
        lladdr = Sockaddr_fromBytes(&cpt.ip[12], Sockaddr_AF_INET, Message_getAlloc(msg));
    } else {
        lladdr = Sockaddr_fromBytes(cpt.ip, Sockaddr_AF_INET6, Message_getAlloc(msg));
    }
    Sockaddr_setPort(lladdr, cpt.port);
    int af = Sockaddr_getFamily(lladdr);

    // Select the appropriate interface (first one matching address type)
    struct InterfaceController_Iface_pvt* ici = NULL;
    for (int i = 0; i < ic->icis->length; i++) {
        struct InterfaceController_Iface_pvt* possibleIci = ArrayList_OfIfaces_get(ic->icis, i);
        if (possibleIci->pub.af == af) {
            ici = possibleIci;
            break;
        }
    }
    if (!ici) {
        Log_debug(ic->logger, "Ignore connectPeer because AF is %d which is unhandled", af);
        return Error(msg, "AF %d is unhandled", af);
    }

    String* pass = String_new((const char*)&cpt.password, Message_getAlloc(msg));
    String* login = String_new((const char*)&cpt.login, Message_getAlloc(msg));

    int epIndex = Map_EndpointsBySockaddr_indexForKey(&lladdr, &ici->peerMap);
    if (epIndex > -1) {
        // The password might have changed!
        struct Peer* ep = ici->peerMap.values[epIndex];
        Ca_setAuth(pass, login, ep->caSession);
        return NULL;
    }

    struct Peer* ep = mkEp(lladdr, ici, cpt.pubkey, false, "seeded_peer", false);
    int setIndex = Map_EndpointsBySockaddr_put(&ep->lladdr, &ep, &ici->peerMap);
    ep->handle = ici->peerMap.handles[setIndex];
    // We will not drop the connection over a non-reply, we'll make the seeder request to disconnect
    ep->isIncomingConnection = false;
    ep->addr.protocolVersion = addr.protocolVersion;
    Ca_setAuth(pass, login, ep->caSession);

    if (SwitchCore_addInterface(ic->switchCore, &ep->switchIf, ep->alloc, &ep->addr.path)) {
        Log_debug(ic->logger, "handleConnectPeer() SwitchCore out of space");
        return Error(msg, "UNHANDLED");
    }

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds() - ic->pingAfterMilliseconds - 1;

    Log_info(ic->logger, "Added peer [%s] from seed ",
        Address_toString(&ep->addr, Message_getAlloc(msg))->bytes);

    // Ping them immediately, this prevents beacon tests from taking 1 second each
    sendPing(ep);
    return NULL;
}

/**
 * Expects [ struct LLAddress ][ beacon ]
 */
static Iface_DEFUN handleBeacon(
    Message_t* msg,
    struct InterfaceController_Iface_pvt* ici,
    struct Sockaddr* lladdr)
{
    struct InterfaceController_pvt* ic = ici->ic;
    if (!ici->pub.beaconState) {
        // accepting beacons disabled.
        Log_debug(ic->logger, "[%s] Dropping beacon because beaconing is disabled",
                  ici->pub.name->bytes);
        return NULL;
    }

    if (Message_getLength(msg) < Headers_Beacon_SIZE) {
        Log_debug(ic->logger, "[%s] Dropping runt beacon", ici->pub.name->bytes);
        return Error(msg, "RUNT");
    }

    // clear the bcast flag
    lladdr->flags = 0;

    struct Headers_Beacon beacon;
    Err(Message_epop(msg, &beacon, Headers_Beacon_SIZE));

    if (Defined(Log_DEBUG)) {
        char* content = Hex_print(&beacon, Headers_Beacon_SIZE, Message_getAlloc(msg));
        Log_debug(ici->ic->logger, "RECV BEACON CONTENT[%s]", content);
    }

    struct Address addr = {0};
    Bits_memcpy(addr.key, beacon.publicKey, 32);
    addr.protocolVersion = Endian_bigEndianToHost32(beacon.version_be);
    Address_getPrefix(&addr);

    String* printedAddr = NULL;
    if (Defined(Log_DEBUG)) {
        printedAddr = Address_toString(&addr, Message_getAlloc(msg));
    }

    if (!AddressCalc_validAddress(addr.ip6.bytes)) {
        Log_debug(ic->logger, "handleBeacon invalid key [%s]", printedAddr->bytes);
        return Error(msg, "INVALID");
    } else if (!Bits_memcmp(ic->ourPubKey, addr.key, 32)) {
        // receive beacon from self, drop silent
        return NULL;
    }

    if (knownIncompatibleVersion(addr.protocolVersion)) {
        if (Defined(Log_DEBUG)) {
            Log_debug(ic->logger, "[%s] DROP beacon from [%s] which was version [%d] "
                      "our version is [%d] making them incompatable", ici->pub.name->bytes,
                      printedAddr->bytes, addr.protocolVersion, Version_CURRENT_PROTOCOL);
        }
        return Error(msg, "UNHANDLED");
    }

    String* beaconPass = String_newBinary(beacon.password, Headers_Beacon_PASSWORD_LEN, Message_getAlloc(msg));
    int epIndex = Map_EndpointsBySockaddr_indexForKey(&lladdr, &ici->peerMap);
    if (epIndex > -1) {
        // The password might have changed!
        struct Peer* ep = ici->peerMap.values[epIndex];
        Ca_setAuth(beaconPass, String_CONST("Local Peers"), ep->caSession);
        return NULL;
    }

    bool useNoise = addr.protocolVersion >= 22;
    struct Peer* ep = mkEp(lladdr, ici, beacon.publicKey, false, "beacon_peer", useNoise);
    int setIndex = Map_EndpointsBySockaddr_put(&ep->lladdr, &ep, &ici->peerMap);
    ep->handle = ici->peerMap.handles[setIndex];
    // We make the connection ourselves but we still consider
    // it "incoming" because we replied to a beacon
    ep->isIncomingConnection = true;
    ep->addr.protocolVersion = addr.protocolVersion;
    Ca_setAuth(beaconPass, String_CONST("Local Peers"), ep->caSession);

    if (SwitchCore_addInterface(ic->switchCore, &ep->switchIf, ep->alloc, &ep->addr.path)) {
        Log_debug(ic->logger, "handleBeacon() SwitchCore out of space");
        Allocator_free(ep->alloc);
        return Error(msg, "UNHANDLED");
    }

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds() - ic->pingAfterMilliseconds - 1;

    Log_info(ic->logger, "Added peer [%s] from beacon",
    Address_toString(&ep->addr, Message_getAlloc(msg))->bytes);

    // Ping them immediately, this prevents beacon tests from taking 1 second each
    sendPing(ep);
    return NULL;
}

static Iface_DEFUN handleIncomingFromWire(Message_t* msg, struct Iface* addrIf)
{
    struct InterfaceController_Iface_pvt* ici =
        Identity_containerOf(addrIf, struct InterfaceController_Iface_pvt, pub.addrIf);

    struct Sockaddr_storage lladdrStore;
    struct Sockaddr* lladdr = (struct Sockaddr*) &lladdrStore;
    {
        struct Sockaddr* lladdr0 = (struct Sockaddr*) Message_bytes(msg);
        if (Message_getLength(msg) < Sockaddr_OVERHEAD || Message_getLength(msg) < lladdr0->addrLen) {
            Log_debug(ici->ic->logger, "DROP runt");
            return Error(msg, "RUNT");
        }
        Err(Message_epop(msg, lladdr, lladdr0->addrLen));
    }
    Assert_true(!((uintptr_t)Message_bytes(msg) % 4) && "alignment fault");
    Assert_true(!((uintptr_t)lladdr->addrLen % 4) && "alignment fault");

    char* printedAddr = "<unknown>";
    if (Defined(Log_DEBUG)) {
        printedAddr = Hex_print(&lladdr[1], lladdr->addrLen - Sockaddr_OVERHEAD, Message_getAlloc(msg));
    }
    // noisy
    if (Defined(Log_DEBUG) && false) {
        Log_debug(ici->ic->logger, "Incoming message from [%s]", printedAddr);
    }

    if (lladdr->flags & Sockaddr_flags_BCAST) {
        return handleBeacon(msg, ici, lladdr);
    }

    if (Message_getLength(msg) < 4) {
        return Error(msg, "RUNT");
    }
    int epIndex = Map_EndpointsBySockaddr_indexForKey(&lladdr, &ici->peerMap);
    if (epIndex == -1) {
        // noise control message
        Err(Message_epush(msg, NULL, 16));
        Sockaddr_asIp6(Message_bytes(msg), lladdr);

        RTypes_CryptoAuth2_TryHandshake_Ret_t ret = { .code = 0 };
        Rffi_CryptoAuth2_tryHandshake(ici->ic->ca, msg, ici->alloc, true, &ret);

        if (ret.sess) {
            // We have a new session, setup the endpoint
            struct Peer* ep = epFromSess(lladdr, ici, ret.sess, ret.alloc);
            if (SwitchCore_addInterface(ici->ic->switchCore, &ep->switchIf, ep->alloc, &ep->addr.path)) {
                Log_debug(ici->ic->logger, "handleUnexpectedIncoming() SwitchCore out of space");
                Allocator_free(ep->alloc);
                return Error(msg, "UNHANDLED");
            }
            Assert_true(Map_EndpointsBySockaddr_indexForKey(&ep->lladdr, &ici->peerMap) == -1);
            int index = Map_EndpointsBySockaddr_put(&ep->lladdr, &ep, &ici->peerMap);
            Assert_true(index >= 0);
            ep->handle = ici->peerMap.handles[index];

            // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
            // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
            // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
            ep->timeOfLastMessage =
                Time_currentTimeMilliseconds() - ici->ic->pingAfterMilliseconds - 1;

            Log_info(ici->ic->logger, "Added peer [%s] from incoming message",
                Address_toString(&ep->addr, Message_getAlloc(msg))->bytes);

            if (ep->addr.protocolVersion) {
                // This will only work if the other end sent us their version (WG mode)
                sendPeer(0xffffffff, PFChan_Core_PEER, ep, 0xffff);
            } else {
                // We don't know their version, ping them to find out
                sendPing(ep);
            }

            if (ret.code == RTypes_CryptoAuth2_TryHandshake_Code_t_RecvPlaintext) {
                // receive the packet
                return afterDecrypt(msg, &ep->plaintext);
            }
        }

        if (ret.code == RTypes_CryptoAuth2_TryHandshake_Code_t_ReplyToPeer) {
            // Send back a reply to the node who sent us this packet
            Err(Message_epush(msg, lladdr, lladdr->addrLen));
            return Iface_next(&ici->pub.addrIf, msg);
        }

        if (ret.code == RTypes_CryptoAuth2_TryHandshake_Code_t_Error) {
            Log_debug(ici->ic->logger, "Error on unexpected packet from [%s]: [%d]",
                printedAddr, ret.err);
            return Error(msg, "DECRYPT");
        }

        if (ret.code == RTypes_CryptoAuth2_TryHandshake_Code_t_Done) {
            // Nothing to do
            return NULL;
        }

        Assert_failure("Rffi_CryptoAuth2_tryHandshake() replied [%d]", ret.code);
    }

    struct Peer* ep = Identity_check((struct Peer*) ici->peerMap.values[epIndex]);

    // Once we know it to be an incompetible version, we quarentine it
    if (knownIncompatibleVersion(ep->addr.protocolVersion)) {
        if (Defined(Log_DEBUG)) {
            Log_debug(ici->ic->logger, "[%s] DROP msg from node with incompat version [%d] ",
                Address_toString(&ep->addr, Message_getAlloc(msg))->bytes, ep->addr.protocolVersion);
        }
        ep->state = InterfaceController_PeerState_INCOMPATIBLE;
        return NULL;
    }

    Ca_resetIfTimeout(ep->caSession);

    Err(Message_epush(msg, NULL, 16));
    Sockaddr_asIp6(Message_bytes(msg), lladdr);

    return Iface_next(&ep->ciphertext, msg); // -> afterDecrypt
}

// Expects result of CryptoAuth decrypt
static Iface_DEFUN afterDecrypt(Message_t* msg, struct Iface* plaintext)
{
    struct Peer* ep = Identity_containerOf(plaintext, struct Peer, plaintext);
    struct InterfaceController_Iface_pvt* ici = Identity_check(ep->ici);
    struct InterfaceController_pvt* ic = Identity_check(ici->ic);

    enum Ca_DecryptErr err = -1;
    Err(Message_epop32h(&err, msg));
    if (err) {
        return Error(msg, "AUTHENTICATION");
    }

    Kbps_accumulate(&ep->recvBw, Time_currentTimeMilliseconds(), Message_getLength(msg));
    ep->bytesIn += Message_getLength(msg);

    int caState = Ca_getState(ep->caSession);
    if (caState != Ca_State_ESTABLISHED) {
        // prevent some kinds of nasty things which could be done with packet replay.
        // This is checking the message switch header and will drop it unless the label
        // directs it to *this* router.
        if (Message_getLength(msg) < 8 || Message_bytes(msg)[7] != 1) {
            Log_info(ic->logger, "DROP message because CA is not established.");
            return Error(msg, "UNHANDLED");
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
    } else {
        if (ep->state != caState) {
            sendPeer(0xffffffff, PFChan_Core_PEER, ep, 0xffff);
        }
        ep->timeOfLastMessage = Time_currentTimeMilliseconds();
    }
    ep->state = caState;

    Identity_check(ep);
    return Iface_next(&ep->switchIf, msg);
}

int InterfaceController_ifaceCount(struct InterfaceController* ifc)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) ifc);
    return ic->icis->length;
}

struct InterfaceController_Iface* InterfaceController_getIface(struct InterfaceController* ifc,
                                                               int ifNum)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) ifc);
    struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, ifNum);
    return (ici) ? &ici->pub : NULL;
}

struct InterfaceController_Iface* InterfaceController_newIface(struct InterfaceController* ifc,
                                                               String* name,
                                                               struct Allocator* alloc)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) ifc);

    struct InterfaceController_Iface_pvt* ici =
        Allocator_calloc(alloc, sizeof(struct InterfaceController_Iface_pvt), 1);
    ici->pub.name = String_clone(name, alloc);
    ici->peerMap.allocator = alloc;
    ici->ic = ic;
    ici->alloc = alloc;
    ici->pub.addrIf.send = handleIncomingFromWire;
    ici->pub.ifNum = ArrayList_OfIfaces_add(ic->icis, ici);
    ici->pub.af = -1;

    Identity_set(ici);

    return &ici->pub;
}

static void sendBeacon(struct InterfaceController_Iface_pvt* ici, struct Allocator* tempAlloc)
{
    if (ici->pub.beaconState < InterfaceController_beaconState_newState_SEND) {
        Log_debug(ici->ic->logger, "sendBeacon(%s) -> beaconing disabled", ici->pub.name->bytes);
        return;
    }

    Log_debug(ici->ic->logger, "sendBeacon(%s)", ici->pub.name->bytes);

    Message_t* msg = Message_new(0, 128, tempAlloc);
    Err_assert(Message_epush(msg, &ici->ic->beacon, Headers_Beacon_SIZE));

    if (Defined(Log_DEBUG)) {
        char* content = Hex_print(Message_bytes(msg), Message_getLength(msg), tempAlloc);
        Log_debug(ici->ic->logger, "SEND BEACON CONTENT[%s]", content);
    }

    struct Sockaddr sa = {
        .addrLen = Sockaddr_OVERHEAD,
        .flags = Sockaddr_flags_BCAST
    };
    Err_assert(Message_epush(msg, &sa, Sockaddr_OVERHEAD));

    Iface_send(&ici->pub.addrIf, msg);
}

static void handshakeCycle(void* vInterfaceController)
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) vInterfaceController);
    struct Allocator* alloc = Allocator_child(ic->alloc);
    for (int i = 0; i < ic->icis->length; i++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, i);
        for (uint32_t j = 0; j < ici->peerMap.count; j++) {
            struct Peer* ep = Identity_check((struct Peer*) ici->peerMap.values[j]);
            Message_t* msg = Rffi_CryptoAuth2_noiseTick(ep->caSession, alloc);
            if (msg != NULL) {
                Err_assert(Message_epush(msg, ep->lladdr, ep->lladdr->addrLen));
                Iface_send(&ep->ici->pub.addrIf, msg);
            }
        }
    }
    Allocator_free(alloc);
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
    Log_debug(ic->logger, "InterfaceController_beaconState(%s, %s)", ici->pub.name->bytes, val);
    ici->pub.beaconState = newState;
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
                                      int version)
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
    if (!AddressCalc_validAddress(ip6) || !Bits_memcmp(ic->ourPubKey, herPublicKey, 32)) {
        return InterfaceController_bootstrapPeer_BAD_KEY;
    }

    // We often don't know this, but in that case we will fallback to the old peering
    bool useNoise = version >= 22;
    struct Peer* ep = mkEp(lladdrParm, ici, herPublicKey, false, user ? user->bytes : NULL, useNoise);
    ep->addr.protocolVersion = version;

    int index = Map_EndpointsBySockaddr_put(&ep->lladdr, &ep, &ici->peerMap);
    Assert_true(index >= 0);
    ep->handle = ici->peerMap.handles[index];
    Ca_setAuth(password, login, ep->caSession);

    if (SwitchCore_addInterface(ic->switchCore, &ep->switchIf, ep->alloc, &ep->addr.path)) {
        Log_debug(ic->logger, "bootstrapPeer() SwitchCore out of space");
        Allocator_free(ep->alloc);
        return InterfaceController_bootstrapPeer_OUT_OF_SPACE;
    }

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds() - ic->pingAfterMilliseconds - 1;

    if (Defined(Log_INFO)) {
        struct Allocator* tempAlloc = Allocator_child(ep->alloc);
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

struct Sockaddr* InterfaceController_getPeerLlAddr(struct InterfaceController* ifController,
                                                   struct Allocator* alloc,
                                                   uint64_t peerLabel)
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifController);
    for (int j = 0; j < ic->icis->length; j++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, j);
        for (int i = 0; i < (int)ici->peerMap.count; i++) {
            struct Peer* peer = Identity_check((struct Peer*) ici->peerMap.values[i]);
            if (peer->addr.path != peerLabel) {
                continue;
            }
            return Sockaddr_clone(peer->lladdr, alloc);
        }
    }
    return NULL;
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

    uint32_t now = Time_currentTimeMilliseconds();
    int xcount = 0;
    for (int j = 0; j < ic->icis->length; j++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, j);
        for (int i = 0; i < (int)ici->peerMap.count; i++) {
            struct Peer* peer = Identity_check((struct Peer*) ici->peerMap.values[i]);
            struct InterfaceController_PeerStats* s = &stats[xcount];
            xcount++;
            s->ifNum = ici->pub.ifNum;
            s->lladdr = Sockaddr_clone(peer->lladdr, alloc);
            Bits_memcpy(&s->addr, &peer->addr, sizeof(struct Address));
            s->bytesOut = peer->bytesOut;
            s->bytesIn = peer->bytesIn;
            s->timeOfLastMessage = peer->timeOfLastMessage;
            s->state = peer->state;
            s->isIncomingConnection = peer->isIncomingConnection;
            s->user = Ca_getName(peer->caSession, alloc);
            RTypes_CryptoStats_t stats;
            Ca_stats(peer->caSession, &stats);
            s->duplicates = stats.duplicate_packets;
            s->receivedOutOfRange = stats.received_unexpected;
            s->noiseProto = stats.noise_proto;

            s->recvKbps = Kbps_accumulate(&peer->recvBw, now, Kbps_accumulate_NO_PACKET);
            s->sendKbps = Kbps_accumulate(&peer->sendBw, now, Kbps_accumulate_NO_PACKET);

            s->receivedPackets = peer->lastPackets;
            s->lostPackets = peer->lastDrops;
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
            if (!herPublicKey || !Bits_memcmp(herPublicKey, peer->addr.key, 32)) {
                Ca_reset(peer->caSession);
            }
        }
    }
}

int InterfaceController_disconnectPeer(struct InterfaceController* ifController,
                                       uint8_t herPublicKey[32])
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifController);

    int count = 0;
    for (int j = 0; j < ic->icis->length; j++) {
        struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, j);
        for (int i = 0; i < (int)ici->peerMap.count; i++) {
            struct Peer* peer = ici->peerMap.values[i];
            if (!Bits_memcmp(herPublicKey, peer->addr.key, 32)) {
                Allocator_free(peer->alloc);
                count++;
            }
        }
    }
    return count;
}

static Iface_DEFUN incomingFromEventEmitterIf(Message_t* msg, struct Iface* eventEmitterIf)
{
    struct InterfaceController_pvt* ic =
         Identity_containerOf(eventEmitterIf, struct InterfaceController_pvt, eventEmitterIf);
    uint32_t type = 0;
    Err(Message_epop32be(&type, msg));
    uint32_t pathfinderId = 0;
    Err(Message_epop32be(&pathfinderId, msg));
    if (type == PFChan_Pathfinder_CONNECT_PEER) {
        return handleConnectPeer(msg, ic);
    } else {
        Assert_true(type == PFChan_Pathfinder_PEERS);
        Assert_true(!Message_getLength(msg));

        for (int j = 0; j < ic->icis->length; j++) {
            struct InterfaceController_Iface_pvt* ici = ArrayList_OfIfaces_get(ic->icis, j);
            for (int i = 0; i < (int)ici->peerMap.count; i++) {
                struct Peer* peer = Identity_check((struct Peer*) ici->peerMap.values[i]);
                if (peer->state != InterfaceController_PeerState_ESTABLISHED) { continue; }
                sendPeer(pathfinderId, PFChan_Core_PEER, peer, 0xffff);
            }
        }
    }
    return NULL;
}

struct InterfaceController* InterfaceController_new(Ca_t* ca,
                                                    struct SwitchCore* switchCore,
                                                    struct Log* logger,
                                                    EventBase_t* eventBase,
                                                    struct SwitchPinger* switchPinger,
                                                    struct Random* rand,
                                                    struct Allocator* allocator,
                                                    struct EventEmitter* ee,
                                                    bool enableNoise)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct InterfaceController_pvt* out =
        Allocator_calloc(alloc, sizeof(struct InterfaceController_pvt), 1);
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
        .enableNoise = enableNoise,

        .linkStateInterval = Timeout_setInterval(
            linkState,
            out,
            LINKSTATE_UPDATE_INTERVAL,
            eventBase,
            alloc),

        .pingInterval = (switchPinger)
            ? Timeout_setInterval(pingCycle,
                                  out,
                                  PING_INTERVAL_MILLISECONDS,
                                  eventBase,
                                  alloc)
            : NULL,

        .noiseHandshakeInterval = Timeout_setInterval(
            handshakeCycle,
            out,
            HANDSHAKE_CYCLE_INTERVAL,
            eventBase,
            alloc)
    }), sizeof(struct InterfaceController_pvt));
    Identity_set(out);

    out->icis = ArrayList_OfIfaces_new(alloc);

    out->eventEmitterIf.send = incomingFromEventEmitterIf;
    EventEmitter_regCore(ee, &out->eventEmitterIf, PFChan_Pathfinder_PEERS);
    EventEmitter_regCore(ee, &out->eventEmitterIf, PFChan_Pathfinder_CONNECT_PEER);

    // Add the beaconing password.
    Random_base32(rand, out->beacon.password, Headers_Beacon_PASSWORD_LEN);
    String strPass = { .bytes=(char*)out->beacon.password, .len=Headers_Beacon_PASSWORD_LEN };
    int ret = Ca_addUser(&strPass, String_CONST("Local Peers"), ca);
    if (ret) {
        Log_warn(logger, "Ca_addUser() returned [%d]", ret);
    }

    Ca_getPubKey(ca, out->ourPubKey);
    Bits_memcpy(out->beacon.publicKey, out->ourPubKey, 32);
    if (enableNoise) {
        out->beacon.version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
    } else {
        // this is mostly here for testing, we have to lie about our protocol version
        out->beacon.version_be = Endian_hostToBigEndian32(21);
    }

    Timeout_setTimeout(beaconInterval, out, BEACON_INTERVAL, eventBase, alloc);

    return &out->pub;
}
