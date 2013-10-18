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
#include "interface/DNSServer.h"
#include "benc/String.h"
#include "exception/Jmp.h"
#include "crypto/AddressCalc.h"
#include "util/Base32.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Error.h"

enum ResponseCode {
    ResponseCode_NO_ERROR = 0,
    ResponseCode_FORMAT_ERROR = 1,
    ResponseCode_SERVER_ERROR = 2,
    ResponseCode_NXDOMAIN = 3,
    ResponseCode_NOT_IMPLEMENTED = 4,
    ResponseCode_REFUSED = 5,
    ResponseCode_XYDOMAIN = 6,
    ResponseCode_YXRRSET = 7,
    ResponseCode_NXRRSET = 8,
    ResponseCode_NOT_AUTH_FOR_ZONE = 9,
};

enum Type {
    Type_TXT = 16,
    Type_AAAA = 28
};

enum Class {
    Class_IN = 1
};

struct DNSServer_RR
{
    String** name;
    uint16_t type;
    uint16_t class;
    uint32_t ttl;
    String* data;
};

struct DNSServer_Question
{
    String** name;
    uint16_t class;
    uint16_t type;
};

struct DNSServer_Flags
{
    int responseCode;
    bool cd;
    bool authenticated;
    bool z;
    bool recursionAvailable;
    bool recursionDesired;
    bool truncated;
    bool authoritative;
    int opCode;
    bool isResponse;
};

struct DNSServer_Message
{
    uint16_t id;
    struct DNSServer_Flags flags;
    struct DNSServer_Question** questions;
    struct DNSServer_RR** answers;
    struct DNSServer_RR** authorities;
    struct DNSServer_RR** additionals;
};

struct DNSServer_pvt
{
    struct DNSServer pub;
    struct Interface* iface;
    struct Log* logger;
    struct RainflyClient* rainfly;
    struct Sockaddr* addr;

    /* Plain old DNS servers for resolving non-cjdns addresses. */
    int serverCount;
    struct Sockaddr** servers;

    struct Allocator* alloc;

    Identity
};

static void parseFlags(uint16_t flags, struct DNSServer_Flags* out)
{
    #define POP(count) flags & ((1<<(count))-1); flags >>= (count)
    out->responseCode = POP(4);
    out->cd = POP(1);
    out->authenticated = POP(1);
    out->z = POP(1);
    out->recursionAvailable = POP(1);
    out->recursionDesired = POP(1);
    out->truncated = POP(1);
    out->authoritative = POP(1);
    out->opCode = POP(4);
    out->isResponse = POP(1);
    Assert_true(flags == 0);
    #undef POP
}

static uint16_t flagsAsInt(struct DNSServer_Flags* flags)
{
    uint16_t out = 0;
    #define PUSH(count, data) out <<= (count); out |= ((data) & ((1<<(count))-1))
    PUSH(1, flags->isResponse);
    PUSH(4, flags->opCode);
    PUSH(1, flags->authoritative);
    PUSH(1, flags->truncated);
    PUSH(1, flags->recursionDesired);
    PUSH(1, flags->recursionAvailable);
    PUSH(1, flags->z);
    PUSH(1, flags->authenticated);
    PUSH(1, flags->cd);
    PUSH(4, flags->responseCode);
    return out;
    #undef PUSH
}

static String** parseName(struct Message* msg, struct Allocator* alloc)
{
    String** array = Allocator_calloc(alloc, sizeof(uintptr_t), 1);
    uint8_t len = Message_pop8(msg);
    int count = 0;
    while (len) {
        count++;
        array = Allocator_realloc(alloc, array, sizeof(uintptr_t) * count + 1);
        array[count] = NULL;
        String* str = array[count-1] = String_newBinary(NULL, len, alloc);
        Message_pop(msg, str->bytes, len);
        len = Message_pop8(msg);
    }
    return array;
}

static void serializeName(struct Message* msg, String** name)
{
    Message_push8(msg, 0);
    int i;
    for (i = 0; name[i]; i++) ;
    i--;
    for (;i >= 0; i--) {
        Message_push(msg, name[i]->bytes, name[i]->len);
        Message_push8(msg, name[i]->len);
    }
}

