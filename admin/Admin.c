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
#include "admin/angel/Angel.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "crypto/Crypto.h"
#include "dht/CJDHTConstants.h"
#include "exception/ExceptionHandler.h"
#include "interface/Interface.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/ArrayWriter.h"
#include "io/Writer.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/log/Log.h"
#include "util/Security.h"
#include "util/Time.h"
#include "util/Timeout.h"

#include "util/Assert.h"
#include <crypto_hash_sha256.h>
#include <event2/event.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#ifdef WIN32
    #define EWOULDBLOCK WSAEWOULDBLOCK
#endif

static String* TYPE =     String_CONST_SO("type");
static String* REQUIRED = String_CONST_SO("required");
static String* STRING =   String_CONST_SO("String");
static String* INTEGER =  String_CONST_SO("Int");
static String* DICT =     String_CONST_SO("Dict");
static String* LIST =     String_CONST_SO("List");
static String* TXID =     String_CONST_SO("txid");

struct Function
{
    String* name;
    Admin_FUNCTION(call);
    void* context;
    bool needsAuth;
    Dict* args;
};

/**
 * This txid prefix is the inter-process communication txid.
 * the txid which is passed to the functions is these bytes followed by the bytes
 * that the user supplied in their "txid" entry. If the user didn't supply a txid
 * the txid which the function gets will be just these bytes and when it is sent back
 * the user will not get a txid back.
 */
union Admin_TxidPrefix {
    uint8_t raw[8];
    struct {
        uint32_t channelNum;
        uint32_t serial;
    } values;
};
#define Admin_TxidPrefix_SIZE 8
Assert_compileTime(sizeof(union Admin_TxidPrefix) == Admin_TxidPrefix_SIZE);

struct Channel
{
    /** True if the channel is waiting for the other end to close. */
    bool isClosing;

    /** The index of this channel in the admin->clientChannels array. */
    uint32_t number;

    uint32_t partialMessageLength;
    uint8_t partialMessageBuffer[Admin_MAX_REQUEST_SIZE];
    struct Allocator* alloc;
};

struct Admin
{
    struct event_base* eventBase;

    struct Function* functions;
    int functionCount;

    struct Allocator* allocator;

    String* password;
    struct Log* logger;

    struct Interface* toAngelInterface;

    struct Channel* clientChannels[Angel_MAX_CONNECTIONS];
};

static void freeChannel(void* vchannel)
{
    struct Channel** channel = vchannel;
    *channel = NULL;
}

static void newChannel(struct Channel** location, struct Allocator* alloc)
{
    struct Allocator* childAlloc = alloc->child(alloc);
    struct Channel* out = childAlloc->clone(sizeof(struct Channel), alloc, &(struct Channel) {
        .alloc = childAlloc
    });
    childAlloc->onFree(freeChannel, location, childAlloc);
    *location = out;
}

/**
 * find a channel by number; could allocate channels on the fly if needed
 * right now only 0..Angel_MAX_CONNECTIONS-1 numbers are valid and allocated in the Admin struct
 */
static struct Channel* channelForNum(uint32_t channelNum, bool create, struct Admin* admin)
{
    Assert_true(channelNum < Angel_MAX_CONNECTIONS);
    struct Channel** chanPtr = &admin->clientChannels[channelNum];
    if (*chanPtr == NULL && create) {
        newChannel(chanPtr, admin->allocator);
        (*chanPtr)->number = channelNum;
    }
    return *chanPtr;
}

static void sendMessage(struct Message* message, uint32_t channelNum, struct Admin* admin)
{
    // stack overflow when used with admin logger.
    //Log_keys(admin->logger, "sending message to angel [%s]", message->bytes);
    Message_shift(message, 4);
    Bits_memcpyConst(message->bytes, &channelNum, 4);
    admin->toAngelInterface->sendMessage(message, admin->toAngelInterface);
}

static int sendBenc(Dict* message, uint32_t channelNum, struct Admin* admin)
{
    struct Allocator* allocator;
    BufferAllocator_STACK(allocator, 256);

    #define SEND_MESSAGE_PADDING 32
    uint8_t buff[Admin_MAX_RESPONSE_SIZE + SEND_MESSAGE_PADDING];

    struct Writer* w = ArrayWriter_new(buff + SEND_MESSAGE_PADDING,
                                       Admin_MAX_RESPONSE_SIZE,
                                       allocator);
    StandardBencSerializer_get()->serializeDictionary(w, message);

    struct Message m = {
        .bytes = buff + SEND_MESSAGE_PADDING,
        .length = w->bytesWritten(w),
        .padding = SEND_MESSAGE_PADDING
    };
    sendMessage(&m, channelNum, admin);
    return 0;
}

