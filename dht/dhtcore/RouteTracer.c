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
#include "dht/Address.h"
#include "dht/dhtcore/RouteTracer.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/VersionList.h"
#include "dht/CJDHTConstants.h"
#include "switch/LabelSplicer.h"
#include "util/events/EventBase.h"
#include "util/events/Timeout.h"
#include "util/version/Version.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/Endian.h"

struct RouteTracer_pvt
{
    struct RouteTracer pub;
    struct EventBase* eventBase;
    struct NodeStore* nodeStore;
    struct RouterModule* router;
    struct Log* logger;
    uint8_t myAddress[16];
    Identity
};

struct RouteTracer_Trace
{
    struct RouterModule_Promise pub;
    struct RouteTracer_pvt* tracer;
    uint64_t target;
    uint64_t lastNodeAsked;
    Identity
};

static void noPeers(void* vtrace)
{
    struct RouteTracer_Trace* trace = Identity_cast((struct RouteTracer_Trace*)vtrace);
    // trace has stalled.
    if (trace->pub.callback) {
        trace->pub.callback(&trace->pub, 0, NULL, NULL);
    }
    Allocator_free(trace->pub.alloc);
}

#define log(logger, trace, message) \
    do {                                                                 \
        uint8_t target[20];                                              \
        uint8_t lastNodeAsked[20];                                       \
        AddrTools_printPath(target, (trace)->target);                    \
        AddrTools_printPath(lastNodeAsked, (trace)->lastNodeAsked);      \
        Log_debug((logger), "tracing [%s] last request to [%s] %s",      \
                  target, lastNodeAsked, (message));                     \
    } while (0)
// CHECKFILES_IGNORE expecting a ;

static void traceStep(struct RouteTracer_Trace* trace, struct Node* next);

static void responseCallback(struct RouterModule_Promise* promise,
                             uint32_t lagMilliseconds,
                             struct Node* fromNode,
                             Dict* result)
{
    struct RouteTracer_Trace* trace = Identity_cast((struct RouteTracer_Trace*)promise->userData);
    struct RouteTracer_pvt* ctx = Identity_cast((struct RouteTracer_pvt*)trace->tracer);
    if (!fromNode) {
        // trace has stalled.
        log(ctx->logger, trace, "STALLED request timed out");
        noPeers(trace);
        return;
    }

    if (trace->pub.callback) {
        trace->pub.callback(&trace->pub, lagMilliseconds, fromNode, result);
    }

    String* nodes = Dict_getString(result, CJDHTConstants_NODES);

    if (nodes && (nodes->len == 0 || nodes->len % Address_SERIALIZED_SIZE != 0)) {
        log(ctx->logger, trace, "STALLED dropping unrecognized reply");
        noPeers(trace);
        return;
    }

    struct VersionList* versions = NULL;
    String* versionsStr = Dict_getString(result, CJDHTConstants_NODE_PROTOCOLS);
    if (versionsStr) {
        versions = VersionList_parse(versionsStr, promise->alloc);
        #ifdef Version_1_COMPAT
            // Version 1 lies about the versions of other nodes, assume they're all v1.
            if (fromNode->version < 2) {
                for (int i = 0; i < (int)versions->length; i++) {
                    versions->versions[i] = 1;
                }
            }
        #endif
    }

    struct Node* next = NULL;

    for (uint32_t i = 0; nodes && i < nodes->len; i += Address_SERIALIZED_SIZE) {

        struct Address addr;
        Address_parse(&addr, (uint8_t*) &nodes->bytes[i]);

        // calculate the ipv6
        Address_getPrefix(&addr);

        if (!Bits_memcmp(ctx->myAddress, addr.ip6.bytes, 16)) {
            // Any path which loops back through us is necessarily a dead route.
            NodeStore_brokenPath(addr.path, ctx->nodeStore);
            continue;
        }

        // We need to splice the given address on to the end of the
        // address of the node which gave it to us.
        addr.path = LabelSplicer_splice(addr.path, fromNode->address.path);

        #ifdef Log_DEBUG
            uint8_t printedAddr[60];
            Address_print(printedAddr, &addr);
            Log_debug(ctx->logger, "discovered node [%s]", printedAddr);
        #endif

        if (addr.path == UINT64_MAX) {
            log(ctx->logger, trace, "dropping node because route could not be spliced");
            continue;
        }

        if (!AddressCalc_validAddress(addr.ip6.bytes)) {
            log(ctx->logger, trace, "was told garbage");
            // This should never happen, badnode.
            break;
        }

        // Nodes we are told about are inserted with 0 reach and assumed version 1.
        uint32_t version = (versions) ? versions->versions[i / Address_SERIALIZED_SIZE] : 1;
        struct Node* n = NodeStore_addNode(ctx->nodeStore, &addr, 0, version);

        if (!n) {
            // incompatible version, introduced to ourselves...
        } else if (!LabelSplicer_routesThrough(trace->target, n->address.path)) {
            // not on the way
        } else  if (n->address.path <= fromNode->address.path) {
            // losing ground
        } else if (next && n->address.path >= next->address.path) {
            // not better than the one we have
        } else {
            next = n;
        }
    }

    if (fromNode->address.path == trace->target) {
        log(ctx->logger, trace, "Trace completed successfully");
        noPeers(trace);
        return;
    }

    if (!nodes) {
        log(ctx->logger, trace, "No nodes in trace response");
    }

    if (!next) {
        log(ctx->logger, trace, "STALLED no suitable peers in reply");
        noPeers(trace);
        return;
    }

    if (!LabelSplicer_routesThrough(trace->target, next->address.path)) {
        log(ctx->logger, trace, "STALLED Nodestore broke the path of the best option");
    }

    traceStep(trace, next);
}

