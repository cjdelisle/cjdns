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
#include "benc/Int.h"
#include "benc/Dict.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/log/Log.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "util/platform/Sockaddr.h"
#include "util/Defined.h"

#include <crypto_hash_sha256.h>

#include <stdlib.h>
#include <stdio.h>

static String* TYPE =     String_CONST_SO("type");
static String* REQUIRED = String_CONST_SO("required");
static String* STRING =   String_CONST_SO("String");
static String* INTEGER =  String_CONST_SO("Int");
static String* DICT =     String_CONST_SO("Dict");
static String* LIST =     String_CONST_SO("List");
static String* TXID =     String_CONST_SO("txid");

/** Number of milliseconds before a session times out and outgoing messages are failed. */
#define TIMEOUT_MILLISECONDS 30000

/** map values for tracking time of last message by source address */
struct MapValue
{
    /** time when the last incoming message was received. */
    uint64_t timeOfLastMessage;

    /** used to allocate the memory for the key (Sockaddr) and value (this). */
    struct Allocator* allocator;
};

//////// generate time-of-last-message-by-address map

#define Map_USE_HASH
#define Map_USE_COMPARATOR
#define Map_NAME LastMessageTimeByAddr
#define Map_KEY_TYPE struct Sockaddr*
#define Map_VALUE_TYPE struct MapValue*
#include "util/Map.h"
static inline uint32_t Map_LastMessageTimeByAddr_hash(struct Sockaddr** key)
{
    return Sockaddr_hash(*key);
}
static inline int Map_LastMessageTimeByAddr_compare(struct Sockaddr** keyA, struct Sockaddr** keyB)
{
    return Sockaddr_compare(*keyA, *keyB);
}

/////// end map

struct Function
{
    String* name;
    Admin_Function call;
    void* context;
    bool needsAuth;
    Dict* args;
};

struct Admin_pvt
{
    struct Admin pub;

    struct Iface iface;

    struct EventBase* eventBase;

    struct Function* functions;
    int functionCount;

    struct Allocator* allocator;

    String* password;
    struct Log* logger;

    struct Map_LastMessageTimeByAddr map;

    /** non-null if we are currently in an admin request. */
    struct Message* currentRequest;

    /** non-zero if this session able to receive asynchronous messages. */
    int asyncEnabled;

    /** Length of addresses of clients which communicate with admin. */
    uint32_t addrLen;

    struct Message* tempSendMsg;

    Identity
};

static void sendMessage(struct Message* message, struct Sockaddr* dest, struct Admin_pvt* admin)
{
    // stack overflow when used with admin logger.
    //Log_keys(admin->logger, "sending message to angel [%s]", message->bytes);
    Message_push(message, dest, dest->addrLen, NULL);
    Iface_send(&admin->iface, message);
}

static void sendBenc(Dict* message,
                     struct Sockaddr* dest,
                     struct Allocator* alloc,
                     struct Admin_pvt* admin)
{
    Message_reset(admin->tempSendMsg);
    BencMessageWriter_write(message, admin->tempSendMsg, NULL);
    struct Message* msg = Message_new(0, admin->tempSendMsg->length + 32, alloc);
    Message_push(msg, admin->tempSendMsg->bytes, admin->tempSendMsg->length, NULL);
    sendMessage(msg, dest, admin);
}

/**
 * If no incoming data has been sent by this address in TIMEOUT_MILLISECONDS
 * then Admin_sendMessage() should fail so that it doesn't endlessly send
 * udp packets into outer space after a logging client disconnects.
 */
static int checkAddress(struct Admin_pvt* admin, int index, uint64_t now)
{
    uint64_t diff = now - admin->map.values[index]->timeOfLastMessage;
    // check for backwards time
    if (diff > TIMEOUT_MILLISECONDS && diff < ((uint64_t)INT64_MAX)) {
        Allocator_free(admin->map.values[index]->allocator);
        Map_LastMessageTimeByAddr_remove(index, &admin->map);
        return -1;
    }

    return 0;
}

static void clearExpiredAddresses(void* vAdmin)
{
    struct Admin_pvt* admin = Identity_check((struct Admin_pvt*) vAdmin);
    uint64_t now = Time_currentTimeMilliseconds(admin->eventBase);
    int count = 0;
    for (int i = admin->map.count - 1; i >= 0; i--) {
        if (checkAddress(admin, i, now)) {
            count++;
        }
    }
    Log_debug(admin->logger, "Cleared [%d] expired sessions", count);
}

