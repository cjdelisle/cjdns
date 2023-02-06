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
#include "crypto/Sign_admin.h"

#include "admin/Admin.h"
#include "benc/String.h"
#include "crypto/Key.h"
#include "crypto/Sign.h"
#include "dht/Address.h"
#include "exception/Er.h"
#include "memory/Allocator.h"
#include "util/Base32.h"
#include "util/Identity.h"
#include "wire/Message.h"

struct Context {
    uint8_t signingKeypair[64];
    uint8_t pubSigningKey[64];
    struct Random* rand;
    struct Allocator* alloc;
    struct Admin* admin;
    Identity
};

static void checkSig(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    String* msgHash = Dict_getStringC(args, "msgHash");
    String* signature = Dict_getStringC(args, "signature");
    Dict* out = Dict_new(requestAlloc);
    uint8_t publicSigningKey[32];
    uint8_t sigBytes[64];
    char* underscore = CString_strchr(signature->bytes, '_');
    if (msgHash->len > 64) {
        Dict_putStringCC(out, "error", "msgHash too long, max 64 bytes", requestAlloc);
    } else if (underscore == NULL) {
        Dict_putStringCC(out, "error",
            "malformed signature, missing separator", requestAlloc);
    } else if (Base32_decode(
        publicSigningKey, 32, signature->bytes, (int)(underscore - signature->bytes)) != 32)
    {
        Dict_putStringCC(out, "error",
            "malformed signature, failed to decode pubkey", requestAlloc);
    } else if (Base32_decode(
        sigBytes, 64, &underscore[1], CString_strlen(&underscore[1])) != 64)
    {
        Dict_putStringCC(out, "error",
            "malformed signature, failed to decode signature", requestAlloc);
    } else {
        struct Message* msg = Message_new(0, msgHash->len + 64, requestAlloc);
        Er_assert(Message_epush(msg, msgHash->bytes, msgHash->len));
        Er_assert(Message_epush(msg, sigBytes, 64));
        uint8_t curve25519key[32];
        if (Sign_verifyMsg(publicSigningKey, msg)) {
            Dict_putStringCC(out, "error", "invalid signature", requestAlloc);
        } else if (Sign_publicSigningKeyToCurve25519(curve25519key, publicSigningKey)) {
            Dict_putStringCC(out, "error", "not a valid curve25519 key", requestAlloc);
        } else {
            struct Address addr = {0};
            Address_forKey(&addr, curve25519key);
            uint8_t ipv6[40];
            Address_printIp(ipv6, &addr);
            String* k = Key_stringify(curve25519key, requestAlloc);
            Dict_putStringC(out, "pubkey", k, requestAlloc);
            Dict_putStringCC(out, "ipv6", ipv6, requestAlloc);
            Dict_putStringCC(out, "error", "none", requestAlloc);
        }
    }
    Admin_sendMessage(out, txid, ctx->admin);
}

static void sign(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    String* msgHash = Dict_getStringC(args, "msgHash");
    Dict* out = Dict_new(requestAlloc);
    if (msgHash->len > 64) {
        Dict_putStringCC(out, "error", "msgHash too long, max 64 bytes", requestAlloc);
    } else {
        struct Message* msg = Message_new(0, msgHash->len + 64, requestAlloc);
        Er_assert(Message_epush(msg, msgHash->bytes, msgHash->len));
        Sign_signMsg(ctx->signingKeypair, msg, ctx->rand);
        uint8_t signB64[128];
        Assert_true(Base32_encode(signB64, 128, msg->msgbytes, 64) > 0);
        String* sig = String_printf(requestAlloc, "%s_%s", ctx->pubSigningKey, signB64);
        Dict_putStringC(out, "signature", sig, requestAlloc);
        Dict_putStringCC(out, "error", "none", requestAlloc);
    }
    Admin_sendMessage(out, txid, ctx->admin);
}

void Sign_admin_register(uint8_t* privateKey,
                         struct Admin* admin,
                         struct Random* rand,
                         struct Allocator* alloc)
{
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Sign_signingKeyPairFromCurve25519(ctx->signingKeypair, privateKey);
    uint8_t sPubKey[32];
    Sign_publicKeyFromKeyPair(sPubKey, ctx->signingKeypair);
    Assert_true(Base32_encode(ctx->pubSigningKey, 64, sPubKey, 32) > 0);

    ctx->alloc = alloc;
    ctx->rand = rand;
    ctx->admin = admin;
    Identity_set(ctx);

    Admin_registerFunction("Sign_checkSig", checkSig, ctx, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "msgHash", .required = true, .type = "String" },
            { .name = "signature", .required = true, .type = "String" },
        }), admin);
    Admin_registerFunction("Sign_sign", sign, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "msgHash", .required = true, .type = "String" },
        }), admin);
}
