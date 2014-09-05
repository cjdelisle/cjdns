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
#include "memory/Allocator.h"
#include "net/SwitchPinger.h"
#include "util/Base32.h"
#include "util/Bits.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "util/version/Version.h"
#include "util/AddrTools.h"
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

/*--------------------Structs--------------------*/


#define Map_NAME OfIFCPeerByExernalIf
#define Map_ENABLE_HANDLES
#define Map_KEY_TYPE struct Interface*
#define Map_VALUE_TYPE struct InterfaceController_Peer*
#include "util/Map.h"

struct InterfaceController_pvt
{
    /** Public functions and fields for this ifcontroller. */
    struct InterfaceController pub;

    /** Used to get a peer by its handle. */
    struct Map_OfIFCPeerByExernalIf peerMap;

    struct Allocator* const allocator;

    struct CryptoAuth* const ca;

    /** Switch for adding nodes when they are discovered. */
    struct SwitchCore* const switchCore;

    /** Router needed to inject newly added nodes to bootstrap the system. */
    struct RouterModule* const routerModule;

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

    /** The timeout event to use for pinging potentially unresponsive neighbors. */
    struct Timeout* const pingInterval;

    /** For pinging lazy/unresponsive nodes. */
    struct SwitchPinger* const switchPinger;

    /** A password which is generated per-startup and sent out in beacon messages. */
    uint8_t beaconPassword[Headers_Beacon_PASSWORD_LEN];

    Identity
};

//---------------//

static inline struct InterfaceController_pvt* ifcontrollerForPeer(
    struct InterfaceController_Peer* ep)
{
    return Identity_check((struct InterfaceController_pvt*) ep->switchIf.senderContext);
}