/**
 * public function to send responses
 */
int Admin_sendMessage(Dict* message, String* txid, struct Admin* adminPub)
{
    struct Admin_pvt* admin = Identity_check((struct Admin_pvt*) adminPub);
    if (!admin) {
        return 0;
    }
    Identity_check(admin);
    Assert_true(txid && txid->len >= admin->addrLen);

    struct Sockaddr_storage addr;
    Bits_memcpy(&addr, txid->bytes, admin->addrLen);

    // if this is an async call, check if we've got any input from that client.
    // if the client is nresponsive then fail the call so logs don't get sent
    // out forever after a disconnection.
    if (!admin->currentRequest) {
        struct Sockaddr* addrPtr = (struct Sockaddr*) &addr.addr;
        int index = Map_LastMessageTimeByAddr_indexForKey(&addrPtr, &admin->map);
        uint64_t now = Time_currentTimeMilliseconds(admin->eventBase);
        if (index < 0 || checkAddress(admin, index, now)) {
            return Admin_sendMessage_CHANNEL_CLOSED;
        }
    }

    struct Allocator* alloc = Allocator_child(admin->allocator);

    // Bounce back the user-supplied txid.
    String* userTxid =
        String_newBinary(&txid->bytes[admin->addrLen], txid->len - admin->addrLen, alloc);
    if (txid->len > admin->addrLen) {
        Dict_putString(message, TXID, userTxid, alloc);
    }

    sendBenc(message, &addr.addr, alloc, admin);

    Dict_remove(message, TXID);

    Allocator_free(alloc);

    return 0;
}

static inline bool authValid(Dict* message, struct Message* messageBytes, struct Admin_pvt* admin)
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

    uint8_t* hashPtr = CString_strstr(messageBytes->bytes, submittedHash->bytes);

    if (!hashPtr || !admin->password) {
        return false;
    }

    uint8_t passAndCookie[64];
    snprintf((char*) passAndCookie, 64, "%s%u", admin->password->bytes, cookie);
    uint8_t hash[32];
    crypto_hash_sha256(hash, passAndCookie, CString_strlen((char*) passAndCookie));
    Hex_encode(hashPtr, 64, hash, 32);

    crypto_hash_sha256(hash, messageBytes->bytes, messageBytes->length);
    Hex_encode(hashPtr, 64, hash, 32);
    return Bits_memcmp(hashPtr, submittedHash->bytes, 64) == 0;
}

static bool checkArgs(Dict* args,
                      struct Function* func,
                      String* txid,
                      struct Allocator* requestAlloc,
                      struct Admin_pvt* admin)
{
    struct Dict_Entry* entry = *func->args;
    String* error = NULL;
    while (entry != NULL) {
        String* key = (String*) entry->key;
        Assert_ifParanoid(entry->val->type == Object_DICT);
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
            error = String_printf(requestAlloc,
                                  "Entry [%s] is required and must be of type [%s]",
                                  key->bytes,
                                  type->bytes);
            break;
        }
    }
    if (error) {
        Dict d = Dict_CONST(String_CONST("error"), String_OBJ(error), NULL);
        Admin_sendMessage(&d, txid, &admin->pub);
    }
    return !error;
}

static void asyncEnabled(Dict* args, void* vAdmin, String* txid, struct Allocator* requestAlloc)
{
    struct Admin_pvt* admin = Identity_check((struct Admin_pvt*) vAdmin);
    int64_t enabled = admin->asyncEnabled;
    Dict d = Dict_CONST(String_CONST("asyncEnabled"), Int_OBJ(enabled), NULL);
    Admin_sendMessage(&d, txid, &admin->pub);
}

#define ENTRIES_PER_PAGE 8
static void availableFunctions(Dict* args, void* vAdmin, String* txid, struct Allocator* tempAlloc)
{
    struct Admin_pvt* admin = Identity_check((struct Admin_pvt*) vAdmin);
    int64_t* page = Dict_getInt(args, String_CONST("page"));
    uint32_t i = (page) ? *page * ENTRIES_PER_PAGE : 0;

    Dict* d = Dict_new(tempAlloc);
    Dict* functions = Dict_new(tempAlloc);
    int count = 0;
    for (; i < (uint32_t)admin->functionCount && count++ < ENTRIES_PER_PAGE; i++) {
        Dict_putDict(functions, admin->functions[i].name, admin->functions[i].args, tempAlloc);
    }
    String* more = String_CONST("more");
    if (count >= ENTRIES_PER_PAGE) {
        Dict_putInt(d, more, 1, tempAlloc);
    }
    Dict_putDict(d, String_CONST("availableFunctions"), functions, tempAlloc);

    Admin_sendMessage(d, txid, &admin->pub);
}

