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
#include "wire/Message.h"


enum RCode {
    RCode_NO_ERROR = 0,
    RCode_FORMAT_ERROR = 1,
    RCode_SERVER_ERROR = 2,
    RCode_NXDOMAIN = 3,
    RCode_NOT_IMPLEMENTED = 4,
    RCode_REFUSED = 5,
    RCode_XYDOMAIN = 6,
    RCode_YXRRSET = 7,
    RCode_NXRRSET = 8,
    RCode_NOT_AUTH = 9,
};

enum Record {
    Record_TXT = 16,
    Record_AAAA = 28
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
}

static void serializeName(struct Message* msg, String** name)
{
    Message_push8(msg, 0);
    for (int i = 0; name[i]; i++) {
        Message_push(msg, name[i]->bytes, name[i]->len);
        Message.push8(msg, name[i]->len);
    }
}

static struct DNSServer_Question* parseQuestion(struct Message* msg, struct Allocator* alloc)
{
    struct DNSServer_Question* q = Allocator_malloc(alloc, sizeof(struct DNSServer_Question));
    q->name = parseName(msg, alloc);
    q->type = Message_pop16(msg);
    q->class = Message_pop16(msg);
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
    rr->name = parseName(msg, alloc, eh);
    rr->type = Message_pop16(msg);
    rr->class = Message_pop16(msg);
    rr->ttl = Message_pop32(msg);
    uint16_t dataLen = Message_pop16(msg);
    rr->data = String_newBinary(NULL, dataLen, alloc);
    Message_pop(msg, rr->data->bytes, dataLen);
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
    struct DNSServer_Message* dmsg = Allocator_calloc(alloc, sizeof(struct DNSServer_Message), 1);
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
        serializeRR(msg, answers->answers[i]);
        totalAnswerRRs++;
    }
    for (int i = 0; dmesg->questions && dmesg->questions[i]; i++) {
        serializeQuestion(msg, dmesg->questions[i]);
        totalQuestions++
    }

    Message_push16(msg, totalAdditionalRRs);
    Message_push16(msg, totalAuthorityRRs);
    Message_push16(msg, totalAnswerRRs);
    Message_push16(msg, totalQuestions);

    Message_push16(msg, flagsAsInt(&dmesg->flags));
    Message_push16(msg, dmesg->id);
};

static uint8_t receiveB(struct Message* msg, struct DNSServer_pvt* ctx)
{
    struct DNSServer_Message* dmsg = parseMessage(msg, msg->allocator);
    
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct DNSServer_pvt* const ctx = Identity_cast((struct DNSServer_pvt*)iface->receiverContext);

    struct Jmp jmp = {.code=0};
    Jmp_try(jmp) {
        Except_setDefaultHandler(jmp.handler);
        uint8_t ret = receiveB(msg, ctx);
        Except_setDefaultHandler(NULL);
        return ret;
    } Jmp_catch {
        Log_debug(ctx->logger, "Failed to parse dns message [%s]", jmp.message);
    }
    Except_setDefaultHandler(NULL);
    return Error_NONE;
}


static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    struct DNSServer_pvt* ctx = Identity_cast((struct DNSServer_pvt*)iface);

    Assert_true(msg->length >= 4);

    uint16_t ethertype = ((uint16_t*) msg->bytes)[1];
    uint16_t afType_be = 0;
    if (ethertype == Ethernet_TYPE_IP6) {
        afType_be = ctx->afInet6_be;
    } else if (ethertype == Ethernet_TYPE_IP4) {
        afType_be = ctx->afInet_be;
    } else {
        Assert_always(!"Unsupported ethertype");
    }
    ((uint16_t*) msg->bytes)[0] = 0;
    ((uint16_t*) msg->bytes)[1] = afType_be;

    return Interface_sendMessage(ctx->wrapped, msg);
}

struct Interface* DNSServer_new(struct Interface* wrapped, struct Log* logger)
{
    struct DNSServer_pvt* context = Allocator_clone(wrapped->allocator, (&(struct DNSServer_pvt) {
        .wrapped = wrapped,
        .afInet6_be = Endian_hostToBigEndian16(Sockaddr_AF_INET6),
        .afInet_be = Endian_hostToBigEndian16(Sockaddr_AF_INET),
        .logger = logger
    }));
    Identity_set(context);

    InterfaceWrapper_wrap(wrapped, sendMessage, receiveMessage, &context->generic);

    return &context->generic;
}