static void onPingResponse(struct SwitchPinger_Response* resp, void* onResponseContext)
{
    if (SwitchPinger_Result_OK != resp->res) {
        return;
    }
    struct InterfaceController_Peer* ep =
        Identity_check((struct InterfaceController_Peer*) onResponseContext);
    struct InterfaceController_pvt* ic = ifcontrollerForPeer(ep);

    struct Address addr;
    Bits_memset(&addr, 0, sizeof(struct Address));
    Bits_memcpyConst(addr.key, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
    addr.path = ep->switchLabel;
    addr.protocolVersion = resp->version;

    #ifdef Log_DEBUG
        uint8_t addrStr[60];
        Address_print(addrStr, &addr);
        uint8_t key[56];
        Base32_encode(key, 56, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
    #endif

    if (!Version_isCompatible(Version_CURRENT_PROTOCOL, resp->version)) {
        Log_debug(ic->logger, "got switch pong from node [%s] with incompatible version [%d]",
                  key, resp->version);
    } else {
        Log_debug(ic->logger, "got switch pong from node [%s] with version [%d]",
                  key, resp->version);
    }

    if (!ep->timeOfLastPing) {
        // We've never heard from this machine before (or we've since forgotten about it)
        // This is here because we want the tests to function without the janitor present.
        // Other than that, it just makes a slightly more synchronous/guaranteed setup.
        RouterModule_getPeers(&addr, 0, 0, ic->routerModule, ic->allocator);
    } else {
        struct Node_Two* nn = RouterModule_nodeForPath(resp->label, ic->routerModule);
        if (!nn) {
            RumorMill_addNode(ic->rumorMill, &addr);
        } else if (!Node_getBestParent(nn)) {
            RouterModule_peerIsReachable(resp->label, resp->milliseconds, ic->routerModule);
        }
    }

    ep->timeOfLastPing = Time_currentTimeMilliseconds(ic->eventBase);

    #ifdef Log_DEBUG
        // This will be false if it times out.
        //Assert_true(label == ep->switchLabel);
        uint8_t path[20];
        AddrTools_printPath(path, resp->label);
        uint8_t sl[20];
        AddrTools_printPath(sl, ep->switchLabel);
        Log_debug(ic->logger, "Received [%s] from lazy endpoint [%s]  [%s]",
                  SwitchPinger_resultString(resp->res)->bytes, path, sl);
    #endif
}

/*
 * Send a ping packet to one of the endpoints.
 */
static void sendPing(struct InterfaceController_Peer* ep)
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

    #ifdef Log_DEBUG
        uint8_t key[56];
        Base32_encode(key, 56, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
    #endif
    if (!ping) {
        Log_debug(ic->logger, "Failed to ping [%s.k], out of ping slots", key);
        return;
    } else {
        Log_debug(ic->logger, "SwitchPing [%s.k]", key);
    }

    ping->onResponseContext = ep;
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
    if (!ic->peerMap.count) { return; }

    uint64_t now = Time_currentTimeMilliseconds(ic->eventBase);

    // scan for endpoints have not sent anything recently.
    uint32_t startAt = Random_uint32(ic->rand) % ic->peerMap.count;
    for (uint32_t i = startAt, count = 0; !count || i != startAt;) {
        i = (i + 1) % ic->peerMap.count;
        count++;

        struct InterfaceController_Peer* ep = ic->peerMap.values[i];

        if (now < ep->timeOfLastMessage + ic->pingAfterMilliseconds) {
            if (now < ep->timeOfLastPing + ic->pingAfterMilliseconds) {
                // Possibly an out-of-date node which is mangling packets, don't ping too often
                // because it causes the RumorMill to be filled with this node over and over.
                continue;
            }
            // This is here because of a pathological state where the connection is in ESTABLISHED
            // state but the *direct peer* has somehow been dropped from the routing table
            // usually because of a call to NodeStore_brokenPath()
            struct Node_Two* peerNode = RouterModule_nodeForPath(ep->switchLabel, ic->routerModule);
            // It exists, it's parent is the self-node, and it's label is equal to the switchLabel.
            if (peerNode
                && Node_getBestParent(peerNode)
                && Node_getBestParent(peerNode)->parent->address.path == 1
                && Node_getBestParent(peerNode)->cannonicalLabel == ep->switchLabel)
            {
                continue;
            }
        }

        #ifdef Log_DEBUG
              uint8_t key[56];
              Base32_encode(key, 56, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
        #endif

        if (ep->isIncomingConnection
            && now > ep->timeOfLastMessage + ic->forgetAfterMilliseconds)
        {
            Log_debug(ic->logger, "Unresponsive peer [%s.k] has not responded in [%u] "
                                  "seconds, dropping connection",
                                  key, ic->forgetAfterMilliseconds / 1024);
            Allocator_free(ep->external->allocator);
            continue;
        }

        bool unresponsive = (now > ep->timeOfLastMessage + ic->unresponsiveAfterMilliseconds);
        if (unresponsive) {
            // flush the peer from the table...
            RouterModule_brokenPath(ep->switchLabel, ic->routerModule);

            // Lets skip 87% of pings when they're really down.
            if (ep->pingCount % 8) {
                ep->pingCount++;
                continue;
            }

            ep->state = InterfaceController_PeerState_UNRESPONSIVE;
        }

        #ifdef Log_DEBUG
            uint32_t lag = (now - ep->timeOfLastMessage) / 1024;
            Log_debug(ic->logger,
                      "Pinging %s peer [%s.k] lag [%u]",
                      (unresponsive ? "unresponsive" : "lazy"), key, lag);
        #endif

        sendPing(ep);

        // we only ping one node
        return;
    }
}

/** If there's already an endpoint with the same public key, merge the new one with the old one. */
static void moveEndpointIfNeeded(struct InterfaceController_Peer* ep,
                                 struct InterfaceController_pvt* ic)
{
    Log_debug(ic->logger, "Checking for old sessions to merge with.");

    uint8_t* key = CryptoAuth_getHerPublicKey(ep->cryptoAuthIf);
    for (uint32_t i = 0; i < ic->peerMap.count; i++) {
        struct InterfaceController_Peer* thisEp = ic->peerMap.values[i];
        uint8_t* thisKey = CryptoAuth_getHerPublicKey(thisEp->cryptoAuthIf);
        if (thisEp != ep && !Bits_memcmp(thisKey, key, 32)) {
            Log_info(ic->logger, "Moving endpoint to merge new session with old.");

            // flush out the new entry if needed.
            RouterModule_brokenPath(ep->switchLabel, ic->routerModule);
            ep->switchLabel = thisEp->switchLabel;
            SwitchCore_swapInterfaces(&thisEp->switchIf, &ep->switchIf);
            Allocator_free(thisEp->external->allocator);
            return;
        }
    }
}

// Incoming message which has passed through the cryptoauth and needs to be forwarded to the switch.
static uint8_t receivedAfterCryptoAuth(struct Message* msg, struct Interface* cryptoAuthIf)
{
    struct InterfaceController_Peer* ep =
        Identity_check((struct InterfaceController_Peer*) cryptoAuthIf->receiverContext);
    struct InterfaceController_pvt* ic = ifcontrollerForPeer(ep);

    ep->bytesIn += msg->length;

    int caState = CryptoAuth_getState(cryptoAuthIf);
    if (ep->state < InterfaceController_PeerState_ESTABLISHED) {
        // EP states track CryptoAuth states...
        ep->state = caState;
        if (caState == CryptoAuth_ESTABLISHED) {
            moveEndpointIfNeeded(ep, ic);
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
    struct InterfaceController_Peer* ep =
        Identity_check((struct InterfaceController_Peer*) switchIf);

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
    struct InterfaceController_Peer* toClose =
        Identity_check((struct InterfaceController_Peer*) job->userData);

    struct InterfaceController_pvt* ic = ifcontrollerForPeer(toClose);

    // flush the peer from the table...
    RouterModule_brokenPath(toClose->switchLabel, ic->routerModule);

    int index = Map_OfIFCPeerByExernalIf_indexForHandle(toClose->handle, &ic->peerMap);
    Assert_true(index >= 0);
    Map_OfIFCPeerByExernalIf_remove(index, &ic->peerMap);
    return 0;
}

int InterfaceController_registerPeer(struct InterfaceController* ifController,
                                     uint8_t herPublicKey[32],
                                     String* password,
                                     bool requireAuth,
                                     bool isIncomingConnection,
                                     struct Interface* externalInterface)
{
    // This function is overridden by some tests...
    if (ifController->registerPeer) {
        return ifController->registerPeer(ifController, herPublicKey, password, requireAuth,
                                          isIncomingConnection, externalInterface);
    }

    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifController);

    if (Map_OfIFCPeerByExernalIf_indexForKey(&externalInterface, &ic->peerMap) > -1) {
        return 0;
    }

    Log_debug(ic->logger, "registerPeer [%p] total [%u]",
              (void*)externalInterface, ic->peerMap.count);

    uint8_t ip6[16];
    if (herPublicKey) {
        AddressCalc_addressForPublicKey(ip6, herPublicKey);
        if (!AddressCalc_validAddress(ip6)) {
            return InterfaceController_registerPeer_BAD_KEY;
        }

        if (!Bits_memcmp(ic->ca->publicKey, herPublicKey, 32)) {
            // can't link with yourself, wiseguy
            return InterfaceController_registerPeer_BAD_KEY;
        }
    } else {
        Assert_true(requireAuth);
    }

    struct Allocator* epAllocator = externalInterface->allocator;
    struct InterfaceController_Peer* ep =
        Allocator_calloc(epAllocator, sizeof(struct InterfaceController_Peer), 1);
    ep->bytesOut = 0;
    ep->bytesIn = 0;
    ep->external = externalInterface;
    int setIndex = Map_OfIFCPeerByExernalIf_put(&externalInterface, &ep, &ic->peerMap);
    ep->handle = ic->peerMap.handles[setIndex];
    Identity_set(ep);
    Allocator_onFree(epAllocator, closeInterface, ep);

    // If the other end need needs to supply a valid password to connect
    // we will set the connection state to UNAUTHENTICATED so that if the
    // packet is invalid, the connection will be dropped right away.
    if (requireAuth) {
        ep->state = InterfaceController_PeerState_UNAUTHENTICATED;
    }

    ep->cryptoAuthIf = CryptoAuth_wrapInterface(externalInterface,
                                                herPublicKey,
                                                NULL,
                                                requireAuth,
                                                "outer",
                                                ic->ca);

    ep->cryptoAuthIf->receiveMessage = receivedAfterCryptoAuth;
    ep->cryptoAuthIf->receiverContext = ep;

    // Always use authType 1 until something else comes along, then we'll have to refactor.
    if (password) {
        CryptoAuth_setAuth(password, 1, ep->cryptoAuthIf);
    }

    ep->isIncomingConnection = isIncomingConnection;

    Bits_memcpyConst(&ep->switchIf, (&(struct Interface) {
        .sendMessage = sendFromSwitch,

        // ifcontrollerForPeer uses this.
        // sendFromSwitch relies on the fact that the
        // switchIf is the same memory location as the Peer.
        .senderContext = ic,

        .allocator = epAllocator
    }), sizeof(struct Interface));

    int ret = SwitchCore_addInterface(&ep->switchIf, 0, &ep->switchLabel, ic->switchCore);
    if (ret) {
        return (ret == SwitchCore_addInterface_OUT_OF_SPACE)
            ? InterfaceController_registerPeer_OUT_OF_SPACE
            : InterfaceController_registerPeer_INTERNAL;
    }

    // We want the node to immedietly be pinged but we don't want it to appear unresponsive because
    // the pinger will only ping every (PING_INTERVAL * 8) so we set timeOfLastMessage to
    // (now - pingAfterMilliseconds - 1) so it will be considered a "lazy node".
    ep->timeOfLastMessage =
        Time_currentTimeMilliseconds(ic->eventBase) - ic->pingAfterMilliseconds - 1;

    if (herPublicKey) {
        #ifdef Log_INFO
            uint8_t printAddr[60];
            AddrTools_printIp(printAddr, ip6);
            Log_info(ic->logger, "Adding peer [%s]", printAddr);
        #endif
    } else {
        Log_info(ic->logger, "Adding peer with unknown key");
    }

    // We can't just add the node directly to the routing table because we do not know
    // the version. We'll send it a switch ping and when it responds, we will know it's
    // key (if we don't already) and version number.
    sendPing(ep);

    return 0;
}

struct InterfaceController_Peer* InterfaceController_getPeer(struct InterfaceController* ifc,
                                                             struct Interface* iface)
{
    if (ifc->getPeer) { return ifc->getPeer(ifc, iface); }
    struct Interface* cryptoAuthIf = CryptoAuth_getConnectedInterface(iface);
    return Identity_check((struct InterfaceController_Peer*) cryptoAuthIf->receiverContext);
}

void InterfaceController_populateBeacon(struct InterfaceController* ifc,
                                        struct Headers_Beacon* beacon)
{
    struct InterfaceController_pvt* ic = Identity_check((struct InterfaceController_pvt*) ifc);
    beacon->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
    Bits_memcpyConst(beacon->password, ic->beaconPassword, Headers_Beacon_PASSWORD_LEN);
    Bits_memcpyConst(beacon->publicKey, ic->ca->publicKey, 32);
}

int InterfaceController_getPeerStats(struct InterfaceController* ifController,
                                     struct Allocator* alloc,
                                     struct InterfaceController_peerStats** statsOut)
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifController);
    int count = ic->peerMap.count;
    struct InterfaceController_peerStats* stats =
        Allocator_malloc(alloc, sizeof(struct InterfaceController_peerStats)*count);

    for (int i = 0; i < count; i++) {
        struct InterfaceController_Peer* peer = ic->peerMap.values[i];
        struct InterfaceController_peerStats* s = &stats[i];
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

    *statsOut = stats;
    return count;
}

int InterfaceController_disconnectPeer(struct InterfaceController* ifController,
                                       uint8_t herPublicKey[32])
{
    struct InterfaceController_pvt* ic =
        Identity_check((struct InterfaceController_pvt*) ifController);

    for (uint32_t i = 0; i < ic->peerMap.count; i++) {
        struct InterfaceController_Peer* peer = ic->peerMap.values[i];
        if (!Bits_memcmp(herPublicKey, CryptoAuth_getHerPublicKey(peer->cryptoAuthIf), 32)) {
          Allocator_free(peer->external->allocator);
          return 0;
        }
    }
    return InterfaceController_disconnectPeer_NOTFOUND;
}

struct InterfaceController* InterfaceController_new(struct CryptoAuth* ca,
                                                    struct SwitchCore* switchCore,
                                                    struct RouterModule* routerModule,
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
        .peerMap = {
            .allocator = allocator
        },
        .allocator = allocator,
        .ca = ca,
        .rand = rand,
        .switchCore = switchCore,
        .routerModule = routerModule,
        .rumorMill = rumorMill,
        .logger = logger,
        .eventBase = eventBase,
        .switchPinger = switchPinger,
        .unresponsiveAfterMilliseconds = UNRESPONSIVE_AFTER_MILLISECONDS,
        .pingAfterMilliseconds = PING_AFTER_MILLISECONDS,
        .timeoutMilliseconds = TIMEOUT_MILLISECONDS,
        .forgetAfterMilliseconds = FORGET_AFTER_MILLISECONDS,

        .pingInterval = (switchPinger)
            ? Timeout_setInterval(pingCallback,
                                  out,
                                  PING_INTERVAL_MILLISECONDS,
                                  eventBase,
                                  allocator)
            : NULL

    }), sizeof(struct InterfaceController_pvt));
    Identity_set(out);

    // Add the beaconing password.
    Random_bytes(rand, out->beaconPassword, Headers_Beacon_PASSWORD_LEN);
    String strPass = { .bytes=(char*)out->beaconPassword, .len=Headers_Beacon_PASSWORD_LEN };
    int ret = CryptoAuth_addUser(&strPass, 1, String_CONST("Local Peers"), ca);
    if (ret) {
        Log_warn(logger, "CryptoAuth_addUser() returned [%d]", ret);
    }

    return &out->pub;
}