static void handleRequest(Dict* messageDict,
                          struct Message* message,
                          struct Sockaddr* src,
                          struct Allocator* allocator,
                          struct Admin_pvt* admin)
{
    String* query = Dict_getString(messageDict, String_CONST("q"));
    if (!query) {
        Log_info(admin->logger, "Got a non-query from admin interface");
        return;
    }

    // txid becomes the user supplied txid combined with the channel num.
    String* userTxid = Dict_getString(messageDict, TXID);
    uint32_t txidlen = ((userTxid) ? userTxid->len : 0) + src->addrLen;
    String* txid = String_newBinary(NULL, txidlen, allocator);
    Bits_memcpy(txid->bytes, src, src->addrLen);
    if (userTxid) {
        Bits_memcpy(txid->bytes + src->addrLen, userTxid->bytes, userTxid->len);
    }

    // If they're asking for a cookie then lets give them one.
    String* cookie = String_CONST("cookie");
    if (String_equals(query, cookie)) {
        //Log_debug(admin->logger, "Got a request for a cookie");
        Dict* d = Dict_new(allocator);
        char bytes[32];
        snprintf(bytes, 32, "%u", (uint32_t) Time_currentTimeSeconds(admin->eventBase));
        String* theCookie = &(String) { .len = CString_strlen(bytes), .bytes = bytes };
        Dict_putString(d, cookie, theCookie, allocator);
        Admin_sendMessage(d, txid, &admin->pub);
        return;
    }

    // If this is a permitted query, make sure the cookie is right.
    String* auth = String_CONST("auth");
    bool authed = false;
    if (String_equals(query, auth)) {
        if (!authValid(messageDict, message, admin)) {
            Dict* d = Dict_new(allocator);
            Dict_putString(d, String_CONST("error"), String_CONST("Auth failed."), allocator);
            Admin_sendMessage(d, txid, &admin->pub);
            return;
        }
        query = Dict_getString(messageDict, String_CONST("aq"));
        authed = true;
    }

    // Then sent a valid authed query, lets track their address so they can receive
    // asynchronous messages.
    int index = Map_LastMessageTimeByAddr_indexForKey(&src, &admin->map);
    uint64_t now = Time_currentTimeMilliseconds(admin->eventBase);
    admin->asyncEnabled = 1;
    if (index >= 0) {
        admin->map.values[index]->timeOfLastMessage = now;
    } else if (authed) {
        struct Allocator* entryAlloc = Allocator_child(admin->allocator);
        struct MapValue* mv = Allocator_calloc(entryAlloc, sizeof(struct MapValue), 1);
        mv->timeOfLastMessage = now;
        mv->allocator = entryAlloc;
        struct Sockaddr* storedAddr = Sockaddr_clone(src, entryAlloc);
        Map_LastMessageTimeByAddr_put(&storedAddr, &mv, &admin->map);
    } else {
        admin->asyncEnabled = 0;
    }

    Dict* args = Dict_getDict(messageDict, String_CONST("args"));
    bool noFunctionsCalled = true;
    for (int i = 0; i < admin->functionCount; i++) {
        if (String_equals(query, admin->functions[i].name)
            && (authed || !admin->functions[i].needsAuth))
        {
            if (checkArgs(args, &admin->functions[i], txid, message->alloc, admin)) {
                admin->functions[i].call(args, admin->functions[i].context, txid, message->alloc);
            }
            noFunctionsCalled = false;
        }
    }

    if (noFunctionsCalled) {
        Dict d = Dict_CONST(
            String_CONST("error"),
            String_OBJ(String_CONST("No functions matched your request, "
                                    "try Admin_availableFunctions()")),
            NULL
        );
        Admin_sendMessage(&d, txid, &admin->pub);
    }

    return;
}

