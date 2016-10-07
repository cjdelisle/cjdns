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
#include "dht/dhtcore/ReplySerializer.h"
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
    struct Address* selfAddr;
    struct BoilerplateResponder* br;
    struct EncodingScheme* myScheme;
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

    String* txid = Dict_getStringC(msg, "txid");
    if (!txid) {
        Log_debug(gprp->log, "getPeers missing txid");
        return;
    }

    String* nearLabelStr = Dict_getStringC(msg, "tar");
    uint64_t label;
    if (!nearLabelStr || nearLabelStr->len != 8) {
        Log_debug(gprp->log, "getPeers does not contain proper target");
        return;
    } else {
        uint64_t label_be;
        Bits_memcpy(&label_be, nearLabelStr->bytes, 8);
        label = Endian_bigEndianToHost64(label_be);
    }

    struct Address outAddrs[GETPEERS_RESPONSE_NODES] = { { .padding = 0 } };

    Bits_memcpy(&outAddrs[GETPEERS_RESPONSE_NODES-1], gprp->selfAddr, Address_SIZE);

    if (label > 1) {
        int formNum = EncodingScheme_getFormNum(gprp->myScheme, label);
        if (formNum > -1) {
            int bitsUsed = EncodingScheme_formSize(&gprp->myScheme->forms[formNum]);
            label = (label & Bits_maxBits64(bitsUsed)) | 1 << bitsUsed;
        }
    }
    int size = 0;
    for (int i = 0; i < gprp->peers->length; i++) {
        struct Address* peer = AddrSet_get(gprp->peers, i);
        if (peer->path < label) { continue; }
        Bits_memcpy(&outAddrs[size++], peer, Address_SIZE);
    }

    Dict* responseDict = Dict_new(tmpAlloc);
    ReplySerializer_serialize((&(struct Address_List) { .elems = outAddrs, .length = size }),
                              responseDict,
                              src,
                              tmpAlloc);

    Dict_putStringC(responseDict, "txid", txid, tmpAlloc);

    BoilerplateResponder_addBoilerplate(gprp->br, responseDict, src, tmpAlloc);

    MsgCore_sendResponse(gprp->msgCore, responseDict, src, tmpAlloc);
}


struct GetPeersResponder* GetPeersResponder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct AddrSet* peers,
                                                struct Address* selfAddr,
                                                struct MsgCore* msgCore,
                                                struct BoilerplateResponder* br,
                                                struct EncodingScheme* myScheme)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct GetPeersResponder_pvt* gprp =
        Allocator_calloc(alloc, sizeof(struct GetPeersResponder_pvt), 1);
    Identity_set(gprp);
    gprp->peers = peers;
    gprp->log = log;
    gprp->alloc = alloc;
    gprp->msgCore = msgCore;
    gprp->selfAddr = selfAddr;
    gprp->handler = MsgCore_onQuery(msgCore, "gp", alloc);
    gprp->handler->userData = gprp;
    gprp->handler->cb = onGetPeers;
    gprp->br = br;
    gprp->myScheme = myScheme;
    return &gprp->pub;
}
