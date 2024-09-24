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
#include "subnode/PeeringSeeder.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "crypto/Ca.h"
#include "dht/Address.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "net/SwitchPinger.h"
#include "rust/cjdns_sys/Rffi.h"
#include "subnode/ReachabilityCollector.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Identity.h"
#include "util/events/EventBase.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/log/Log.h"

typedef struct PeeringSeeder_pvt {
    PeeringSeeder_t pub;

    struct SwitchPinger* sp;
    struct ReachabilityCollector* rc;
    Allocator_t* alloc;
    Log_t* log;
    struct MsgCore* mc;
    Ca_t* ca;

    Rffi_Seeder* seeder;

    bool active;
    char login[24];

    // Our peering credentials
    uint16_t userNum;
    // 8 random bytes used to derive the password
    uint64_t pass;

    int lastPeerQueried;
    uint64_t lastQueryTimeSec;
    struct SwitchPinger_Ping* requestOutstanding;

    struct Address snode;
    uint64_t lastPostTimeSec;
    struct MsgCore_Promise* snodePost;

    Identity
} PeeringSeeder_pvt_t;

void* PeeringSeeder_getRsSeeder(PeeringSeeder_t* self)
{
    PeeringSeeder_pvt_t* pq = Identity_check((PeeringSeeder_pvt_t*) self);
    return pq->seeder;
}

static void ipQueryResp(struct SwitchPinger_Response* resp, void* userData)
{
    PeeringSeeder_pvt_t* pq = Identity_check((PeeringSeeder_pvt_t*) userData);
    if (resp->ping != pq->requestOutstanding) {
        Log_info(pq->log, "Ping does not match requestOutstanding");
    }
    pq->requestOutstanding = NULL;
    char* err = "";
    switch (resp->res) {
        case SwitchPinger_Result_OK:
            if (Rffi_Seeder_got_lladdr(pq->seeder, &resp->lladdr)) {
                // Since there was a change, post ASAP.
                pq->lastPostTimeSec = 0;
            }
            break;
        case SwitchPinger_Result_LABEL_MISMATCH: err = "LABEL_MISMATCH"; break;
        case SwitchPinger_Result_WRONG_DATA: err = "WRONG_DATA"; break;
        case SwitchPinger_Result_ERROR_RESPONSE: err = "ERROR_RESPONSE"; break;
        case SwitchPinger_Result_LOOP_ROUTE: err = "LOOP_ROUTE"; break;
        case SwitchPinger_Result_TIMEOUT: err = "TIMEOUT"; break;
        default: err = "unknown error"; break;
    }
    Log_debug(pq->log, "Error sending LLADDR query to peer [%llx] [%s]",
        (long long)resp->label, err);
}

static void publicIPQuery(PeeringSeeder_pvt_t* pq) {
    if (pq->requestOutstanding) {
        return;
    }

    uint64_t now = Time_currentTimeSeconds();
    if (pq->lastQueryTimeSec + 10 < now) {
        return;
    }
    if (Rffi_Seeder_has_lladdr(pq->seeder)) {
        if (pq->lastQueryTimeSec + 300 < now) {
            return;
        }
    }

    int peerCount = ReachabilityCollector_peerCount(pq->rc);
    if (!peerCount) {
        return;
    }
    int tryPeerN = (pq->lastPeerQueried + 1) % peerCount;
    pq->lastPeerQueried = tryPeerN;
    struct ReachabilityCollector_PeerInfo* tryPeer =
        ReachabilityCollector_getPeerInfo(pq->rc, tryPeerN);
    
    struct Allocator* alloc = Allocator_child(pq->alloc);
    struct SwitchPinger_Ping* q = SwitchPinger_newPing(
        tryPeer->addr.path,
        String_new("", alloc),
        10000,
        ipQueryResp,
        alloc,
        pq->sp);
    q->onResponseContext = pq;
    q->type = SwitchPinger_Type_LLADDR;
    pq->requestOutstanding = q;

    String_t* peerAddr = Address_toString(&tryPeer->addr, q->pingAlloc);
    Log_debug(pq->log, "Sent LlAddr query to [%s]", peerAddr->bytes);
}

static void snodeResp(Dict* msg, struct Address* src, struct MsgCore_Promise* prom)
{
    PeeringSeeder_pvt_t* pq = Identity_check((PeeringSeeder_pvt_t*) prom->userData);
    String* peers = Dict_getStringC(msg, "pr");
    if (!peers) {
        Log_debug(pq->log, "Got snode reply with no peers");
        return;
    }
    Rffi_Seeder_got_peers(pq->seeder, peers, prom->alloc);
}