static void handleMessage(struct Message* message,
                          struct Sockaddr* src,
                          struct Allocator* alloc,
                          struct Admin_pvt* admin)
{
    if (Defined(Log_KEYS)) {
        uint8_t lastChar = message->bytes[message->length - 1];
        message->bytes[message->length - 1] = '\0';
        Log_keys(admin->logger, "Got message from [%s] [%s]",
                 Sockaddr_print(src, alloc), message->bytes);
        message->bytes[message->length - 1] = lastChar;
    }

    // handle non empty message data
    if (message->length > Admin_MAX_REQUEST_SIZE) {
        #define TOO_BIG "d5:error16:Request too big.e"
        #define TOO_BIG_STRLEN (sizeof(TOO_BIG) - 1)
        Bits_memcpy(message->bytes, TOO_BIG, TOO_BIG_STRLEN);
        message->length = TOO_BIG_STRLEN;
        sendMessage(message, src, admin);
        return;
    }

    int origMessageLen = message->length;
    Dict* messageDict = NULL;
    char* err = BencMessageReader_readNoExcept(message, alloc, &messageDict);
    if (err) {
        Log_warn(admin->logger,
                 "Unparsable data from [%s] content: [%s] error: [%s]",
                 Sockaddr_print(src, alloc), message->bytes, err);
        return;
    }

    if (message->length) {
        Log_warn(admin->logger,
                 "Message from [%s] contained garbage after byte [%d] content: [%s]",
                 Sockaddr_print(src, alloc), message->length, message->bytes);
        return;
    }

    // put the data back in the front of the message because it is used by the auth checker.
    Message_shift(message, origMessageLen, NULL);

    handleRequest(messageDict, message, src, alloc, admin);
}

static Iface_DEFUN receiveMessage(struct Message* message, struct Iface* iface)
{
    struct Admin_pvt* admin = Identity_containerOf(iface, struct Admin_pvt, iface);

    Assert_ifParanoid(message->length >= (int)admin->addrLen);
    struct Sockaddr_storage addrStore = { .addr = { .addrLen = 0 } };
    Message_pop(message, &addrStore, admin->addrLen, NULL);

    struct Allocator* alloc = Allocator_child(admin->allocator);
    admin->currentRequest = message;

    handleMessage(message, &addrStore.addr, alloc, admin);

    admin->currentRequest = NULL;
    Allocator_free(alloc);
    return NULL;
}

void Admin_registerFunctionWithArgCount(char* name,
                                        Admin_Function callback,
                                        void* callbackContext,
                                        bool needsAuth,
                                        struct Admin_FunctionArg* arguments,
                                        int argCount,
                                        struct Admin* adminPub)
{
    struct Admin_pvt* admin = Identity_check((struct Admin_pvt*) adminPub);

    String* str = String_new(name, admin->allocator);
    admin->functions =
        Allocator_realloc(admin->allocator,
                          admin->functions,
                          sizeof(struct Function) * (admin->functionCount + 1));
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
        if (!CString_strcmp(arguments[i].type, STRING->bytes)) {
            type = STRING;
        } else if (!CString_strcmp(arguments[i].type, INTEGER->bytes)) {
            type = INTEGER;
        } else if (!CString_strcmp(arguments[i].type, DICT->bytes)) {
            type = DICT;
        } else if (!CString_strcmp(arguments[i].type, LIST->bytes)) {
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

struct Admin* Admin_new(struct AddrIface* ai,
                        struct Log* logger,
                        struct EventBase* eventBase,
                        String* password)
{
    struct Allocator* alloc = ai->alloc;
    struct Admin_pvt* admin = Allocator_calloc(alloc, sizeof(struct Admin_pvt), 1);
    Identity_set(admin);
    admin->allocator = alloc;
    admin->logger = logger;
    admin->eventBase = eventBase;
    admin->addrLen = ai->addr->addrLen;
    admin->map.allocator = alloc;
    admin->iface.send = receiveMessage;
    Iface_plumb(&admin->iface, &ai->iface);
    admin->tempSendMsg = Message_new(0, Admin_MAX_RESPONSE_SIZE, alloc);

    admin->password = String_clone(password, alloc);

    Timeout_setInterval(clearExpiredAddresses, admin, TIMEOUT_MILLISECONDS * 3, eventBase, alloc);

    Admin_registerFunction("Admin_asyncEnabled", asyncEnabled, admin, false, NULL, &admin->pub);
    Admin_registerFunction("Admin_availableFunctions", availableFunctions, admin, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 0, .type = "Int" }
        }), &admin->pub);

    return &admin->pub;
}