static struct DNSServer_Question* parseQuestion(struct Message* msg, struct Allocator* alloc)
{
    struct DNSServer_Question* q = Allocator_malloc(alloc, sizeof(struct DNSServer_Question));
    q->name = parseName(msg, alloc);
    q->type = Message_pop16(msg);
    q->class = Message_pop16(msg);
    return q;
}

static void serializeQuestion(struct Message* msg, struct DNSServer_Question* q)
{
    Message_push16(msg, q->class);
    Message_push16(msg, q->type);
    serializeName(msg, q->name);
}

static struct DNSServer_RR* parseRR(struct Message* msg, struct Allocator* alloc)
{
    struct DNSServer_RR* rr = Allocator_malloc(alloc, sizeof(struct DNSServer_RR));
    rr->name = parseName(msg, alloc);
    rr->type = Message_pop16(msg);
    rr->class = Message_pop16(msg);
    rr->ttl = Message_pop32(msg);
    uint16_t dataLen = Message_pop16(msg);
    rr->data = String_newBinary(NULL, dataLen, alloc);
    Message_pop(msg, rr->data->bytes, dataLen);
    return rr;
}

static void serializeRR(struct Message* msg, struct DNSServer_RR* rr)
{
    Message_push(msg, rr->data->bytes, rr->data->len);
    Message_push16(msg, rr->data->len);
    Message_push32(msg, rr->ttl);
    Message_push16(msg, rr->class);
    Message_push16(msg, rr->type);
    serializeName(msg, rr->name);
}

static struct DNSServer_Message* parseMessage(struct Message* msg, struct Allocator* alloc)
{
    struct DNSServer_Message* dmesg = Allocator_calloc(alloc, sizeof(struct DNSServer_Message), 1);
    dmesg->id = Message_pop16(msg);

    uint16_t flags = Message_pop16(msg);
    parseFlags(flags, &dmesg->flags);

    int totalQuestions = Message_pop16(msg);
    int totalAnswerRRs = Message_pop16(msg);
    int totalAuthorityRRs = Message_pop16(msg);
    int totalAdditionalRRs = Message_pop16(msg);

    dmesg->questions = Allocator_calloc(alloc, sizeof(uintptr_t), totalQuestions+1);
    dmesg->answers = Allocator_calloc(alloc, sizeof(uintptr_t), totalAnswerRRs+1);
    dmesg->authorities = Allocator_calloc(alloc, sizeof(uintptr_t), totalAuthorityRRs+1);
    dmesg->additionals = Allocator_calloc(alloc, sizeof(uintptr_t), totalAdditionalRRs+1);

    for (int i = 0; i < totalQuestions; i++) {
        dmesg->questions[i] = parseQuestion(msg, alloc);
    }
    for (int i = 0; i < totalAnswerRRs; i++) {
        dmesg->answers[i] = parseRR(msg, alloc);
    }
    for (int i = 0; i < totalAuthorityRRs; i++) {
        dmesg->authorities[i] = parseRR(msg, alloc);
    }
    for (int i = 0; i < totalAdditionalRRs; i++) {
        dmesg->additionals[i] = parseRR(msg, alloc);
    }

    return dmesg;
}

static void serializeMessage(struct Message* msg, struct DNSServer_Message* dmesg)
{
    int totalAdditionalRRs = 0;
    int totalAuthorityRRs = 0;
    int totalAnswerRRs = 0;
    int totalQuestions = 0;

    for (int i = 0; dmesg->additionals && dmesg->additionals[i]; i++) {
        serializeRR(msg, dmesg->additionals[i]);
        totalAdditionalRRs++;
    }
    for (int i = 0; dmesg->authorities && dmesg->authorities[i]; i++) {
        serializeRR(msg, dmesg->authorities[i]);
        totalAuthorityRRs++;
    }
    for (int i = 0; dmesg->answers && dmesg->answers[i]; i++) {
        serializeRR(msg, dmesg->answers[i]);
        totalAnswerRRs++;
    }
    for (int i = 0; dmesg->questions && dmesg->questions[i]; i++) {
        serializeQuestion(msg, dmesg->questions[i]);
        totalQuestions++;
    }

    Message_push16(msg, totalAdditionalRRs);
    Message_push16(msg, totalAuthorityRRs);
    Message_push16(msg, totalAnswerRRs);
    Message_push16(msg, totalQuestions);

    Message_push16(msg, flagsAsInt(&dmesg->flags));
    Message_push16(msg, dmesg->id);
}

