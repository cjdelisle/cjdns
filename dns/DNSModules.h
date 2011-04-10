#ifndef DNSMODULES_H
#define DNSMODULES_H

#include <event2/dns_struct.h>

#include "libbenc/benc.h"

struct DNSMessage {
    struct evdns_server_request* request;
    int returnCode;
};

struct DNSModule;
struct DNSModuleRegistry;

/**
 * This represents a DNS module.
 * Pass one of these to DNSModule_register() and it
 * will handle DNS requests and responses.
 */
struct DNSModule {
    /**
     * A user friendly null terminated string which will be used to
     * manipulate the module using the DNSModules API.
     */
    const char* const name;

    /**
     * The module's state.
     */
    void* const context;

    /**
     * Serialize the content of the context into a bencoded object.
     *
     * @param module the module's state.
     * @return a serialized form of module.
     */
    benc_bstr_t* (* const serialize)(struct DNSModule* module);

    /**
     * Deserialize the context from a bencoded object.
     *
     * @param serialData the same data which was output by
     *                   moduleContext_serialize
     *                   this will not be freed until after
     *                   moduleContext_free is called so it is safe to
     *                   point to items in this.
     *
     * @param module the existing module to add content to.
     */
    void (* const deserialize)(const benc_bstr_t serialData,
                               struct DNSModule* module);

    /**
     * @param the message which came in from a peer.
     * @param context the module's state.
     * @return 1 if a response should be sent for this message.
     *         -1 if the message is known invalid and should not be passed
     *            to any more handlers.
     */
    int (* const handleIncoming)(struct DNSMessage* message,
                                 struct DNSModule* module,
                                 const struct DNSModuleRegistry* registry);

    /**
     * @param message the message which will be sent to the peer.
     * @param context the module's state.
     * @return -1 if the message should not be propigated to any more modules.
     *            use with caution as it may be interpreted as network loss.
     */
    int (* const handleOutgoing)(struct DNSMessage* message,
                                 struct DNSModule* module,
                                 const struct DNSModuleRegistry* registry);
};

/** State of the registry. */
struct DNSModuleRegistry {

    /** Number of members. */
    int memberCount;

    /** A null terminated list of pointers to members. */
    struct DNSModule** members;

    /**
     * A list of serialized contexts to be
     * deserialized when the modules are loaded.
     */
    bobj_t* serializedContexts;

};

/**
 * @return a new (empty) registry.
 * @see DNSModules_free()
 */
struct DNSModuleRegistry* DNSModules_new();

/**
 * @param registry the module registry to remove.
 * @see DNSModules_new()
 */
void DNSModules_free(struct DNSModuleRegistry* registry);

/**
 * Register an event handler.
 *
 * @param module the module to register.
 * @return 0 if everything went well.
 */
int DNSModules_register(struct DNSModule* module,
                        struct DNSModuleRegistry* registry);

/**
 * handleIncoming starts by running the last module registered
 * and working back. It is assumed that the core modules will
 * be registered first and will be the ones to initiate the
 * response.
 * The last module registered must be the one with access to
 * the network.
 *
 * @see DNSModule->handleIncoming()
 */
void DNSModules_handleIncoming(struct DNSMessage* message,
                               const struct DNSModuleRegistry* registry);

/**
 * @see DNSModule->handleOutgoing()
 */
void DNSModules_handleOutgoing(struct DNSMessage* message,
                               const struct DNSModuleRegistry* registry);

#endif
