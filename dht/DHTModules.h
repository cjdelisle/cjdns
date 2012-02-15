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
#ifndef DHTModules_H
#define DHTModules_H

#include "memory/Allocator.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "benc/Object.h"

/**
 * Maximum number of bytes in a message.
 * Ethernet MTU is 1500 so it's hard to imagine much more.
 */
#define DHTModules_MAX_MESSAGE_SIZE 1536

/**
 * This struct represents a DHT message which will be passed to the
 * modules. The only part of the message which will be available to
 * all modules is Message.peer. Incoming modules will have Message.bytes
 * and Message.length when they come from the network module.
 */
struct DHTMessage;
struct DHTMessage
{
    struct Address* address;

    char padding[512];

    /** The message in binary format. */
    char bytes[DHTModules_MAX_MESSAGE_SIZE];

    /** The length of the binary message. */
    unsigned short length;

    /** The message as a bencoded dictionary. */
    Dict* asDict;

    /**
     * If this message is an outgoing reply, replyTo is the original query.
     * For incoming replies or any queries, it is NULL.
     */
    struct DHTMessage* replyTo;

    /** A memory allocator which will be freed after this message is sent. */
    const struct Allocator* allocator;
};

/**
 * This represents a DHT module.
 * Pass one of these to DHTModule_register() and it
 * will handle dht requests and responses.
 */
struct DHTModule;
struct DHTModule {
    /**
     * A user friendly null terminated string which will be used to
     * manipulate the module using the DHTModules API.
     */
    const char* const name;

    /**
     * The module's state.
     */
    void* const context;

    /**
     * Serialize the content of the context into a bencoded object.
     *
     * @param context the module's state.
     * @return a serialized form of context.
     */
    Dict* (* const serialize)(void* context);

    /**
     * Deserialize the context from a bencoded object.
     *
     * @param serialData the same data which was output by
     *                   moduleContext_serialize.
     *
     * @param context the existing context to add content to.
     */
    void (* const deserialize)(const Dict* serialData,
                               void* context);

    /**
     * When a node must be evicted from the table, this will allow the
     * module to decide which one it should be. Thus other node with more
     * capabilities will be favored.
     *
     * @param one node's ID.
     * @param another node's ID.
     * @param context the module's state.
     * @return less than 0 if the first node is favorable, greater than 0
     *         if the second node is more favorable,
     *         0 if they are considered equal.
     */
    int (* const compareNodes)(const char nodeId[20],
                               const char otherNodeId[20],
                               void* context);

    /**
     * @param the message which came in from a peer.
     * @param context the module's state.
     * @return 1 if a response should be sent for this message.
     *         -1 if the message is known invalid and should not be passed
     *            to any more handlers.
     */
    int (* const handleIncoming)(struct DHTMessage* message,
                                 void* context);

    /**
     * @param message the message which will be sent to the peer.
     * @param context the module's state.
     * @return -1 if the message should not be propigated to any more modules.
     *            use with caution as it may be interpreted as network loss.
     */
    int (* const handleOutgoing)(struct DHTMessage* message,
                                 void* context);
};

/** State of the registry. */
struct DHTModuleRegistry {

    /** Number of members. */
    int memberCount;

    /** A null terminated list of pointers to members. */
    struct DHTModule** members;

    /**
     * A list of serialized contexts by module name to be
     * deserialized when the modules are loaded.
     */
    Dict* serializedContexts;

    /** Means of getting memory for the registry. */
    struct Allocator* allocator;
};

/**
 * Create a new registry.
 *
 * @param allocator the means of getting memory to store the registry.
 * @return a new (empty) registry.
 */
struct DHTModuleRegistry* DHTModules_new(struct Allocator* allocator);

/**
 * Register an event handler.
 *
 * @param module the module to register.
 * @return 0 if everything went well.
 */
int DHTModules_register(struct DHTModule* module,
                        struct DHTModuleRegistry* registry);

/**
 * handleIncoming starts by running the last module registered
 * and working back. It is assumed that the core modules will
 * be registered first and will be the ones to initiate the
 * response.
 * The last module registered must be the one with access to
 * the network.
 *
 * @see DHTModule->handleIncoming()
 */
void DHTModules_handleIncoming(struct DHTMessage* message,
                               const struct DHTModuleRegistry* registry);

/**
 * @see DHTModule->handleOutgoing()
 */
void DHTModules_handleOutgoing(struct DHTMessage* message,
                               const struct DHTModuleRegistry* registry);

/**
 * Serialize the state of each member in the registry.
 *
 * @param registry the module registry to serialize.
 * @param writer a writer to which the output will be written.
 * @see DHTModule->serialize
 */
void DHTModules_serialize(const struct DHTModuleRegistry* registry,
                          const struct Writer* writer);

/**
 * Deserialize a new registry from binary.
 * NOTE: After deserialization, the modules still need to be registered.
 *
 * @param reader a reader which reads a stream of data prepared by a former call to DHTModules_serialize()
 * @param allocator a means of acquiring memory.
 * @return a new registry, modules will still need to be registered although they will regain their
 *         former state afterwords.
 * @see DHTModule->deserialize
 */
struct DHTModuleRegistry* DHTModules_deserialize(const struct Reader* reader,
                                                 struct Allocator* allocator);

#endif
