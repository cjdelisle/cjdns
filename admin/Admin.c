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
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "interface/addressable/AddrInterface.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/ArrayWriter.h"
#include "io/Writer.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/log/Log.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "util/platform/Sockaddr.h"

#define string_strstr
#define string_strcmp
#define string_strlen
#include "util/platform/libc/string.h"

#include <crypto_hash_sha256.h>

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
    uint32_t* k = (uint32_t*) *key;
    return k[ ((*key)->addrLen / 4)-1 ];
}

static inline int Map_LastMessageTimeByAddr_compare(struct Sockaddr** keyA, struct Sockaddr** keyB)
{
    return Bits_memcmp(*keyA, *keyB, (*keyA)->addrLen);
}

/////// end map

struct Function
{
    String* name;
    Admin_FUNCTION(call);
    void* context;
    bool needsAuth;
    Dict* args;
};

struct Admin
{
    struct EventBase* eventBase;

    struct Function* functions;
    int functionCount;

    struct Allocator* allocator;

    String* password;
    struct Log* logger;

    struct AddrInterface* iface;

    struct Map_LastMessageTimeByAddr map;

    /** non-zero if we are currently in an admin request. */
    int inRequest;

    /** non-zero if this session able to receive asynchronous messages. */
    int asyncEnabled;

    /** Length of addresses of clients which communicate with admin. */
    uint32_t addrLen;

    Identity
};

static uint8_t sendMessage(struct Message* message, struct Sockaddr* dest, struct Admin* admin)
{
    // stack overflow when used with admin logger.
    //Log_keys(admin->logger, "sending message to angel [%s]", message->bytes);
    Message_push(message, dest, dest->addrLen);
    return admin->iface->generic.sendMessage(message, &admin->iface->generic);
}

static int sendBenc(Dict* message, struct Sockaddr* dest, struct Admin* admin)
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
    return sendMessage(&m, dest, admin);
}

/**
 * If no incoming data has been sent by this address in TIMEOUT_MILLISECONDS
 * then Admin_sendMessage() should fail so that it doesn't endlessly send
 * udp packets into outer space after a logging client disconnects.
 */
static int checkAddress(struct Admin* admin, int index, uint64_t now)
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
    struct Admin* admin = Identity_cast((struct Admin*) vAdmin);
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
int Admin_sendMessage(Dict* message, String* txid, struct Admin* admin)
{
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
    if (!admin->inRequest) {
        struct Sockaddr* addrPtr = (struct Sockaddr*) &addr.addr;
        int index = Map_LastMessageTimeByAddr_indexForKey(&addrPtr, &admin->map);
        uint64_t now = Time_currentTimeMilliseconds(admin->eventBase);
        if (index < 0 || checkAddress(admin, index, now)) {
            return -1;
        }
    }

    struct Allocator* allocator;
    BufferAllocator_STACK(allocator, 256);

    // Bounce back the user-supplied txid.
    String userTxid = {
        .bytes = txid->bytes + admin->addrLen,
        .len = txid->len - admin->addrLen
    };
    if (txid->len > admin->addrLen) {
        Dict_putString(message, TXID, &userTxid, allocator);
    }

    return sendBenc(message, &addr.addr, admin);
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
    return Bits_memcmp(hashPtr, submittedHash->bytes, 64) == 0;
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

static void asyncEnabled(Dict* args, void* vAdmin, String* txid)
{
    struct Admin* admin = Identity_cast((struct Admin*) vAdmin);
    int64_t enabled = admin->asyncEnabled;
    Dict d = Dict_CONST(String_CONST("asyncEnabled"), Int_OBJ(enabled), NULL);
    Admin_sendMessage(&d, txid, admin);
}

#define ENTRIES_PER_PAGE 8
static void availableFunctions(Dict* args, void* vAdmin, String* txid)
{
    struct Admin* admin = Identity_cast((struct Admin*) vAdmin);
    int64_t* page = Dict_getInt(args, String_CONST("page"));
    uint32_t i = (page) ? *page * ENTRIES_PER_PAGE : 0;
    struct Allocator* tempAlloc = Allocator_child(admin->allocator);

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

    Admin_sendMessage(d, txid, admin);
    Allocator_free(tempAlloc);
    return;
}

static void handleRequest(Dict* messageDict,
                          struct Message* message,
                          struct Sockaddr* src,
                          struct Allocator* allocator,
                          struct Admin* admin)
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
            if (checkArgs(args, &admin->functions[i], txid, admin)) {
                admin->functions[i].call(args, admin->functions[i].context, txid);
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
        Admin_sendMessage(&d, txid, admin);
    }

    return;
}

