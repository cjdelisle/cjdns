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

    struct EncodingScheme* legacyV3x5x8;

    struct Log* logger;

    Identity
};

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    struct EncodingSchemeModule_pvt* ctx =
        Identity_check((struct EncodingSchemeModule_pvt*) vcontext);

    struct EncodingScheme* scheme = NULL;

    String* schemeDefinition = Dict_getString(message->asDict, CJDHTConstants_ENC_SCHEME);
    if (schemeDefinition) {
        scheme = EncodingScheme_deserialize(schemeDefinition, message->allocator);
    } else {
        scheme = ctx->legacyV3x5x8;
    }

    if (!scheme) {
        Log_debug(ctx->logger, "Failed to parse encoding scheme");
        Assert_ifTesting(0);
        return -1;
    }
    message->encodingScheme = scheme;

    int64_t* version = Dict_getInt(message->asDict, CJDHTConstants_PROTOCOL);
    if (!version || !Version_isCompatible(Version_CURRENT_PROTOCOL, *version)) {
        Log_debug(ctx->logger, "Incompatible version");
        Assert_ifTesting(0);
        return -1;
    }
    message->address->protocolVersion = *version;

    int64_t* encIdx = Dict_getInt(message->asDict, CJDHTConstants_ENC_INDEX);
    if (!encIdx) {
        Log_debug(ctx->logger, "Missing encoding index, version [%d]", (int) (*version));
        return -1;
    }
    if (*encIdx < 0 || *encIdx >= scheme->count) {
        Log_debug(ctx->logger, "Invalid encoding index [%lld], version [%d]",
                  (long long int) (*encIdx),
                  (int) (*version));
        Assert_ifTesting(0);
        return -1;
    }
    message->encIndex = *encIdx;

    return 0;
}

static int handleOutgoing(struct DHTMessage* dmesg, void* vcontext)
{
    struct EncodingSchemeModule_pvt* ctx =
        Identity_check((struct EncodingSchemeModule_pvt*) vcontext);

    // Send our encoding scheme definition
    Dict_putString(dmesg->asDict,
                   CJDHTConstants_ENC_SCHEME,
                   ctx->schemeDefinition,
                   dmesg->allocator);

    // sanity check if in sym, real world broken nodes will trigger this.
    Assert_ifTesting(dmesg->address->path ==
        EncodingScheme_convertLabel(ctx->scheme,
                                    dmesg->address->path,
                                    EncodingScheme_convertLabel_convertTo_CANNONICAL));

    // And tell the asker which interface the message came from
    int encIdx = EncodingScheme_getFormNum(ctx->scheme, dmesg->address->path);
    Assert_ifParanoid(encIdx != EncodingScheme_getFormNum_INVALID);
    Dict_putInt(dmesg->asDict, CJDHTConstants_ENC_INDEX, encIdx, dmesg->allocator);

    return 0;
}

void EncodingSchemeModule_register(struct DHTModuleRegistry* reg,
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
        }));
    Identity_set(ctx);
    ctx->module.context = ctx;
    ctx->legacyV3x5x8 = NumberCompress_v3x5x8_defineScheme(alloc);

    DHTModuleRegistry_register(&ctx->module, reg);
}