int Admin_sendMessageToAngel(Dict* message, struct Admin* admin)
{
    return sendBenc(message, 0xFFFFFFFF, admin);
}

/**
 * public function to send responses
 */
int Admin_sendMessage(Dict* message, String* txid, struct Admin* admin)
{
    if (!admin) {
        return 0;
    }
    Assert_true(txid && txid->len >= 4);

    uint32_t channelNum;
    Bits_memcpyConst(&channelNum, txid->bytes, 4);
    struct Channel* channel = channelForNum(channelNum, false, admin);

    if (!channel) {
        return Admin_sendMessage_CHANNEL_CLOSED;
    }

    struct Allocator* allocator;
    BufferAllocator_STACK(allocator, 256);

    // Bounce back the user-supplied txid.
    String userTxid = {
        .bytes = txid->bytes + 4,
        .len = txid->len - 4
    };
    if (txid->len > 4) {
        Dict_putString(message, TXID, &userTxid, allocator);
    }

    return sendBenc(message, channelNum, admin);
}

/**
 * close a channel (for example if an error happened or we received non-empty
 * messages on a invalid channel number)
 * also used to cleanup if we receive a close message
 */
static void closeChannel(struct Channel* channel, struct Admin* admin)
{
    if (channel && !channel->isClosing) {
        channel->isClosing = true;
        uint8_t buff[128];
        struct Message m = { .bytes = &buff[124], .length = 0, .padding = 128 };
        sendMessage(&m, channel->number, admin);
        channel->isClosing = true;
    }
}

static inline bool authValid(Dict* message, struct Message* messageBytes, struct Admin* admin)
{
    String* cookieStr = Dict_getString(message, String_CONST("cookie"));
    uint32_t cookie = (cookieStr != NULL) ? strtoll(cookieStr->bytes, NULL, 10) : 0;
    if (!cookie) {
        int64_t* cookieInt = Dict_getInt(message, String_CONST("cookie"));
        cookie = (cookieInt) ? *cookieInt : 0;
    }
    uint64_t nowSecs = Time_currentTimeSeconds(admin->eventBase);
    String* submittedHash = Dict_getString(message, String_CONST("hash"));
    if (cookie >  nowSecs || cookie < nowSecs - 20 || !submittedHash || submittedHash->len != 64) {
        return false;
    }

    uint8_t* hashPtr = (uint8_t*) strstr((char*) messageBytes->bytes, submittedHash->bytes);

    if (!hashPtr || !admin->password) {
        return false;
    }

    uint8_t passAndCookie[64];
    snprintf((char*) passAndCookie, 64, "%s%u", admin->password->bytes, cookie);
    uint8_t hash[32];
    crypto_hash_sha256(hash, passAndCookie, strlen((char*) passAndCookie));
    Hex_encode(hashPtr, 64, hash, 32);

    crypto_hash_sha256(hash, messageBytes->bytes, messageBytes->length);
    Hex_encode(hashPtr, 64, hash, 32);
    return memcmp(hashPtr, submittedHash->bytes, 64) == 0;
}

static bool checkArgs(Dict* args, struct Function* func, String* txid, struct Admin* admin)
{
    struct Dict_Entry* entry = *func->args;
    String* error = NULL;
    uint8_t buffer[1024];
    struct Allocator* alloc = BufferAllocator_new(buffer, 1024);
    while (entry != NULL) {
        String* key = (String*) entry->key;
        Assert_true(entry->val->type == Object_DICT);
        Dict* value = entry->val->as.dictionary;
        entry = entry->next;
        if (*Dict_getInt(value, String_CONST("required")) == 0) {
            continue;
        }
        String* type = Dict_getString(value, String_CONST("type"));
        if ((type == STRING && !Dict_getString(args, key))
            || (type == DICT && !Dict_getDict(args, key))
            || (type == INTEGER && !Dict_getInt(args, key))
            || (type == LIST && !Dict_getList(args, key)))
        {
            error = String_printf(alloc,
                                  "Entry [%s] is required and must be of type [%s]",
                                  key->bytes,
                                  type->bytes);
            break;
        }
    }
    if (error) {
        Dict d = Dict_CONST(String_CONST("error"), String_OBJ(error), NULL);
        Admin_sendMessage(&d, txid, admin);
    }
    return !error;
}

