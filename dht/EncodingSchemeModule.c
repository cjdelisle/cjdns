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
#include "dht/EncodingSchemeModule.h"
#include "dht/Address.h"
#include "dht/CJDHTConstants.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "benc/String.h"
#include "util/Identity.h"
#include "switch/NumberCompress.h"
#include "switch/EncodingScheme.h"
#include "util/version/Version.h"
#include "dht/dhtcore/NodeStore.h"

/**
 * The encoding scheme module tags each message with our encoding scheme
 * representation as well as the number of the smallest encoding form which
 * can represent the return path to the querying node.
 *
 * On incoming messages, it informs NodeStore of the encoding scheme used by
 * the node sending the message and if the node is pre-version-6, it converts
 * the query responses from the pre-version-6 representation to the version-6
 * representation.
 */

struct EncodingSchemeModule_pvt
{
    struct DHTModule module;

    /** Our scheme. */
    String* schemeDefinition;
    struct EncodingScheme* scheme;

    struct NodeStore* ns;

    struct Log* logger;

    Identity
};

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    struct EncodingSchemeModule_pvt* ctx =
        Identity_cast((struct EncodingSchemeModule_pvt*) vcontext);

    String* schemeDefinition = Dict_getString(message->asDict, CJDHTConstants_ENC_SCHEME);
    if (!schemeDefinition) {
        return 0;
    }
    struct EncodingScheme* scheme =
        EncodingScheme_deserialize(schemeDefinition, message->allocator);
    if (!scheme) {
        Log_debug(ctx->logger, "Failed to parse encoding scheme");
        return -1;
    }

    int form = -1;

    int64_t* versionP = Dict_getInt(message->asDict, CJDHTConstants_PROTOCOL);
    if (!versionP || !Version_isCompatible(Version_CURRENT_PROTOCOL, *versionP)) {
        Log_debug(ctx->logger, "Incompatible version");
        return -1;
    }
    uint32_t version = *versionP;

    int64_t* ifIndex = Dict_getInt(message->asDict, CJDHTConstants_IF_INDEX);
    if (ifIndex) {
        int log2Ifi = Bits_log2x32((uint32_t) (*ifIndex));
        for (int i = 0; i < scheme->count; i++) {
            if (log2Ifi < scheme->forms[i].bitCount) {
                form = i;
                break;
            }
        }
    }
    #ifdef Version_5_COMPAT
        else if (version < 6) {
            int64_t* encIdx = Dict_getInt(message->asDict, CJDHTConstants_ENC_INDEX);
            if ((encIdx && *encIdx > 0) || *encIdx < scheme->count) {
                form = *encIdx;
            }
        }
    #endif

    if (form < 0) {
        Log_debug(ctx->logger, "Invalid interface index");
        return -1;
    }

    NodeStore_discoverNode(ctx->ns, message->address, 2, version, scheme, form);

    return 0;
}

static int handleOutgoing(struct DHTMessage* dmesg, void* vcontext)
{
    struct EncodingSchemeModule_pvt* ctx =
        Identity_cast((struct EncodingSchemeModule_pvt*) vcontext);

    // Send our encoding scheme definition
    Dict_putString(dmesg->asDict,
                   CJDHTConstants_ENC_SCHEME,
                   ctx->schemeDefinition,
                   dmesg->allocator);

    // And tell the asker which interface the message came from
    int totalBits = NumberCompress_bitsUsedForLabel(dmesg->address->path);
    uint32_t ifIndex = NumberCompress_dyn_getDecompressed(dmesg->address->path, totalBits);
    Dict_putInt(dmesg->asDict, CJDHTConstants_IF_INDEX, ifIndex, dmesg->allocator);

    return 0;
}

void EncodingSchemeModule_register(struct DHTModuleRegistry* reg,
                                   struct NodeStore* ns,
                                   struct Log* logger,
                                   struct Allocator* alloc)
{
    struct EncodingScheme* scheme = NumberCompress_defineScheme(alloc);
    String* schemeDefinition = EncodingScheme_serialize(scheme, alloc);

    struct EncodingSchemeModule_pvt* ctx =
        Allocator_clone(alloc, (&(struct EncodingSchemeModule_pvt) {
            .module = {
                .name = "EncodingSchemeModule",
                .handleIncoming = handleIncoming,
                .handleOutgoing = handleOutgoing
            },
            .logger = logger,
            .scheme = scheme,
            .schemeDefinition = schemeDefinition,
            .ns = ns
        }));
    Identity_set(ctx);
    ctx->module.context = ctx;

    DHTModuleRegistry_register(&ctx->module, reg);
}
