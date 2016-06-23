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
#include "subnode/FindNodeResponder.h"
#include "util/Identity.h"
#include "subnode/BoilerplateResponder.h"
#include "dht/dhtcore/ReplySerializer.h"
#include "util/events/Time.h"

#define FINDNODE_RESPONSE_NODES 8

struct FindNodeResponder_pvt
{
    struct FindNodeResponder pub;

    struct EventBase* base;
    struct MsgCore_Handler* handler;
    struct Log* log;
    struct Allocator* alloc;
    struct MsgCore* msgCore;
    struct BoilerplateResponder* br;
    struct NodeCache* nc;

    Identity
};

static void onFindNode(Dict* msg,
                       struct Address* src,
                       struct Allocator* tmpAlloc,
                       struct MsgCore_Handler* handler)
{
    struct FindNodeResponder_pvt* fnrp =
        Identity_check((struct FindNodeResponder_pvt*) handler->userData);
    Log_debug(fnrp->log, "Received findNode req from [%s]", Address_toString(src, tmpAlloc)->bytes);

    String* txid = Dict_getStringC(msg, "txid");
    if (!txid) {
        Log_debug(fnrp->log, "findNode missing txid");
        return;
    }

    String* targetStr = Dict_getStringC(msg, "tar");
    struct Address target;
    if (!targetStr || targetStr->len != 16) {
        Log_debug(fnrp->log, "findNode does not contain proper target");
        return;
    } else {
        Bits_memcpy(target.ip6.bytes, targetStr->bytes, 8);
    }

    struct Address_List* al =
        NodeCache_getNearest(fnrp->nc, &target, tmpAlloc, FINDNODE_RESPONSE_NODES);

    Dict* responseDict = Dict_new(tmpAlloc);
    ReplySerializer_serialize(al, responseDict, src, tmpAlloc);
    Dict_putStringC(responseDict, "txid", txid, tmpAlloc);
    BoilerplateResponder_addBoilerplate(fnrp->br, responseDict, src, tmpAlloc);
    MsgCore_sendResponse(fnrp->msgCore, responseDict, src, tmpAlloc);
}

struct FindNodeResponder* FindNodeResponder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct MsgCore* msgCore,
                                                struct EventBase* base,
                                                struct BoilerplateResponder* br,
                                                struct NodeCache* nc)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct FindNodeResponder_pvt* fnrp =
        Allocator_calloc(alloc, sizeof(struct FindNodeResponder_pvt), 1);
    Identity_set(fnrp);
    fnrp->log = log;
    fnrp->alloc = alloc;
    fnrp->msgCore = msgCore;
    fnrp->base = base;
    fnrp->handler = MsgCore_onQuery(msgCore, "fn", alloc);
    fnrp->handler->userData = fnrp;
    fnrp->handler->cb = onFindNode;
    fnrp->br = br;
    fnrp->nc = nc;
    return &fnrp->pub;
}
