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
#include "net/DefaultInterfaceController.h"
#include "memory/Allocator.h"
#include "net/SwitchPinger.h"
#include "util/Base32.h"
#include "util/Bits.h"
#include "util/Time.h"
#include "util/Timeout.h"
#include "util/Identity.h"
#include "wire/Error.h"
#include "wire/Message.h"

#include <stddef.h> // offsetof


#ifndef CJDNS_MAX_PEERS
    #error CJDNS_MAX_PEERS needs to be defined.
#endif

/** After this number of milliseconds, a node will be regarded as unresponsive. */
#define UNRESPONSIVE_AFTER_MILLISECONDS 20000

#define PING_AFTER_MILLISECONDS 3000

#define PING_INTERVAL 1000

#define TIMEOUT_MILLISECONDS 2000

/*--------------------Structs--------------------*/

struct Endpoint
{
    struct Interface switchIf;

    struct Interface* cryptoAuthIf;

    /**
     * The internal interface, this is on the external side of the CryptoAuth.
     * This is wrapped by CryptoAuth and incoming packets from the CryptoAuth go to
     * receivedAfterCryptoAuth() then to the switch.
     */
    struct Interface internal;

    /** The external (network side) interface. */
    struct Interface* external;

    /** The label for this endpoint, needed to ping the endpoint. */
    uint64_t switchLabel;

    /** The handle which can be used to look up this endpoint in the endpoint set. */
    uint32_t handle;

    /** If state is UNAUTHENTICATED, the other node has not sent a single valid packet. */
    #define Endpoint_state_UNAUTHENTICATED 0

    /** In state == HANDSHAKE, a valid packet has been received but it could still be a replay . */
    #define Endpoint_state_HANDSHAKE 1

    /** In state == ESTABLISHED, we know the node at the other end is authentic. */
    #define Endpoint_state_ESTABLISHED 2

    /**
     * one of Endpoint_state_UNAUTHENTICATED, Endpoint_state_HANDSHAKE or
     * Endpoint_state_ESTABLISHED.
     * If Endpoint_state_UNAUTHENTICATED, no permanent state will be kept.
     * During transition from Endpoint_state_HANDSHAKE to Endpoint_state_ESTABLISHED, a check
     * is done for a registeration of a node which is already registered in a different switch
     * slot, if there is one and the handshake completes, it will be moved.
     */
    int state;

    /**
     * The time of the last incoming message in milliseconds, used to clear out endpoints
     * if they are not responsive.
     */
    uint64_t timeOfLastMessage;

    Identity
};

#define Map_NAME OfEndpointsByKey
#define Map_ENABLE_HANDLES
#define Map_VALUE_TYPE struct Endpoint*
#include "util/Map.h"

struct Context
{
    /** Public functions and fields for this ifcontroller. */
    struct InterfaceController pub;

    /** Used to get an endpoint by its lookup key, endpoint.internal is entered into the map. */
    struct Map_OfEndpointsByKey endpointSet;

    struct Allocator* const allocator;

    struct CryptoAuth* const ca;

    /** Switch for adding nodes when they are discovered. */
    struct SwitchCore* const switchCore;

    /** Router needed to inject newly added nodes to bootstrap the system. */
    struct RouterModule* const routerModule;

    struct Log* const logger;

    struct event_base* const eventBase;

    /** After this number of milliseconds, a neoghbor will be regarded as unresponsive. */
    uint32_t unresponsiveAfterMilliseconds;

    /** The number of milliseconds to wait before pinging. */
    uint32_t pingAfterMilliseconds;

    /** The number of milliseconds to let a ping go before timing it out. */
    uint32_t timeoutMilliseconds;

    /** The timeout event to use for pinging potentially unresponsive neighbors. */
    struct Timeout* const pingInterval;

    /** For pinging lazy/unresponsive nodes. */
    struct SwitchPinger* const switchPinger;

    /** A counter to allow for 3/4 of all pings to be skipped when a node is definitely down. */
    uint32_t pingCount;

    Identity
};

//---------------//

static inline struct Context* interfaceControllerForEndpoint(struct Endpoint* ep)
{
    return Identity_cast((struct Context*)ep->internal.senderContext);
}