static void handleMessage(struct Message* message,
                          struct Sockaddr* src,
                          struct Allocator* alloc,
                          struct Admin* admin)
{
    #ifdef Log_KEYS
        uint8_t lastChar = message->bytes[message->length - 1];
        message->bytes[message->length - 1] = '\0';
        Log_keys(admin->logger, "Got message from [%s] [%s]",
                 Sockaddr_print(src, alloc), message->bytes);
        message->bytes[message->length - 1] = lastChar;
    #endif

    // handle non empty message data
    if (message->length > Admin_MAX_REQUEST_SIZE) {
        #define TOO_BIG "d5:error16:Request too big.e"
        #define TOO_BIG_STRLEN (sizeof(TOO_BIG) - 1)
        Bits_memcpyConst(message->bytes, TOO_BIG, TOO_BIG_STRLEN);
        message->length = TOO_BIG_STRLEN;
        sendMessage(message, src, admin);
        return;
    }

    struct Reader* reader = ArrayReader_new(message->bytes, message->length, alloc);
    Dict messageDict;
    if (StandardBencSerializer_get()->parseDictionary(reader, alloc, &messageDict)) {
        Log_warn(admin->logger,
                 "Unparsable data from [%s] content: [%s]",
                 Sockaddr_print(src, alloc), message->bytes);
        return;
    }

    int amount = reader->bytesRead(reader);
    if (amount < message->length) {
        Log_warn(admin->logger,
                 "Message from [%s] contained garbage after byte [%d] content: [%s]",
                 Sockaddr_print(src, alloc), amount - 1, message->bytes);
        return;
    }

    handleRequest(&messageDict, message, src, alloc, admin);
}

static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    struct Admin* admin = Identity_cast((struct Admin*) iface->receiverContext);

    Assert_true(message->length >= (int)admin->addrLen);
    struct Sockaddr_storage addrStore = { .addr = { .addrLen = 0 } };
    Message_pop(message, &addrStore, admin->addrLen);

    struct Allocator* alloc = Allocator_child(admin->allocator);
    admin->inRequest = 1;

    handleMessage(message, &addrStore.addr, alloc, admin);

    admin->inRequest = 0;
    Allocator_free(alloc);
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
    Identity_check(admin);

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

struct Admin* Admin_new(struct AddrInterface* iface,
                        struct Allocator* alloc,
                        struct Log* logger,
                        struct EventBase* eventBase,
                        String* password)
{
    struct Admin* admin = Allocator_clone(alloc, (&(struct Admin) {
        .iface = iface,
        .allocator = alloc,
        .logger = logger,
        .eventBase = eventBase,
        .addrLen = iface->addr->addrLen,
        .map = {
            .allocator = alloc
        }
    }));
    Identity_set(admin);

    admin->password = String_clone(password, alloc);

    Timeout_setInterval(clearExpiredAddresses, admin, TIMEOUT_MILLISECONDS * 3, eventBase, alloc);

    iface->generic.receiveMessage = receiveMessage;
    iface->generic.receiverContext = admin;

    Admin_registerFunction("Admin_asyncEnabled", asyncEnabled, admin, false, NULL, admin);
    Admin_registerFunction("Admin_availableFunctions", availableFunctions, admin, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 0, .type = "Int" }
        }), admin);

    return admin;
}