static void cycle(void* vpq)
{
    PeeringSeeder_pvt_t* pq = Identity_check((PeeringSeeder_pvt_t*) vpq);

    if (!pq->active) {
        return;
    }

    // Try to get our IP if we can
    publicIPQuery(pq);

    uint64_t now = Time_currentTimeSeconds();
    if (pq->lastPostTimeSec + 5*60 > now) {
        // Only post to the snode every 5 minutes (or on new news)
        return;
    }
    if (!Rffi_Seeder_has_lladdr(pq->seeder)) {
        return;
    }
    if (Bits_isZero(&pq->snode, sizeof pq->snode)) {
        return;
    }

    struct MsgCore_Promise* snodeQuery = MsgCore_createQuery(pq->mc, 0, pq->alloc);
    snodeQuery->cb = snodeResp;
    snodeQuery->userData = pq;
    snodeQuery->target = Address_clone(&pq->snode, snodeQuery->alloc);

    String* creds = NULL;
    Err_assert(Rffi_Seeder_mk_creds(pq->seeder, &creds, snodeQuery->alloc));

    snodeQuery->msg = Dict_new(snodeQuery->alloc);
    Dict_putStringCC(snodeQuery->msg, "sq", "pc", snodeQuery->alloc);
    Dict_putStringC(snodeQuery->msg, "pc", creds, snodeQuery->alloc);

    pq->snodePost = snodeQuery;

    Log_debug(pq->log, "Posting our public IP addresses to snode");
}

void PeeringSeeder_setSnode(PeeringSeeder_t* self, struct Address* snode)
{
    PeeringSeeder_pvt_t* pq = Identity_check((PeeringSeeder_pvt_t*) self);
    struct Address esnode = { .path = 0 };
    if (snode != NULL) {
        Bits_memcpy(&esnode, snode, sizeof esnode);
    }
    if (Bits_memcmp(&esnode, &pq->snode, sizeof esnode)) {
        uint8_t fromAddr[60];
        Address_print(fromAddr, &pq->snode);
        if (snode == NULL) {
            Log_debug(pq->log, "Snode [%s] was lost", fromAddr);
        } else {
            uint8_t toAddr[60];
            Address_print(toAddr, &pq->snode);
            if (Bits_isZero(&pq->snode, sizeof pq->snode)) {
                Log_debug(pq->log, "Snode discovered [%s]", toAddr);
            } else {
                Log_debug(pq->log, "Snode was changed [%s] -> [%s]", fromAddr, toAddr);
            }
        }
        pq->snode = esnode;
    }
}

Err_DEFUN PeeringSeeder_publicPeer(PeeringSeeder_t* self, String_t* code, Allocator_t* reqAlloc)
{
    PeeringSeeder_pvt_t* pq = Identity_check((PeeringSeeder_pvt_t*) self);
    if (code == NULL) {
        if (pq->active) {
            String_t* s = String_new(pq->login, reqAlloc);
            Ca_removeUsers(pq->ca, s);
        }
        return NULL;
    }
    if (code->len > PeeringSeeder_publicPeer_ID_MAX_LEN) {
        Err_raise(reqAlloc, "PeerID is too long, max length: [%d]",
            PeeringSeeder_publicPeer_ID_MAX_LEN);
    }
    String_t* user = NULL;
    String_t* pass = NULL;
    Err(Rffi_Seeder_public_peer(
        pq->seeder,
        &user,
        &pass,
        pq->userNum,
        pq->pass,
        code,
        reqAlloc));

    Assert_true(user->len < sizeof pq->login);
    Bits_memset(pq->login, 0, sizeof pq->login);
    Bits_memcpy(pq->login, user->bytes, user->len);
    Assert_true(pq->login[sizeof pq->login - 1] == 0);

    Ca_addUser(pass, user, pq->ca);
    pq->active = true;
    return NULL;
}

struct PeeringSeeder* PeeringSeeder_new(
    struct SwitchPinger* sp,
    struct ReachabilityCollector* rc,
    Allocator_t* alloc,
    Log_t* log,
    struct MsgCore* mc,
    EventBase_t* base,
    Ca_t* ca)
{
    PeeringSeeder_pvt_t* out = Allocator_calloc(alloc, sizeof(PeeringSeeder_pvt_t), 1);
    Identity_set(out);
    out->sp = sp;
    out->rc = rc;
    out->alloc = alloc;
    out->log = log;
    out->mc = mc;
    out->ca = ca;

    uint8_t myPubKey[32];
    Ca_getPubKey(ca, myPubKey);
    Rffi_Seeder_new(&out->seeder, &out->pub.seederIface, myPubKey, alloc);

    // Random (stable across restarts) user num and passwd
    uint8_t secretBuf[64];
    Assert_true(!Ca_getSecret(ca, String_CONST("SEED_PASSWD"), secretBuf));
    Bits_memcpy(&out->pass, secretBuf, sizeof out->pass);
    Bits_memcpy(&out->userNum, &secretBuf[8], sizeof out->userNum);

    Timeout_setInterval(cycle, out, 3000, base, alloc);

    return &out->pub;
}