static void onPingResponse(enum SwitchPinger_Result result,
                           uint64_t label,
                           String* data,
                           uint32_t millisecondsLag,
                           uint32_t version,
                           void* onResponseContext)
{
    if (SwitchPinger_Result_OK != result) {
        return;
    }
    struct Endpoint* ep = onResponseContext;
    Identity_check(ep);
    struct Context* ic = interfaceControllerForEndpoint(ep);
    struct Address addr;
    Bits_memset(&addr, 0, sizeof(struct Address));
    Bits_memcpyConst(addr.key, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
    addr.path = ep->switchLabel;
    Log_debug(ic->logger, "got switch pong from node with version [%d]", version);
    RouterModule_addNode(ic->routerModule, &addr, version);

    #ifdef Log_DEBUG
        // This will be false if it times out.
        //Assert_true(label == ep->switchLabel);
        uint8_t path[20];
        AddrTools_printPath(path, label);
        uint8_t sl[20];
        AddrTools_printPath(sl, ep->switchLabel);
        Log_debug(ic->logger, "Received [%s] from lazy endpoint [%s]  [%s]",
                  SwitchPinger_resultString(result)->bytes, path, sl);
    #endif
}

// Called from the pingInteral timeout.
static void pingCallback(void* vic)
{
    struct Context* ic = vic;
    uint64_t now = Time_currentTimeMilliseconds(ic->eventBase);
    ic->pingCount++;

    // scan for endpoints have not sent anything recently.
    for (uint32_t i = 0; i < ic->endpointSet.count; i++) {
        struct Endpoint* ep = ic->endpointSet.values[i];
        if (now > ep->timeOfLastMessage + ic->pingAfterMilliseconds) {
            #ifdef Log_DEBUG
                  uint8_t key[56];
                  Base32_encode(key, 56, CryptoAuth_getHerPublicKey(ep->cryptoAuthIf), 32);
            #endif
            if (now > ep->timeOfLastMessage + ic->unresponsiveAfterMilliseconds) {
                // Lets skip 87% of pings when they're really down.
                if (ic->pingCount % 8) {
                    continue;
                }
                Log_debug(ic->logger, "Pinging unresponsive neighbor [%s.k].", key);
            } else {
                Log_debug(ic->logger, "Pinging lazy neighbor [%s].", key);
            }

            struct SwitchPinger_Ping* ping =
                SwitchPinger_newPing(ep->switchLabel,
                                     String_CONST(""),
                                     ic->timeoutMilliseconds,
                                     onPingResponse,
                                     ic->switchPinger);

            ping->onResponseContext = ep;

            SwitchPinger_sendPing(ping);
        }
    }
}

static inline struct Endpoint* endpointForInternalInterface(struct Interface* iface)
{
    return Identity_cast((struct Endpoint*) (((char*)iface) - offsetof(struct Endpoint, internal)));
}

/** If there's already an endpoint with the same public key, merge the new one with the old one. */
static void moveEndpointIfNeeded(struct Endpoint* ep, struct Context* ic)
{
    Log_debug(ic->logger, "Checking for old sessions to merge with.");

    uint8_t* key = CryptoAuth_getHerPublicKey(ep->cryptoAuthIf);
    for (uint32_t i = 0; i < ic->endpointSet.count; i++) {
        struct Endpoint* thisEp = ic->endpointSet.values[i];
        uint8_t* thisKey = CryptoAuth_getHerPublicKey(thisEp->cryptoAuthIf);
        if (thisEp != ep && !Bits_memcmp(thisKey, key, 32)) {
            Log_info(ic->logger, "Moving endpoint to merge new session with old.");

            ep->switchLabel = thisEp->switchLabel;
            SwitchCore_swapInterfaces(&thisEp->switchIf, &ep->switchIf);
            thisEp->internal.allocator->free(thisEp->internal.allocator);
            return;
        }
    }
}

// Incoming message which has passed through the cryptoauth and needs to be forwarded to the switch.
static uint8_t receivedAfterCryptoAuth(struct Message* msg, struct Interface* cryptoAuthIf)
{
    struct Endpoint* ep = cryptoAuthIf->receiverContext;
    Identity_check(ep);
    struct Context* ic = interfaceControllerForEndpoint(ep);

    if (ep->state != Endpoint_state_ESTABLISHED) {
        if (CryptoAuth_getState(cryptoAuthIf) >= CryptoAuth_HANDSHAKE3) {
            moveEndpointIfNeeded(ep, ic);
            ep->state = Endpoint_state_ESTABLISHED;
        } else {
            ep->state = Endpoint_state_HANDSHAKE;
            // prevent some kinds of nasty things which could be done with packet replay.
            // This is checking the message switch header and will drop it unless the label
            // directs it to *this* router.
            if (msg->length < 8 || msg->bytes[7] != 1) {
                Log_info(ic->logger, "Dropping message because CA is not established.");
                return Error_NONE;
            } else {
                // When a "server" gets a new connection from a "client" the router doesn't
                // know about that client so if the client sends a packet to the server, the
                // server will be unable to handle it until the client has sent inter-router
                // communication to the server. Here we will ping the client so when the
                // server gets the ping response, it will insert the client into its table
                // and know its version.
                pingCallback(ic);
            }
        }
    }
    ep->timeOfLastMessage = Time_currentTimeMilliseconds(ic->eventBase);

    return ep->switchIf.receiveMessage(msg, &ep->switchIf);
}

// This is directly called from SwitchCore, message is not encrypted.
static uint8_t sendFromSwitch(struct Message* msg, struct Interface* switchIf)
{
    struct Endpoint* ep = switchIf->senderContext;
    Identity_check(ep);
    Assert_true(ep->switchIf.senderContext == ep);
    Assert_true(ep->internal.sendMessage);

    // This sucks but cryptoauth trashes the content when it encrypts
    // and we need to be capable of sending back a coherent error message.
    uint8_t top[255];
    uint8_t* messageBytes = msg->bytes;
    uint16_t padding = msg->padding;
    uint16_t len = (msg->length < 255) ? msg->length : 255;
    Bits_memcpy(top, msg->bytes, len);

    uint8_t ret = ep->cryptoAuthIf->sendMessage(msg, ep->cryptoAuthIf);

    // If this node is unresponsive then return an error.
    struct Context* ic = interfaceControllerForEndpoint(ep);
    uint64_t now = Time_currentTimeMilliseconds(ic->eventBase);
    if (ret || now - ep->timeOfLastMessage > ic->unresponsiveAfterMilliseconds)
    {
        msg->bytes = messageBytes;
        msg->padding = padding;
        msg->length = len;
        Bits_memcpy(msg->bytes, top, len);

        return ret ? ret : Error_UNDELIVERABLE;
    } else {
        /* Way way way too much noise
        Log_debug(ic->logger,  "Sending to neighbor, last message from this node was [%u] ms ago.",
                  (now - ep->timeOfLastMessage));
        */
    }

    return Error_NONE;
}

static void closeInterface(void* vendpoint)
{
    struct Endpoint* toClose = Identity_cast((struct Endpoint*) vendpoint);
    struct Context* ic = interfaceControllerForEndpoint(toClose);

    int index = Map_OfEndpointsByKey_indexForHandle(toClose->handle, &ic->endpointSet);
    Assert_true(index >= 0);
    Map_OfEndpointsByKey_remove(index, &ic->endpointSet);
}

/**
 * Take a message from the switch (which has already been encrypted) and prepend the key.
 * Send the result on to the actual network level interface.
 */
static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    struct Endpoint* ep = endpointForInternalInterface(iface);
    Assert_true(ep->external);
    return ep->external->sendMessage(message, ep->external);
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface);