static void handleRequest(Dict* messageDict,
                          struct Message* message,
                          struct Channel* channel,
                          struct Allocator* allocator,
                          struct Admin* admin)
{
    Log_debug(admin->logger, "Got a request on channel [%u]", channel->number);
    String* query = Dict_getString(messageDict, CJDHTConstants_QUERY);
    if (!query) {
        Log_info(admin->logger,
                 "Got a non-query from admin interface on channel [%u].",
                 channel->number);
        closeChannel(channel, admin);
        return;
    }

    // txid becomes the user supplied txid combined with the channel num.
    String* userTxid = Dict_getString(messageDict, TXID);
    uint32_t txidlen = ((userTxid) ? userTxid->len : 0) + 4;
    String* txid = String_newBinary(NULL, txidlen, allocator);
    Bits_memcpyConst(txid->bytes, &channel->number, 4);
    if (userTxid) {
        Bits_memcpy(txid->bytes + 4, userTxid->bytes, userTxid->len);
    }

    // If they're asking for a cookie then lets give them one.
    String* cookie = String_CONST("cookie");
    if (String_equals(query, cookie)) {
        Log_debug(admin->logger, "Got a request for a cookie");
        Dict* d = Dict_new(allocator);
        char bytes[32];
        snprintf(bytes, 32, "%u", (uint32_t) Time_currentTimeSeconds(admin->eventBase));
        String* theCookie = &(String) { .len = strlen(bytes), .bytes = bytes };
        Dict_putString(d, cookie, theCookie, allocator);
        Admin_sendMessage(d, txid, admin);
        return;
    }

    // If this is a permitted query, make sure the cookie is right.
    String* auth = String_CONST("auth");
    bool authed = false;
    if (String_equals(query, auth)) {
        if (!authValid(messageDict, message, admin)) {
            Dict* d = Dict_new(allocator);
            Dict_putString(d, String_CONST("error"), String_CONST("Auth failed."), allocator);
            Admin_sendMessage(d, txid, admin);
            return;
        }
        query = Dict_getString(messageDict, String_CONST("aq"));
        authed = true;
    }

    Dict* args = Dict_getDict(messageDict, String_CONST("args"));
    bool noFunctionsCalled = true;
    for (int i = 0; i < admin->functionCount; i++) {
        if (String_equals(query, admin->functions[i].name)
            && (authed || !admin->functions[i].needsAuth))
        {
            if (checkArgs(args, &admin->functions[i], txid, admin)) {
                admin->functions[i].call(args, admin->functions[i].context, txid);
            }
            noFunctionsCalled = false;
        }
    }

    if (noFunctionsCalled) {
        Dict* d = Dict_new(allocator);
        Dict_putString(d,
                       String_CONST("error"),
                       String_CONST("No functions matched your request."),
                       allocator);
        Dict* functions = Dict_new(allocator);
        for (int i = 0; i < admin->functionCount; i++) {
            Dict_putDict(functions, admin->functions[i].name, admin->functions[i].args, allocator);
        }
        if (functions) {
            Dict_putDict(d, String_CONST("availableFunctions"), functions, allocator);
        }
        Admin_sendMessage(d, txid, admin);
        return;
    }

    return;
}

static void handleMessage(struct Message* message,
                          struct Channel* channel,
                          struct Admin* admin)
{
    Log_debug(admin->logger, "Handling message.");
    if (channel->partialMessageLength > 0) {
        Log_debug(admin->logger, "Channel [%d] has [%u] bytes of existing data on it.",
                  channel->number, channel->partialMessageLength);
        Message_shift(message, channel->partialMessageLength);
        Bits_memcpy(message->bytes, channel->partialMessageBuffer, channel->partialMessageLength);
        channel->partialMessageLength = 0;
    }

    // Try to handle multiple stacked requests in the same frame.
    for (;;) {
        struct Allocator* allocator = admin->allocator->child(admin->allocator);
        struct Reader* reader = ArrayReader_new(message->bytes, message->length, allocator);

        char nextByte;
        int ret;
        while (!(ret = reader->read(&nextByte, 1, reader)) && nextByte != 'd');

        if (ret) {
            // out of data.
            channel->partialMessageLength = 0;
            return;
        }

        // back the reader up by 1.
        reader->skip(-1, reader);

        Dict messageDict;
        ret = StandardBencSerializer_get()->parseDictionary(reader, allocator, &messageDict);
        if (ret == -2) {
            // couldn't parse any more data.
            break;
        }
        if (ret) {
            Log_warn(admin->logger,
                     "Got unparsable data from admin interface on channel [%u] content: [%s].",
                     channel->number, message->bytes);
            closeChannel(channel, admin);
            return;
        }

        uint32_t amount = reader->bytesRead(reader);

        struct Message tmpMessage = { .bytes = message->bytes, .length = amount, .padding = 0 };
        handleRequest(&messageDict, &tmpMessage, channel, allocator, admin);

        Message_shift(message, -amount);

        allocator->free(allocator);
    }

