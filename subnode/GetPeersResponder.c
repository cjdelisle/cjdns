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
#include "subnode/GetPeersResponder.h"
#include "subnode/BoilerplateResponder.h"
#include "switch/NumberCompress.h"
#include "dht/dhtcore/VersionList.h"
#include "util/Identity.h"

#define GETPEERS_RESPONSE_NODES 8

struct GetPeersResponder_pvt
{
    struct GetPeersResponder pub;
    struct MsgCore_Handler* handler;
    struct AddrSet* peers;
    struct Log* log;
    struct Allocator* alloc;
    struct MsgCore* msgCore;
    struct BoilerplateResponder* br;
    Identity
};

static void onGetPeers(Dict* msg,
                       struct Address* src,
                       struct Allocator* tmpAlloc,
                       struct MsgCore_Handler* handler)
{
    struct GetPeersResponder_pvt* gprp =
        Identity_check((struct GetPeersResponder_pvt*) handler->userData);
    Log_debug(gprp->log, "Received getPeers req from [%s]", Address_toString(src, tmpAlloc)->bytes);

    String* nearLabelStr = Dict_getStringC(msg, "tar");
    if (!nearLabelStr || nearLabelStr->len != 8) {
        Log_debug(gprp->log, "getPeers does not contain proper target");
        return;
    }
    uint64_t label;
    Bits_memcpy(&label, nearLabelStr->bytes, 8);

    struct Address* outAddrs[GETPEERS_RESPONSE_NODES] = { NULL };

    if (label > 1) {
        int bitsUsed = NumberCompress_bitsUsedForLabel(label);
        label = (label & Bits_maxBits64(bitsUsed)) | 1 << bitsUsed;
    }
    for (int i = 0; i < gprp->peers->length; i++) {
        struct Address* peer = AddrSet_get(gprp->peers, i);
        uint64_t p = peer->path;
        if (p < label) { continue; }
        int j;
        for (j = 0; j < GETPEERS_RESPONSE_NODES; j++) {
            if (!outAddrs[j]) { continue; }
            if ((outAddrs[j]->path - label) > (p - label)) { continue; }
            break;
        }
        switch (j) {
            default: Bits_memmove(outAddrs, &outAddrs[1], (j - 1) * sizeof(char*));
            case 1: outAddrs[j - 1] = peer;
            case 0:;
        }
    }

    int size = GETPEERS_RESPONSE_NODES;
    for (int i = 0; i < GETPEERS_RESPONSE_NODES; i++) {
        if (!outAddrs[i]) { size--; }
    }

    Dict* responseDict = Dict_new(tmpAlloc);
    if (size > 0) {
        String* nodes = String_newBinary(NULL, size * Address_SERIALIZED_SIZE, tmpAlloc);
        struct VersionList* versions = VersionList_new(size, tmpAlloc);
        for (int i = 0, j = 0; i < GETPEERS_RESPONSE_NODES; i++) {
            if (!outAddrs[i]) { continue; }
            struct Address addr;
            Bits_memcpy(&addr, outAddrs[i], sizeof(struct Address));
            addr.path = NumberCompress_getLabelFor(addr.path, src->path);
            Address_serialize(&nodes->bytes[j * Address_SERIALIZED_SIZE], &addr);
            versions->versions[j] = outAddrs[i]->protocolVersion;
            j++;
        }
        Dict_putStringC(responseDict, "n", nodes, tmpAlloc);
        String* versionsStr = VersionList_stringify(versions, tmpAlloc);
        Dict_putStringC(responseDict, "np", versionsStr, tmpAlloc);
    }

    BoilerplateResponder_addBoilerplate(gprp->br, responseDict, src, tmpAlloc);

    MsgCore_sendResponse(gprp->msgCore, responseDict, src, gprp->alloc);
}


struct GetPeersResponder* GetPeersResponder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct AddrSet* peers,
                                                struct MsgCore* msgCore)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct GetPeersResponder_pvt* gprp =
        Allocator_calloc(alloc, sizeof(struct GetPeersResponder_pvt), 1);
    Identity_set(gprp);
    gprp->peers = peers;
    gprp->log = log;
    gprp->alloc = alloc;
    gprp->msgCore = msgCore;
    gprp->handler = MsgCore_onQuery(msgCore, "gp", alloc);
    gprp->handler->userData = gprp;
    gprp->handler->cb = onGetPeers;

    struct EncodingScheme* myScheme = NumberCompress_defineScheme(alloc);
    gprp->br = BoilerplateResponder_new(myScheme, alloc);
    return &gprp->pub;
}