/**
 * Add a new endpoint.
 * Called from the network interface when it is asked to make a connection or it autoconnects.
 *
 * @param key the ip/mac address to use for discriminating this endpoint.
 * @param herPublicKey the public key of the foreign node, NULL if unknown.
 * @param requireAuth if true then the other end must supply a valid password on connect.
 * @param password the password for authenticating to the other node or null if none.
 * @param externalInterface the network interface which is used to connect to this node.
 * @param ic the interface controller, a child of the memory allocator for this controller
 *           will be used for the endpoint because we want to be able to free a single
 *           endpoint without freeing the whole network interface but if the network interface
 *           is freed, we would expect all of its children to deregister.
 * @return the newly inserted endpoint, NULL if there is no space to add one.
 */
static int registerPeer(struct InterfaceController* ifController,
                        uint8_t herPublicKey[32],
                        String* password,
                        bool requireAuth,
                        struct Interface* externalInterface)
{
    struct Context* ic = Identity_cast((struct Context*) ifController);

    uint8_t ip6[16];
    if (herPublicKey) {
        AddressCalc_addressForPublicKey(ip6, herPublicKey);
        if (!AddressCalc_validAddress(ip6)) {
            return InterfaceController_registerPeer_BAD_KEY;
        }
    }

    if (ic->endpointSet.count >= CJDNS_MAX_PEERS) {
        return InterfaceController_registerPeer_OUT_OF_SPACE;
    }

