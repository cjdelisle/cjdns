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
#include "admin/Admin.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "crypto/Key.h"
#include "crypto/ReplayProtector.h"
#include "interface/SessionManager.h"
#include "interface/SessionManager_admin.h"
#include "util/AddrTools.h"

struct Context
{
    struct Allocator* alloc;
    struct SessionManager* sm;
    struct Admin* admin;
};

// typical peer record is around 140 benc chars, so can't have very many in 1023
#define ENTRIES_PER_PAGE 64
static void getHandles(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    struct Allocator* alloc = Allocator_child(context->alloc);

    int64_t* page = Dict_getInt(args, String_CONST("page"));
    uint32_t i = (page) ? *page * ENTRIES_PER_PAGE : 0;
    struct SessionManager_HandleList* hList = SessionManager_getHandleList(context->sm, alloc);

    List* list = NULL;
    for (int counter=0; i < hList->count && counter++ < ENTRIES_PER_PAGE; i++) {
        list = List_addInt(list, hList->handles[i], alloc);
    }

    Dict* r = Dict_new(alloc);
    Dict_putList(r, String_CONST("handles"), list, alloc);
    Dict_putInt(r, String_CONST("total"), hList->count, alloc);

    String* more = String_CONST("more");
    if (i < hList->count) {
      Dict_putInt(r, more, 1, alloc);
    }

    Admin_sendMessage(r, txid, context->admin);

    Allocator_free(alloc);
}

static void sessionStats(Dict* args,
                         void* vcontext,
                         String* txid,
                         struct Allocator* alloc)
{
    struct Context* context = vcontext;
    int64_t* handleP = Dict_getInt(args, String_CONST("handle"));
    uint32_t handle = *handleP;

    struct SessionManager_Session* session = SessionManager_sessionForHandle(handle, context->sm);
    uint8_t* ip6 = SessionManager_getIp6(handle, context->sm);

    Dict* r = Dict_new(alloc);
    if (!session) {
        Dict_putString(r, String_CONST("error"), String_CONST("no such session"), alloc);
        Admin_sendMessage(r, txid, context->admin);
        return;
    }
    // both or neither
    Assert_true(ip6);

    uint8_t printedAddr[40];
    AddrTools_printIp(printedAddr, ip6);
    Dict_putString(r, String_CONST("ip6"), String_new(printedAddr, alloc), alloc);

    Dict_putString(r,
                   String_CONST("state"),
                   String_new(CryptoAuth_stateString(session->cryptoAuthState), alloc),
                   alloc);

    struct ReplayProtector* rp = CryptoAuth_getReplayProtector(session->internal);
    Dict_putInt(r, String_CONST("duplicates"), rp->duplicates, alloc);
    Dict_putInt(r, String_CONST("lostPackets"), rp->lostPackets, alloc);
    Dict_putInt(r, String_CONST("receivedOutOfRange"), rp->receivedOutOfRange, alloc);

    uint8_t* key = CryptoAuth_getHerPublicKey(session->internal);
    Dict_putString(r, String_CONST("publicKey"), Key_stringify(key, alloc), alloc);
    Dict_putInt(r, String_CONST("version"), session->version, alloc);
    Dict_putInt(r, String_CONST("handle"),
                Endian_bigEndianToHost32(session->receiveHandle_be), alloc);
    Dict_putInt(r, String_CONST("sendHandle"),
                Endian_bigEndianToHost32(session->sendHandle_be), alloc);

    Dict_putInt(r, String_CONST("timeOfLastIn"), session->timeOfLastIn, alloc);
    Dict_putInt(r, String_CONST("timeOfLastOut"), session->timeOfLastOut, alloc);

    Admin_sendMessage(r, txid, context->admin);
    return;
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

    Admin_registerFunction("SessionManager_getHandles", getHandles, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 0, .type = "Int" }
        }), admin);

    Admin_registerFunction("SessionManager_sessionStats", sessionStats, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "handle", .required = 1, .type = "Int" }
        }), admin);
}