static void traceStep(struct RouteTracer_Trace* trace, struct Node* next)
{
    struct RouteTracer_pvt* ctx = Identity_cast((struct RouteTracer_pvt*)trace->tracer);

    if (!next) {
        // can't find a next node, stalled.
        Timeout_setTimeout(noPeers, trace, 0, trace->tracer->eventBase, trace->pub.alloc);
        return;
    }

    Assert_true(LabelSplicer_routesThrough(trace->target, next->address.path));

    trace->lastNodeAsked = next->address.path;

    struct RouterModule_Promise* rp =
        RouterModule_newMessage(next, 0, ctx->router, trace->pub.alloc);

    Dict* message = Dict_new(rp->alloc);

    #ifdef Version_4_COMPAT
        if (next->version < 5) {
            // The node doesn't support the new API so try running a search for
            // the bitwise complement of their address to get some peers.
            Dict_putString(message, CJDHTConstants_QUERY, CJDHTConstants_QUERY_FN, rp->alloc);
            String* notAddr = String_newBinary((char*)next->address.ip6.bytes, 16, rp->alloc);
            for (int i = 0; i < 16; i++) {
                notAddr->bytes[i] ^= 0xff;
            }
            Dict_putString(message, CJDHTConstants_TARGET, notAddr, rp->alloc);
            log(ctx->logger, trace, "Sending legacy search method because getpeers is unavailable");
        } else {
    #endif

    Dict_putString(message, CJDHTConstants_QUERY, CJDHTConstants_QUERY_GP, rp->alloc);
    uint64_t labelForThem = LabelSplicer_unsplice(trace->target, next->address.path);
    labelForThem = Endian_hostToBigEndian64(labelForThem);
    String* target = String_newBinary((char*)&labelForThem, 8, rp->alloc);
    Dict_putString(message, CJDHTConstants_TARGET, target, rp->alloc);
    log(ctx->logger, trace, "Sending getpeers request");

    #ifdef Version_4_COMPAT
        }
    #endif

    rp->userData = trace;
    rp->callback = responseCallback;

    RouterModule_sendMessage(rp, message);
}

struct RouterModule_Promise* RouteTracer_trace(uint64_t route,
                                               struct RouteTracer* routeTracer,
                                               struct Allocator* allocator)
{
    struct RouteTracer_pvt* tracer = Identity_cast((struct RouteTracer_pvt*)routeTracer);
    struct Allocator* alloc = Allocator_child(allocator);
    struct RouteTracer_Trace* trace = Allocator_clone(alloc, (&(struct RouteTracer_Trace) {
        .target = route,
        .tracer = tracer,
        .pub = {
            .alloc = alloc
        }
    }));
    Identity_set(trace);

    struct NodeList* firstHop = NodeStore_getPeers(route, 1, alloc, tracer->nodeStore);

    traceStep(trace, (firstHop->size > 0) ? firstHop->nodes[0] : NULL);

    return &trace->pub;
}

struct RouteTracer* RouteTracer_new(struct NodeStore* store,
                                    struct RouterModule* router,
                                    const uint8_t myAddress[16],
                                    struct EventBase* base,
                                    struct Log* logger,
                                    struct Allocator* alloc)
{
    struct RouteTracer_pvt* out = Allocator_clone(alloc, (&(struct RouteTracer_pvt) {
        .nodeStore = store,
        .router = router,
        .logger = logger,
        .eventBase = base
    }));
    Identity_set(out);
    Bits_memcpyConst(out->myAddress, myAddress, 16);
    return &out->pub;
}