static int cannonicalizeDomain(String* str)
{
    #define XX(x,y) [((uint8_t)x)]=y,
    static const char numForAscii[256] =
    {
        XX('a','a') XX('b','b') XX('c','c') XX('d','d')
        XX('e','e') XX('f','f') XX('g','g') XX('h','h')
        XX('i','i') XX('j','j') XX('k','k') XX('l','l')
        XX('m','m') XX('n','n') XX('o','o') XX('p','p')
        XX('q','q') XX('r','r') XX('s','s') XX('t','t')
        XX('u','u') XX('v','v') XX('w','w') XX('x','x')
        XX('y','y') XX('z','z')

        XX('A','a') XX('B','b') XX('C','c') XX('D','d')
        XX('E','e') XX('F','f') XX('G','g') XX('H','h')
        XX('I','i') XX('J','j') XX('K','k') XX('L','l')
        XX('M','m') XX('N','n') XX('O','o') XX('P','p')
        XX('Q','q') XX('R','r') XX('S','s') XX('T','t')
        XX('U','u') XX('V','v') XX('W','w') XX('X','x')
        XX('Y','y') XX('Z','z')

        XX('0','0') XX('1','1') XX('2','2') XX('3','3')
        XX('4','4') XX('5','5') XX('6','6') XX('7','7')
        XX('8','8') XX('9','9')

        XX('-','-')
    };
    #undef XX
    for (int i = 0; i < (int)str->len; i++) {
        if (!(str->bytes[i] = numForAscii[(uint8_t)str->bytes[i]])) {
            return -1;
        }
    }
    return 0;
}

static uint8_t sendResponse(struct Message* msg,
                            struct DNSServer_Message* dmesg,
                            struct Sockaddr* sourceAddr,
                            struct DNSServer_pvt* ctx)
{
    dmesg->flags.isResponse = 1;
    serializeMessage(msg, dmesg);
    Message_push(msg, sourceAddr, ctx->addr->addrLen);
    return Interface_sendMessage(ctx->iface, msg);
}

static uint8_t handleDotK(struct Message* msg,
                          struct DNSServer_Message* dmesg,
                          struct DNSServer_Question* q,
                          struct Sockaddr* sourceAddr,
                          struct DNSServer_pvt* ctx)
{
    // .k address lookup
    String* domain = q->name[0];
    uint8_t publicKey[32];
    uint8_t ipv6[16];
    if (Base32_decode(publicKey, 32, domain->bytes, domain->len) != 32) {
        Log_debug(ctx->logger, "Unparsable .k address [%s]", domain->bytes);

    } else if (!AddressCalc_addressForPublicKey(ipv6, publicKey)) {
        Log_debug(ctx->logger, "Invalid (not fc prefix) .k address [%s]", q->name[0]->bytes);

    } else if (q->type != Type_AAAA || q->class != Class_IN) {
        Bits_memset(&dmesg->flags, 0, sizeof(struct DNSServer_Flags));
        dmesg->flags.responseCode = ResponseCode_NO_ERROR;
        dmesg->answers = NULL;
        dmesg->authorities = NULL;
        dmesg->additionals = NULL;
        sendResponse(msg, dmesg, sourceAddr, ctx);
        return Error_NONE;

    } else {
        struct DNSServer_RR rr = {
            .name = q->name,
            .type = Type_AAAA,
            .class = Class_IN,
            .ttl = ~0,
            .data = &(String) { .bytes = ipv6, .len = 16 }
        };
        struct DNSServer_RR* answers[2] = { &rr, NULL };
        dmesg->answers = answers;
        dmesg->authorities = NULL;
        dmesg->additionals = NULL;
        Bits_memset(&dmesg->flags, 0, sizeof(struct DNSServer_Flags));
        dmesg->flags.responseCode = ResponseCode_NO_ERROR;
        sendResponse(msg, dmesg, sourceAddr, ctx);

        return Error_NONE;
    }

    Bits_memset(&dmesg->flags, 0, sizeof(struct DNSServer_Flags));
    dmesg->flags.responseCode = ResponseCode_NXDOMAIN;
    dmesg->answers = NULL;
    dmesg->authorities = NULL;
    dmesg->additionals = NULL;
    sendResponse(msg, dmesg, sourceAddr, ctx);
    return Error_NONE;
}

