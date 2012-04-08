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

#include "crypto/CryptoAuth_struct.h"
#include "interface/InterfaceController.h"
#include "interface/InterfaceMap.h"
#include "memory/Allocator.h"
#include "util/Time.h"
#include "wire/Error.h"
#include "wire/Message.h"


/** Maximum number of interfaces (should be same as number of switch slots). */
#define MAX_INTERFACES 255


/*--------------------Structs--------------------*/

struct Endpoint
{
    /**
     * The internal interface, this is on the external side of the CryptoAuth.
     * This is wrapped by CryptoAuth and incoming packets from the CryptoAuth go to
     * receivedAfterCryptoAuth() then to the switch.
     */
    struct Interface internal;

    /** The switch's receiverMessage callback, needed for adding receivedAfterCryptoAuth(). */
    Interface_CALLBACK(switchReceiveMessage);

    /** The switch's receiverContext, needed for adding receivedAfterCryptoAuth(). */
    void* switchReceiverContext;

    struct Interface* cryptoAuthIf;

    /**
     * The external (network side) interface,
     * this is the same for all endpoints using this network module.
     */
    struct Interface* external;

    /** The lookup key for this endpoint (ip/mac address) */
    uint8_t key[InterfaceController_KEY_SIZE];

    /**
     * True after the CryptoAuth handshake is completed.
     * This is used to check for a registeration of a node which is already registered in a
     * different switch slot, if there is one and the handshake completes, it will be moved.
     */
    bool authenticated : 1;

    /** If true then the entry will be removed if it sits too long with no incoming message. */
    bool removeIfExpires : 1;

    /**
     * The time of the last incoming message, used to clear out endpoints
     * if they are not responsive. Only updated if removeIfExpires == true.
     */
    uint32_t timeOfLastMessage;
};

struct InterfaceController
{
    /** Used to get an endpoint by it's lookup key, endpoint.internal is entered into the map. */
    struct InterfaceMap* imap;

    struct Endpoint endpoints[MAX_INTERFACES];

    struct Allocator* allocator;

    struct CryptoAuth* ca;

    /** Switch for adding nodes when they are discovered. */
    struct SwitchCore* switchCore;

    /** Router needed to inject newly added nodes to bootstrap the system. */
    struct RouterModule* routerModule;

    struct Log* logger;

    struct event_base* eventBase;
};

struct InterfaceController* InterfaceController_new(struct CryptoAuth* ca,
                                                    struct SwitchCore* switchCore,
                                                    struct RouterModule* routerModule,
                                                    struct Log* logger,
                                                    struct Allocator* allocator)
{
    struct InterfaceController* ic =
        allocator->calloc(sizeof(struct InterfaceController), 1, allocator);
    ic->imap = InterfaceMap_new(InterfaceController_KEY_SIZE, allocator);
    ic->allocator = allocator;
    ic->ca = ca;
    ic->switchCore = switchCore;
    ic->routerModule = routerModule;
    ic->logger = logger;
    return ic;
}

static inline struct Endpoint* endpointForInternalInterface(struct Interface* iface)
{
    return (struct Endpoint*) (((char*)iface) - offsetof(struct Endpoint, internal));
}

/** If there's already an endpoint with the same public key, merge the new one with the old one. */
static inline void moveEndpointIfNeeded(struct Endpoint** epPtr,
                                        struct InterfaceController* ic)
{
    Log_debug(ic->logger, "Checking for old sessions to merge with.");
    struct Endpoint* ep = *epPtr;
    uint8_t* key = CryptoAuth_getHerPublicKey(ep->cryptoAuthIf);
    for (int i = 0; i < MAX_INTERFACES; i++) {
        struct Endpoint* thisEp = &ic->endpoints[i];
        if (thisEp == ep) {
            return;
        }
        if (thisEp->authenticated
            && !memcmp(CryptoAuth_getHerPublicKey(thisEp->cryptoAuthIf), key, 32))
        {
            Log_info(ic->logger, "Moving endpoint to merge new session with old.");
            /* just pretending, trying to isolate a segfault bug.
            memcpy(thisEp->key, ep->key, InterfaceController_KEY_SIZE);

            // This is a mess.
            // We copy the CryptoAuth session over so the user is able to `hijack'
            // their old session seamlessly.
            memcpy(thisEp->cryptoAuthIf->receiverContext,
                   ep->cryptoAuthIf->receiverContext,
                   sizeof(struct CryptoAuth_Wrapper));

            *epPtr = thisEp;*/

            // This is probably causing the crash bug.
            //ep->cryptoAuthIf->allocator->free(ep->cryptoAuthIf->allocator);
            return;
        }
    }
}

