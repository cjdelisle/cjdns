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
#include "net/Benchmark.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "crypto/random/Random.h"
#include "interface/Iface.h"
#include "util/log/FileWriterLog.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "net/NetCore.h"
#include "util/Checksum.h"

struct Context
{
    struct Allocator* alloc;
    struct EventBase* base;
    struct Log* log;
    struct Random* rand;

    uint64_t startTime;
    uint64_t items;
    char* benchName;
    char* itemName;

    Identity
};

static void begin(struct Context* ctx, char* benchName, uint64_t items, char* itemName)
{
    Assert_true(!ctx->benchName);
    ctx->benchName = benchName;
    ctx->itemName = itemName;
    ctx->items = items;
    Log_info(ctx->log, "----- Begin %s benchmark -----", benchName);
    ctx->startTime = Time_hrtime();
}

static void done(struct Context* ctx)
{
    uint64_t endTimes = Time_hrtime();
    uint64_t time = (endTimes - ctx->startTime) / 1000000;

    // same as items / (time / 1024) (converting time to seconds)
    uint64_t kbps = (ctx->items * 1024) / time;

    Log_info(ctx->log, " ");
    Log_info(ctx->log, "---------------------------------------------------------------");
    Log_info(ctx->log, "Benchmark %s in %dms. %d %s per second",
        ctx->benchName, (int)time, (int)kbps, ctx->itemName);
    Log_info(ctx->log, "---------------------------------------------------------------");
    Log_info(ctx->log, " ");
    Assert_true(ctx->benchName);
    ctx->benchName = NULL;
}

static void cryptoAuth(struct Context* ctx)
{
    Log_info(ctx->log, "Setting up salsa20/poly1305 benchmark (encryption and decryption only)");
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct CryptoAuth* ca1 = CryptoAuth_new(alloc, NULL, ctx->base, ctx->log, ctx->rand);
    struct CryptoAuth* ca2 = CryptoAuth_new(alloc, NULL, ctx->base, ctx->log, ctx->rand);

    struct CryptoAuth_Session* sess1 =
        CryptoAuth_newSession(ca1, alloc, ca2->publicKey, false, "bench");
    struct CryptoAuth_Session* sess2 =
        CryptoAuth_newSession(ca2, alloc, ca1->publicKey, false, "bench");

    int size = 1500;
    int count = 100000;
    struct Message* msg = Message_new(size, 256, alloc);
    Random_bytes(ctx->rand, msg->bytes, msg->length);

    // setup session
    for (int i = 0; i < 2; i++) {
        Assert_true(!CryptoAuth_encrypt(sess1, msg));
        Assert_true(!CryptoAuth_decrypt(sess2, msg));
        Assert_true(!CryptoAuth_encrypt(sess2, msg));
        Assert_true(!CryptoAuth_decrypt(sess1, msg));
    }

    begin(ctx, "salsa20/poly1305", (count * size * 8) / 1024, "kilobits");
    for (int i = 0; i < count; i++) {
        Assert_true(!CryptoAuth_encrypt(sess1, msg));
        Assert_true(!CryptoAuth_decrypt(sess2, msg));
    }
    done(ctx);
    Allocator_free(alloc);
}



struct SwitchingContext
{
    struct Iface aliceIf;
    struct Iface bobIf;

    struct Context* benchmarkCtx;
    struct Iface aliceCtrlIf;
    int msgCount;
    Identity
};

static Iface_DEFUN aliceToBob(struct Message* msg, struct Iface* aliceIf)
{
    struct SwitchingContext* sc =
        Identity_containerOf(aliceIf, struct SwitchingContext, aliceIf);
    return Iface_next(&sc->bobIf, msg);
}

static Iface_DEFUN bobToAlice(struct Message* msg, struct Iface* bobIf)
{
    struct SwitchingContext* sc =
        Identity_containerOf(bobIf, struct SwitchingContext, bobIf);
    return Iface_next(&sc->aliceIf, msg);
}

static Iface_DEFUN aliceCtrlRecv(struct Message* msg, struct Iface* aliceCtrlIf)
{
    struct SwitchingContext* sc =
        Identity_containerOf(aliceCtrlIf, struct SwitchingContext, aliceCtrlIf);
    //Log_debug(sc->benchmarkCtx->log, "pong!");
    sc->msgCount++;
    return NULL;
}

#define SECRETA \
    "\xd8\x54\x3e\x70\xb9\xae\x7c\x41\xbc\x18\xa4\x9a\x9c\xee\xca\x9c" \
    "\xdc\x45\x01\x96\x6b\xbd\x7e\x76\xcf\x3a\x9f\xbc\x12\xed\x8b\xb4"