struct DNSServer_RainflyRequest
{
    struct RainflyClient_Lookup* lookup;
    struct Sockaddr* addr;
    struct Message* msg;
    struct DNSServer_Message* dmesg;
    struct DNSServer_pvt* ctx;
    Identity
};

static void onRainflyReply(struct RainflyClient_Lookup* promise,
                           Dict* value,
                           enum RainflyClient_ResponseCode code)
{
    struct DNSServer_RainflyRequest* lookup =
        Identity_cast((struct DNSServer_RainflyRequest*)promise->userData);
    struct DNSServer_pvt* ctx =
        Identity_cast((struct DNSServer_pvt*)lookup->ctx);

    struct DNSServer_Message* dmesg = lookup->dmesg;
    struct DNSServer_Question* q = dmesg->questions[0];

    String* cjdns = Dict_getString(value, String_CONST("cjdns"));
    uint8_t publicKey[32];
    uint8_t ipv6[16];
    if (code) {
        // got error code
    } else if (!cjdns || cjdns->len < 3) {
        // no error, no answer

    } else if (Base32_decode(publicKey, 32, cjdns->bytes, cjdns->len-2) != 32) {
        Log_debug(ctx->logger, "Unparsable .k address [%s]", cjdns->bytes);

    } else if (!AddressCalc_addressForPublicKey(ipv6, publicKey)) {
        Log_debug(ctx->logger, "Invalid (not fc prefix) .k address [%s]", cjdns->bytes);

    } else {

        struct DNSServer_RR rr = {
            .name = q->name,
            .type = Type_AAAA,
            .class = Class_IN,
            .ttl = 153600,
            .data = &(String) { .bytes = ipv6, .len = 16 }
        };
        struct DNSServer_RR* answers[2] = { &rr, NULL };
        dmesg->answers = answers;
        dmesg->authorities = NULL;
        dmesg->additionals = NULL;
        Bits_memset(&dmesg->flags, 0, sizeof(struct DNSServer_Flags));
        dmesg->flags.responseCode = ResponseCode_NO_ERROR;
        sendResponse(lookup->msg, dmesg, lookup->addr, ctx);

        return;
    }

    Bits_memset(&dmesg->flags, 0, sizeof(struct DNSServer_Flags));
    dmesg->flags.responseCode = code;
    dmesg->answers = NULL;
    dmesg->authorities = NULL;
    dmesg->additionals = NULL;
    sendResponse(lookup->msg, dmesg, lookup->addr, ctx);
    return;
}

static uint8_t handleDotH(struct Message* msg,
                          struct DNSServer_Message* dmesg,
                          struct DNSServer_Question* q,
                          String* domain,
                          struct Sockaddr* sourceAddr,
                          struct DNSServer_pvt* ctx)
{
    // .h address lookup
    if (q->type != Type_AAAA || q->class != Class_IN) {
        Bits_memset(&dmesg->flags, 0, sizeof(struct DNSServer_Flags));
        dmesg->flags.responseCode = ResponseCode_NO_ERROR;
        dmesg->answers = NULL;
        dmesg->authorities = NULL;
        dmesg->additionals = NULL;
        sendResponse(msg, dmesg, sourceAddr, ctx);
        return Error_NONE;
    }

    if (cannonicalizeDomain(domain)) {
        // invalid domain
        Bits_memset(&dmesg->flags, 0, sizeof(struct DNSServer_Flags));
        dmesg->flags.responseCode = ResponseCode_NXDOMAIN;
        dmesg->answers = NULL;
        dmesg->authorities = NULL;
        dmesg->additionals = NULL;
        sendResponse(msg, dmesg, sourceAddr, ctx);
        return Error_NONE;
    }

    struct RainflyClient_Lookup* lookup = RainflyClient_lookup(ctx->rainfly, domain);
    Allocator_adopt(lookup->alloc, msg->alloc);
    struct DNSServer_RainflyRequest* req =
        Allocator_calloc(lookup->alloc, sizeof(struct DNSServer_RainflyRequest), 1);
    req->lookup = lookup;
    req->addr = sourceAddr;
    req->msg = msg;
    req->dmesg = dmesg;
    req->ctx = ctx;
    Identity_set(req);
    lookup->onReply = onRainflyReply;
    lookup->userData = req;

    return Error_NONE;
}