// Incoming message which has passed through the cryptoauth and needs to be forwarded to the switch.
static uint8_t receivedAfterCryptoAuth(struct Message* msg, struct Interface* cryptoAuthIf)
{
    struct Endpoint* ep = cryptoAuthIf->receiverContext;
    struct InterfaceController* ic = ep->internal.senderContext;

    if (ep->removeIfExpires) {
        ep->timeOfLastMessage = Time_currentTimeSeconds(ic->eventBase);
    }
    if (!ep->authenticated) {
        if (CryptoAuth_getState(cryptoAuthIf) == CryptoAuth_ESTABLISHED) {
            moveEndpointIfNeeded(&ep, ic);
            ep->authenticated = true;
        }
    }

    // yet another hack to make this asymmetric flow thing work.
    return ep->switchReceiveMessage(msg, &(struct Interface) {
        .receiverContext = ep->switchReceiverContext
    });
}

static void closeInterface(void* vendpoint)
{
    struct Endpoint* toClose = (struct Endpoint*) vendpoint;
    struct InterfaceController* ic = toClose->internal.senderContext;

    int index = InterfaceMap_indexOf(toClose->key, ic->imap);
    InterfaceMap_remove(index, ic->imap);

    // make sure not to reorder the entries because InterfaceMap points to them.
    memset(toClose, 0, sizeof(struct Endpoint));
}

/**
 * Take a message from the switch (which has already been encrypted) and prepend the key.
 * Send the result on to the actual network level interface.
 */
static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    struct Endpoint* ep = endpointForInternalInterface(iface);

    Message_shift(message, InterfaceController_KEY_SIZE);
    memcpy(message->bytes, ep->key, InterfaceController_KEY_SIZE);

    return ep->external->sendMessage(message, ep->external);
}

static inline struct Endpoint* getEndpoint(uint8_t key[InterfaceController_KEY_SIZE],
                                           struct InterfaceController* ic)
{
    int index = InterfaceMap_indexOf(key, ic->imap);
    if (index > -1) {
        struct Endpoint* ep = endpointForInternalInterface(ic->imap->interfaces[index]);
        #ifdef Log_DEBUG
            assert(ep->external || !"Entry was not removed from the map but was null.");
            assert(!memcmp(key, ep->key, InterfaceController_KEY_SIZE));
        #endif
        return ep;
    }
    return NULL;
}

/**
 * Allow for a configurable which makes different network interfaces require or not require auth.
 * An IPv4 interface might require auth to connect while an 802.11 interface
 * allows anyone to connect.
 */
