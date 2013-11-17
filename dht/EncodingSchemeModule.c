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

struct EncodingSchemeModule_pvt;

struct SchemeAndIndex
{
    /** The peer's encoding scheme. */
    String* schemeDefinition;

    /** The peer's encoding scheme form index. */
    int encIdx;

    /** Our interface index. */
    uint32_t ifIndex;

    /** The module. */
    struct EncodingSchemeModule_pvt* ctx;

    /** Allocator for this SchemeAndIndex. */
    struct Allocator* alloc;

    Identity
};

#define Map_VALUE_TYPE struct SchemeAndIndex*
#define Map_KEY_TYPE uint32_t
#define Map_NAME OfPeerSchemes
#include "util/Map.h"

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

    struct Map_OfPeerSchemes peerSchemeMap;

    struct Allocator* alloc;

    Identity
};

static int peerSchemeOnFree(struct Allocator_OnFreeJob* job)
{
    struct SchemeAndIndex* sai = Identity_cast((struct SchemeAndIndex*) job->userData);
    int mapIndex = Map_OfPeerSchemes_indexForKey(&sai->ifIndex, &sai->ctx->peerSchemeMap);
    Assert_true(mapIndex > -1);
    Map_OfPeerSchemes_remove(mapIndex, &sai->ctx->peerSchemeMap);
    return 0;
}

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

    int64_t* version = Dict_getInt(message->asDict, CJDHTConstants_PROTOCOL);
    if (!version || !Version_isCompatible(Version_CURRENT_PROTOCOL, *version)) {
        Log_debug(ctx->logger, "Incompatible version");
        return -1;
    }

    int64_t* encIdx = Dict_getInt(message->asDict, CJDHTConstants_ENC_INDEX);
    if (!encIdx || *encIdx <= 0 || *encIdx >= scheme->count) {
        Log_debug(ctx->logger, "Invalid encoding index");
        return -1;
    }

    NodeStore_discoverNode(ctx->ns, message->address, 2, *version, scheme, *encIdx);

    int bits = NumberCompress_bitsUsedForLabel(message->address->path);
    if (Bits_log2x64(message->address->path) == bits) {
        // This node is a peer.
        uint32_t ifIndex = NumberCompress_getDecompressed(message->address->path, bits);
        int mapIndex = Map_OfPeerSchemes_indexForKey(&ifIndex, &ctx->peerSchemeMap);
        if (mapIndex > -1
          && String_equals(ctx->peerSchemeMap.values[mapIndex]->schemeDefinition, schemeDefinition)
          && ctx->peerSchemeMap.values[mapIndex]->encIdx == *encIdx)
        {
            return 0;
        }
        if (mapIndex > -1) {
            Allocator_free(ctx->peerSchemeMap.values[mapIndex]->alloc);
        }
        struct Allocator* peerSchemeAlloc = Allocator_child(ctx->alloc);
        struct SchemeAndIndex* sai =
            Allocator_calloc(peerSchemeAlloc, sizeof(struct SchemeAndIndex), 1);
        sai->schemeDefinition = String_clone(schemeDefinition, peerSchemeAlloc);
        sai->encIdx = *encIdx;
        sai->ifIndex = ifIndex;
        sai->alloc = peerSchemeAlloc;
        sai->ctx = ctx;
        Identity_set(sai);
        Allocator_onFree(peerSchemeAlloc, peerSchemeOnFree, sai);
    }

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
    int encIdx = EncodingScheme_getFormNum(ctx->scheme, dmesg->address->path);
    Dict_putInt(dmesg->asDict, CJDHTConstants_ENC_INDEX, encIdx, dmesg->allocator);

    // Then tell them the scheme and form index for the nearest node along the path.
    int totalBits = NumberCompress_bitsUsedForLabel(dmesg->address->path);
    uint32_t ifIndex = NumberCompress_dyn_getDecompressed(dmesg->address->path, totalBits);
    int mapIndex = Map_OfPeerSchemes_indexForKey(&ifIndex, &ctx->peerSchemeMap);
    if (mapIndex > -1) {
        Dict_putString(dmesg->asDict,
                       CJDHTConstants_PEER_ENC_SCHEME,
                       ctx->peerSchemeMap.values[mapIndex]->schemeDefinition,
                       dmesg->allocator);
        Dict_putInt(dmesg->asDict,
                    CJDHTConstants_PEER_ENC_INDEX,
                    ctx->peerSchemeMap.values[mapIndex]->encIdx,
                    dmesg->allocator);
    }

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
            .ns = ns,
            .alloc = alloc,
            .peerSchemeMap = {
                .allocator = alloc
            }
        }));
    Identity_set(ctx);
    ctx->module.context = ctx;

    DHTModuleRegistry_register(&ctx->module, reg);
}