    // This is an idea of just how much the incoming message can be shifted.
    // Pathological messages full of massive requests will fail.
    if (0 != message->length && message->length <= Admin_MAX_REQUEST_SIZE) {
        // move data to start of buffer, so we can append new data
        Bits_memcpy(channel->partialMessageBuffer, message->bytes, message->length);
        channel->partialMessageLength = message->length;
    }
}

static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    struct Admin* admin = iface->receiverContext;

    Assert_true(message->length >= 4);

    uint32_t channelNum;
    Bits_memcpyConst(&channelNum, message->bytes, 4);
    Message_shift(message, -4);

    Log_debug(admin->logger, "Got data from [%u]", channelNum);

    struct Channel* channel = channelForNum(channelNum, true, admin);

    if (0 == message->length) {
        // empty message -> close channel
        channel->alloc->free(channel->alloc);
        return 0;
    }

    // handle non empty message data
    if (message->length > Admin_MAX_REQUEST_SIZE) {
        #define TOO_BIG "d5:error16:Request too big.e"
        #define TOO_BIG_STRLEN (sizeof(TOO_BIG) - 1)
        Bits_memcpyConst(message->bytes, TOO_BIG, TOO_BIG_STRLEN);
        message->length = TOO_BIG_STRLEN;
        sendMessage(message, channelNum, admin);
    } else if (channel->isClosing) {
        // Do nothing because the channel is in closing state.
    } else {
        handleMessage(message, channel, admin);
    }
    return 0;
}

void Admin_registerFunctionWithArgCount(char* name,
                                        Admin_FUNCTION(callback),
                                        void* callbackContext,
                                        bool needsAuth,
                                        struct Admin_FunctionArg* arguments,
                                        int argCount,
                                        struct Admin* admin)
{
    if (!admin) {
        return;
    }
    String* str = String_new(name, admin->allocator);
    if (!admin->functionCount) {
        admin->functions = admin->allocator->malloc(sizeof(struct Function), admin->allocator);
    } else {
        admin->functions =
            admin->allocator->realloc(admin->functions,
                                      sizeof(struct Function) * (admin->functionCount + 1),
                                      admin->allocator);
    }
    struct Function* fu = &admin->functions[admin->functionCount];
    admin->functionCount++;

    fu->name = str;
    fu->call = callback;
    fu->context = callbackContext;
    fu->needsAuth = needsAuth;
    fu->args = Dict_new(admin->allocator);
    for (int i = 0; arguments && i < argCount; i++) {
        // "type" must be one of: [ "String", "Int", "Dict", "List" ]
        String* type = NULL;
        if (!strcmp(arguments[i].type, STRING->bytes)) {
            type = STRING;
        } else if (!strcmp(arguments[i].type, INTEGER->bytes)) {
            type = INTEGER;
        } else if (!strcmp(arguments[i].type, DICT->bytes)) {
            type = DICT;
        } else if (!strcmp(arguments[i].type, LIST->bytes)) {
            type = LIST;
        } else {
            abort();
        }
        Dict* arg = Dict_new(admin->allocator);
        Dict_putString(arg, TYPE, type, admin->allocator);
        Dict_putInt(arg, REQUIRED, arguments[i].required, admin->allocator);
        String* name = String_new(arguments[i].name, admin->allocator);
        Dict_putDict(fu->args, name, arg, admin->allocator);
    }
}

struct Admin* Admin_new(struct Interface* toAngelInterface,
                        struct Allocator* alloc,
                        struct Log* logger,
                        struct event_base* eventBase,
                        String* password)
{
    struct Admin* admin = alloc->clone(sizeof(struct Admin), alloc, &(struct Admin) {
        .toAngelInterface = toAngelInterface,
        .allocator = alloc,
        .logger = logger,
        .functionCount = 0,
        .eventBase = eventBase,
        .password = String_clone(password, alloc)
    });

    toAngelInterface->receiveMessage = receiveMessage;
    toAngelInterface->receiverContext = admin;

    return admin;
}
