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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "admin/Admin.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "crypto/Key.h"
#include "crypto/ReplayProtector.h"
#include "dht/Address.h"
#include "net/SessionManager.h"
#include "net/SessionManager_admin.h"
#include "util/AddrTools.h"
#include "util/Identity.h"

struct Context
{
    struct Allocator* alloc;
    struct SessionManager* sm;
    struct Admin* admin;
    Identity
};

#define ENTRIES_PER_PAGE 64
static void getHandles(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*) vcontext);
    struct Allocator* alloc = Allocator_child(context->alloc);

    int64_t* page = Dict_getIntC(args, "page");
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;
    struct SessionManager_HandleList* hList = SessionManager_getHandleList(context->sm, alloc);

    List* list = List_new(alloc);
    for (int counter = 0; i < hList->length && counter++ < ENTRIES_PER_PAGE; i++) {
        List_addInt(list, hList->handles[i], alloc);
    }

    Dict* r = Dict_new(alloc);
    Dict_putListC(r, "handles", list, alloc);
    Dict_putIntC(r, "total", hList->length, alloc);

    if (i < hList->length) {
        Dict_putIntC(r, "more", 1, alloc);
    }

    Admin_sendMessage(r, txid, context->admin);

    Allocator_free(alloc);
}

static void outputSession(struct Context* context,
                          struct SessionManager_Session* session,
                          String* txid,
                          struct Allocator* alloc)
{
    Dict* r = Dict_new(alloc);
    if (!session) {
        Dict_putStringCC(r, "error", "no such session", alloc);
        Admin_sendMessage(r, txid, context->admin);
        return;
    }

    uint8_t printedAddr[40];
    AddrTools_printIp(printedAddr, session->caSession->herIp6);
    Dict_putStringC(r, "ip6", String_new(printedAddr, alloc), alloc);

    String* state =
        String_new(CryptoAuth_stateString(CryptoAuth_getState(session->caSession)), alloc);
    Dict_putStringC(r, "state", state, alloc);

    struct ReplayProtector* rp = &session->caSession->replayProtector;
    Dict_putIntC(r, "duplicates", rp->duplicates, alloc);
    Dict_putIntC(r, "lostPackets", rp->lostPackets, alloc);
    Dict_putIntC(r, "receivedOutOfRange", rp->receivedOutOfRange, alloc);

    struct Address addr;
    Bits_memcpy(addr.key, session->caSession->herPublicKey, 32);
    addr.path = session->sendSwitchLabel;
    addr.protocolVersion = session->version;

    Dict_putStringC(r, "addr", Address_toString(&addr, alloc), alloc);

    Dict_putIntC(r, "handle", session->receiveHandle, alloc);
    Dict_putIntC(r, "sendHandle", session->sendHandle, alloc);

    Dict_putIntC(r, "timeOfLastIn", session->timeOfLastIn, alloc);
    Dict_putIntC(r, "timeOfLastOut", session->timeOfLastOut, alloc);

    Dict_putIntC(r, "metric", session->metric, alloc);

    Admin_sendMessage(r, txid, context->admin);
    return;
}

static void sessionStats(Dict* args,
                         void* vcontext,
                         String* txid,
                         struct Allocator* alloc)
{
    struct Context* context = Identity_check((struct Context*) vcontext);
    int64_t* handleP = Dict_getIntC(args, "handle");
    uint32_t handle = *handleP;

    struct SessionManager_Session* session = SessionManager_sessionForHandle(handle, context->sm);
    outputSession(context, session, txid, alloc);
}

static struct SessionManager_Session* sessionForIP(Dict* args,
                                                   struct Context* context,
                                                   String* txid,
                                                   struct Allocator* alloc)
{
    String* ip6Str = Dict_getStringC(args, "ip6");
    uint8_t ip6Binary[16] = {0};

    Dict* r = Dict_new(alloc);
    if (AddrTools_parseIp(ip6Binary, ip6Str->bytes)) {
        Dict_putStringCC(r, "error", "malformed_ip", alloc);
        Admin_sendMessage(r, txid, context->admin);
        return NULL;
    }

    struct SessionManager_Session* session = SessionManager_sessionForIp6(ip6Binary, context->sm);

    if (!session) {
        Dict_putStringCC(r, "error", "no such session", alloc);
        Admin_sendMessage(r, txid, context->admin);
        return NULL;
    }

    return session;
}

static void sessionStatsByIP(Dict* args,
                             void* vcontext,
                             String* txid,
                             struct Allocator* alloc)
{
    struct Context* context = Identity_check((struct Context*) vcontext);
    struct SessionManager_Session* session = sessionForIP(args, context, txid, alloc);
    if (!session) { return; }
    outputSession(context, session, txid, alloc);
}

static void resetCA(Dict* args,
                    void* vcontext,
                    String* txid,
                    struct Allocator* alloc)
{
    struct Context* context = Identity_check((struct Context*) vcontext);
    struct SessionManager_Session* session = sessionForIP(args, context, txid, alloc);
    if (!session) { return; }
    CryptoAuth_reset(session->caSession);
    Dict* r = Dict_new(alloc);
    Dict_putStringCC(r, "error", "none", alloc);
    Admin_sendMessage(r, txid, context->admin);
}

void SessionManager_admin_register(struct SessionManager* sm,
                                   struct Admin* admin,
                                   struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .alloc = alloc,
        .sm = sm,
        .admin = admin
    }));
    Identity_set(ctx);

    Admin_registerFunction("SessionManager_getHandles", getHandles, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("SessionManager_sessionStats", sessionStats, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "handle", .required = 1, .type = "Int" }
        }), admin);

    Admin_registerFunction("SessionManager_sessionStatsByIP", sessionStatsByIP, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "ip6", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("SessionManager_resetCA", resetCA, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "ip6", .required = 1, .type = "String" }
        }), admin);
}
