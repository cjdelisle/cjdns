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
#include "subnode/PingResponder.h"
#include "util/Identity.h"

struct PingResponder_pvt
{
    struct PingResponder pub;
    struct MsgCore_Handler* handler;
    struct AddrSet* peers;
    struct Log* log;
    struct Allocator* alloc;
    struct MsgCore* msgCore;
    struct BoilerplateResponder* br;
    Identity
};

static void onPing(Dict* msg,
                   struct Address* src,
                   struct Allocator* tmpAlloc,
                   struct MsgCore_Handler* handler)
{
    struct PingResponder_pvt* prp = Identity_check((struct PingResponder_pvt*) handler->userData);
    Log_debug(prp->log, "Received ping req from [%s]", Address_toString(src, tmpAlloc)->bytes);

    String* txid = Dict_getStringC(msg, "txid");
    if (!txid) {
        Log_debug(prp->log, "ping missing txid");
        return;
    }

    Dict* responseDict = Dict_new(tmpAlloc);
    Dict_putStringC(responseDict, "txid", txid, tmpAlloc);
    BoilerplateResponder_addBoilerplate(prp->br, responseDict, src, tmpAlloc);
    MsgCore_sendResponse(prp->msgCore, responseDict, src, tmpAlloc);
}

struct PingResponder* PingResponder_new(struct Allocator* allocator,
                                        struct Log* log,
                                        struct MsgCore* msgCore,
                                        struct BoilerplateResponder* br)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct PingResponder_pvt* prp =
        Allocator_calloc(alloc, sizeof(struct PingResponder_pvt), 1);
    Identity_set(prp);
    prp->log = log;
    prp->alloc = alloc;
    prp->msgCore = msgCore;
    prp->handler = MsgCore_onQuery(msgCore, "pn", alloc);
    prp->handler->userData = prp;
    prp->handler->cb = onPing;
    prp->br = br;
    return &prp->pub;
}