static void legacyResponse(struct Message* msg,
                           struct DNSServer_Message* dmesg,
                           struct DNSServer_Question* q,
                           String* domain,
                           struct Sockaddr* sourceAddr,
                           struct DNSServer_pvt* ctx)
{
}

static void receiveB(struct Message* msg, struct DNSServer_pvt* ctx)
{
    struct Sockaddr* sourceAddr = Allocator_malloc(msg->alloc, ctx->addr->addrLen);
    Message_pop(msg, sourceAddr, ctx->addr->addrLen);

    struct DNSServer_Message* dmesg = parseMessage(msg, msg->alloc);

    if (msg->length > 0) {
        Log_debug(ctx->logger, "[%d] bytes of crap following DNS request", msg->length);
        Message_shift(msg, -(msg->length));
    }

    if (!dmesg->questions || !dmesg->questions[0]) {
        Log_debug(ctx->logger, "Got DNS query with no questions");
        return;
    }
    struct DNSServer_Question* q = dmesg->questions[0];

    if (!q->name[0] || !q->name[1]) {
        Log_debug(ctx->logger, "Missing domain");
        return;
    }

    String* tld = NULL;
    String* domain = NULL;
    for (int i = 0; q->name[i]; i++) {
        if (cannonicalizeDomain(q->name[i])) {
            Log_debug(ctx->logger, "Invalid chars in domain");
            return;
        }
        tld = q->name[i];
        if (i > 0) {
            domain = q->name[i-1];
        }
    }

    if (String_equals(tld, String_CONST("h"))) {
        handleDotH(msg, dmesg, q, domain, sourceAddr, ctx);
        return;

    } else if (String_equals(tld, String_CONST("k"))) {
        if (q->name[2]) {
            Log_debug(ctx->logger, ".k domain with subdomain");
            return;
        }
        handleDotK(msg, dmesg, q, sourceAddr, ctx);
        return;

    } else if (dmesg->answers) {
        // response from a legacy server
        legacyResponse(msg, dmesg, q, domain, sourceAddr, ctx);
        return;

//    } else if (ctx->serverCount > 0) {
//
  //      return;

    } else {
        // not authoritative for zone
        Bits_memset(&dmesg->flags, 0, sizeof(struct DNSServer_Flags));
        dmesg->flags.responseCode = ResponseCode_NOT_AUTH_FOR_ZONE;
        dmesg->answers = NULL;
        dmesg->authorities = NULL;
        dmesg->additionals = NULL;
        sendResponse(msg, dmesg, sourceAddr, ctx);
    }
    return;
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct DNSServer_pvt* const ctx = Identity_cast((struct DNSServer_pvt*)iface->receiverContext);

    struct Jmp jmp = {.code=0};
    Jmp_try(jmp) {
        Except_setDefaultHandler(&jmp.handler);
        receiveB(msg, ctx);
    } Jmp_catch {
        Log_debug(ctx->logger, "Failed to parse dns message [%s]", jmp.message);
    }
    Except_setDefaultHandler(NULL);
    return Error_NONE;
}

///////////

int DNSServer_addServer(struct DNSServer* dns, struct Sockaddr* addr)
{
    struct DNSServer_pvt* ctx = Identity_cast((struct DNSServer_pvt*)dns);

    if (addr->addrLen != ctx->addr->addrLen
        || Sockaddr_getFamily(addr) != Sockaddr_getFamily(ctx->addr))
    {
        return DNSServer_addServer_WRONG_ADDRESS_TYPE;
    }
    ctx->serverCount++;
    ctx->servers =
        Allocator_realloc(ctx->alloc, ctx->servers, ctx->serverCount * sizeof(uintptr_t));
    ctx->servers[ctx->serverCount-1] = Sockaddr_clone(addr, ctx->alloc);
    return 0;
}

struct DNSServer* DNSServer_new(struct AddrInterface* iface,
                                struct Log* logger,
                                struct RainflyClient* rainfly)
{
    struct DNSServer_pvt* context =
        Allocator_clone(iface->generic.allocator, (&(struct DNSServer_pvt) {
            .iface = &iface->generic,
            .logger = logger,
            .rainfly = rainfly,
            .addr = iface->addr,
            .alloc = iface->generic.allocator
        }));
    Identity_set(context);

    iface->generic.receiveMessage = receiveMessage;
    iface->generic.receiverContext = context;

    return &context->pub;
}