#define SECRETB \
    "\xad\x7e\xa3\x26\xaa\x01\x94\x0a\x25\xbc\x9e\x01\x26\x22\xdb\x69" \
    "\x4f\xd9\xb4\x17\x7c\xf3\xf8\x91\x16\xf3\xcf\xe8\x5c\x80\xe1\x4a"

static void switching(struct Context* ctx)
{
    Log_info(ctx->log, "Setting up salsa20/poly1305 benchmark (encryption and decryption only)");
    struct Allocator* alloc = Allocator_child(ctx->alloc);;

    struct SwitchingContext* sc = Allocator_calloc(alloc, sizeof(struct SwitchingContext), 1);
    Identity_set(sc);
    sc->benchmarkCtx = ctx;
    sc->aliceIf.send = aliceToBob;
    sc->bobIf.send = bobToAlice;
    sc->aliceCtrlIf.send = aliceCtrlRecv;

    struct NetCore* alice = NetCore_new(SECRETA, alloc, ctx->base, ctx->rand, ctx->log);
    struct InterfaceController_Iface* aliceIci =
        InterfaceController_newIface(alice->ifController, String_CONST("alice"), alloc);
    Iface_plumb(&sc->aliceIf, &aliceIci->addrIf);

    struct NetCore* bob = NetCore_new(SECRETB, alloc, ctx->base, ctx->rand, ctx->log);
    struct InterfaceController_Iface* bobIci =
        InterfaceController_newIface(bob->ifController, String_CONST("bob"), alloc);
    Iface_plumb(&sc->bobIf, &bobIci->addrIf);

    CryptoAuth_addUser(String_CONST("abcdefg123"), String_CONST("TEST"), bob->ca);

    // Client has pubKey and passwd for the server.
    int ret = InterfaceController_bootstrapPeer(alice->ifController,
                                                aliceIci->ifNum,
                                                bob->ca->publicKey,
                                                Sockaddr_LOOPBACK,
                                                String_CONST("abcdefg123"),
                                                NULL,
                                                NULL,
                                                alloc);
    Assert_true(!ret);

    Iface_unplumb(alice->switchAdapter->controlIf.connectedIf, &alice->switchAdapter->controlIf);
    Iface_plumb(&alice->switchAdapter->controlIf, &sc->aliceCtrlIf);

    struct Message* msg = Message_new(Control_Ping_MIN_SIZE + Control_Header_SIZE, 256, alloc);
    struct Control_Header* ch = (struct Control_Header*) msg->bytes;
    struct Control_Ping* ping = (struct Control_Ping*) &ch[1];
    ping->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
    Message_push32(msg, 0xffffffff, NULL);
    uint32_t* handle_be = (uint32_t*)msg->bytes;
    Message_push(msg, NULL, SwitchHeader_SIZE, NULL);
    struct SwitchHeader* sh = (struct SwitchHeader*) msg->bytes;
    // TODO(cjd): this will fail with a different encoding scheme
    sh->label_be = Endian_hostToBigEndian64(0x13);

    for (int i = 1; i < 6; i++) {
        ping->magic = Control_Ping_MAGIC;
        ch->type_be = Control_PING_be;
        ch->checksum_be = 0;
        ch->checksum_be = Checksum_engine((void*)ch, Control_Ping_MIN_SIZE + Control_Header_SIZE);

        Iface_send(&sc->aliceCtrlIf, msg);

        Assert_true(sc->msgCount == i);
        Assert_true(msg->bytes == (void*)sh);
        Assert_true(ping->magic == Control_Pong_MAGIC);
        Assert_true(ch->type_be = Control_PONG_be);
        Assert_true(!Checksum_engine((void*)ch, Control_Ping_MIN_SIZE + Control_Header_SIZE));
    }

    *handle_be = 0xfffffff0;
    int count = 1000000;
    begin(ctx, "Switching", count, "packets");
    for (int i = 0; i < count; i++) {
        sh->versionAndLabelShift = SwitchHeader_CURRENT_VERSION << 6;
        Iface_send(&sc->aliceCtrlIf, msg);
        Assert_true(msg->bytes == (void*)sh);
    }
    done(ctx);

    Log_info(ctx->log, "DONE");
    Allocator_free(alloc);
}

/** Check if nodes A and C can communicate via B without A knowing that C exists. */
void Benchmark_runAll()
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->alloc = alloc;
    ctx->base = EventBase_new(alloc);
    struct Log* log = ctx->log = FileWriterLog_new(stdout, alloc);
    ctx->rand = Random_new(alloc, log, NULL);

    cryptoAuth(ctx);
    switching(ctx);
}