    struct Allocator* epAllocator = externalInterface->allocator;
    struct Endpoint* ep = Allocator_calloc(epAllocator, sizeof(struct Endpoint), 1);
    int setIndex = Map_OfEndpointsByKey_put(&ep, &ic->endpointSet);
    ep->handle = ic->endpointSet.handles[setIndex];
    Identity_set(ep);
    epAllocator->onFree(closeInterface, ep, epAllocator);


    externalInterface->receiverContext = ep;
    externalInterface->receiveMessage = receiveMessage;


    // If the other end need not supply a valid password to connect
    // we will set the connection state to HANDSHAKE because we don't
    // want the connection to be trashed after the first invalid packet.
    if (!requireAuth) {
        ep->state = Endpoint_state_HANDSHAKE;
    }

    ep->external = externalInterface;

    Bits_memcpyConst(&ep->internal, (&(struct Interface) {
        .senderContext = ic,
        .sendMessage = sendMessage,
        .allocator = epAllocator,
        .maxMessageLength = externalInterface->maxMessageLength
    }), sizeof(struct Interface));

    struct Interface* authedIf =
        CryptoAuth_wrapInterface(&ep->internal, herPublicKey, requireAuth, true, ic->ca);

    // Always use authType 1 until something else comes along, then we'll have to refactor.
    if (password) {
        CryptoAuth_setAuth(password, 1, authedIf);
    }
    ep->cryptoAuthIf = authedIf;

    Bits_memcpyConst(&ep->switchIf, (&(struct Interface) {
        .sendMessage = sendFromSwitch,
        .senderContext = ep,
        .allocator = epAllocator
    }), sizeof(struct Interface));

    Assert_true(!SwitchCore_addInterface(&ep->switchIf, 0, &ep->switchLabel, ic->switchCore));

    authedIf->receiveMessage = receivedAfterCryptoAuth;
    authedIf->receiverContext = ep;

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
        #ifdef Log_KEYS
            uint8_t keyHex[2 * InterfaceController_KEY_SIZE + 1];
            Hex_encode(keyHex, sizeof(keyHex), key, InterfaceController_KEY_SIZE);
            Log_keys(ic->logger, "With connection identifier [%s]", keyHex);
        #endif
        // Kick the ping callback so that the node will be pinged ASAP.
        pingCallback(ic);
    }

    return 0;
}

// Get an incoming message from a network interface, doesn't matter what interface or what endpoint.
static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct Endpoint* ep = Identity_cast((struct Endpoint*) iface->receiverContext);
    uint8_t out = ep->internal.receiveMessage(msg, &ep->internal);

    if (ep->state == Endpoint_state_UNAUTHENTICATED) {
        // some random stray packet wandered in to the interface....
        // This removes all of the state associated with the endpoint.
        ep->internal.allocator->free(ep->internal.allocator);
    }

    return out;
}

struct InterfaceController* DefaultInterfaceController_new(struct CryptoAuth* ca,
                                                           struct SwitchCore* switchCore,
                                                           struct RouterModule* routerModule,
                                                           struct Log* logger,
                                                           struct event_base* eventBase,
                                                           struct SwitchPinger* switchPinger,
                                                           struct Allocator* allocator)
{
    struct Context* out = Allocator_malloc(allocator, sizeof(struct Context));
    Bits_memcpyConst(out, (&(struct Context) {
        .pub = {
            .registerPeer = registerPeer
        },
        .endpointSet = {
            .allocator = allocator
        },
        .allocator = allocator,
        .ca = ca,
        .switchCore = switchCore,
        .routerModule = routerModule,
        .logger = logger,
        .eventBase = eventBase,
        .switchPinger = switchPinger,
        .unresponsiveAfterMilliseconds = UNRESPONSIVE_AFTER_MILLISECONDS,
        .pingAfterMilliseconds = PING_AFTER_MILLISECONDS,
        .timeoutMilliseconds = TIMEOUT_MILLISECONDS,

        .pingInterval = (switchPinger)
            ? Timeout_setInterval(pingCallback, out, PING_INTERVAL, eventBase, allocator)
            : NULL

    }), sizeof(struct Context));
    Identity_set(out);
    return &out->pub;
}