static inline bool requiresAuth(struct Interface* networkInterface, struct InterfaceController* ic)
{
    // TODO: add configuration.
    return true;
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
 *           is freed, we would expect all of it's children to deregister.
 * @return the newly inserted endpoint, NULL if there is no space to add one.
 */
static struct Endpoint* insertEndpoint(uint8_t key[InterfaceController_KEY_SIZE],
                                       uint8_t herPublicKey[32],
                                       bool requireAuth,
                                       String* password,
                                       struct Interface* externalInterface,
                                       struct InterfaceController* ic)
{
    // scan for an unused endpoint slot.
    struct Endpoint* ep = NULL;
    for (int i = 0; i < MAX_INTERFACES; i++) {
        if (ic->endpoints[i].external == NULL) {
            ep = &ic->endpoints[i];
            break;
        }
    }
    if (!ep) {
        return NULL;
    }

    struct Allocator* epAllocator =
        externalInterface->allocator->child(externalInterface->allocator);
    epAllocator->onFree(closeInterface, &ep, epAllocator);

    externalInterface->receiverContext = ic;
    externalInterface->receiveMessage = receiveMessage;

    ep->external = externalInterface;
    memcpy(&ep->key, key, InterfaceController_KEY_SIZE);
    InterfaceMap_put(key, &ep->internal, 0, ic->imap);

    // Outgoing:   switch -> cryptoAuth -> sendMessage() -> external
    // Incoming:   external -> receiveMessage() -> cryptoAuth -> postCryptoAuth() -> switch

    memcpy(&ep->internal, (&(struct Interface) {
        .senderContext = ep,
        .sendMessage = sendMessage,
        .allocator = epAllocator,
        .maxMessageLength = externalInterface->maxMessageLength,
        .requiredPadding = InterfaceController_KEY_SIZE + externalInterface->requiredPadding
    }), sizeof(struct Interface));

    struct Interface* authedIf =
        CryptoAuth_wrapInterface(&ep->internal, herPublicKey, requireAuth, true, ic->ca);

    // Always use authType 1 until something else comes along, then we'll have to refactor.
    if (password) {
        CryptoAuth_setAuth(password, 1, authedIf);
    }
    ep->cryptoAuthIf = authedIf;

    struct Address addr;
    memset(&addr, 0, sizeof(struct Address));
    if (SwitchCore_addInterface(authedIf, 0, &addr.networkAddress_be, ic->switchCore)) {
        return NULL;
    }


    // Hack:
    // Store switchReceiverContext and switchReceiveMessage in the ep and change them to add
    // another level of indirection on the incoming side but not on the outgoing side.
    ep->switchReceiveMessage = authedIf->receiveMessage;
    ep->switchReceiverContext = authedIf->receiverContext;
    authedIf->receiveMessage = receivedAfterCryptoAuth;
    authedIf->receiverContext = ep;


    if (herPublicKey) {
        memcpy(addr.key, herPublicKey, 32);
        RouterModule_addNode(&addr, ic->routerModule);
        #ifdef Log_INFO
            uint8_t printAddr[60];
            Address_print(printAddr, &addr);
            Log_info1(ic->logger, "Adding direct peer %s.", printAddr);
        #endif
        #ifdef Log_KEYS
            uint8_t keyHex[2 * InterfaceController_KEY_SIZE + 1];
            Hex_encode(keyHex, sizeof(keyHex), key, InterfaceController_KEY_SIZE);
            Log_keys1(ic->logger, "With connection identifier [%s]", keyHex);
        #endif
    }

    return ep;
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct InterfaceController* ic = iface->receiverContext;
    struct Endpoint* ep = getEndpoint(msg->bytes, ic);
    if (ep) {
        Message_shift(msg, -InterfaceController_KEY_SIZE);
        return ep->internal.receiveMessage(msg, &ep->internal);
    }

    // Not a known peer, add them.
    ep = insertEndpoint(msg->bytes, NULL, requiresAuth(iface, ic), NULL, iface, ic);

    if (!ep) {
        Log_warn(ic->logger, "Could not insert endpoint, out of space in switch.");
        return Error_NONE;
    } else {
        #ifdef Log_KEYS
            uint8_t keyHex[2 * InterfaceController_KEY_SIZE + 1];
            Hex_encode(keyHex, sizeof(keyHex), msg->bytes, InterfaceController_KEY_SIZE);
            Log_keys1(ic->logger, "Got incoming connection request from [%s]", keyHex);
        #else
            Log_info(ic->logger, "Got incoming connection request.");
        #endif
    }

    // Send the message on to the switch so the first message isn't lost.
    Message_shift(msg, -InterfaceController_KEY_SIZE);
    return ep->internal.receiveMessage(msg, &ep->internal);
}

int InterfaceController_insertEndpoint(uint8_t key[InterfaceController_KEY_SIZE],
                                       uint8_t herPublicKey[32],
                                       String* password,
                                       struct Interface* externalInterface,
                                       struct InterfaceController* ic)
{
    return (insertEndpoint(key, herPublicKey, false, password, externalInterface, ic)) ? 0 : -1;
}

void InterfaceController_registerInterface(struct Interface* externalInterface,
                                           struct InterfaceController* ic)
{
    externalInterface->receiverContext = ic;
    externalInterface->receiveMessage = receiveMessage;
}
