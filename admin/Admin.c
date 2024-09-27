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
#include "benc/Int.h"
#include "benc/Dict.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/log/Log.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "util/platform/Sockaddr.h"
#include "util/Defined.h"

#include <sodium/crypto_verify_32.h>

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

    EventBase_t* eventBase;

    struct Function* functions;
    int functionCount;

    struct Allocator* allocator;

    String* password;
    struct Log* logger;

    struct Map_LastMessageTimeByAddr map;

    /** non-null if we are currently in an admin request. */
    Message_t* currentRequest;

    /** non-zero if this session able to receive asynchronous messages. */
    int asyncEnabled;

    Message_t* tempSendMsg;

    Identity
};

static Err_DEFUN sendMessage(
    Message_t* message, struct Sockaddr* dest, struct Admin_pvt* admin)
{
    // stack overflow when used with admin logger.
    //Log_keys(admin->logger, "sending message to angel [%s]", message->bytes);
    Err(Message_epush(message, dest, dest->addrLen));
    return Iface_send(&admin->iface, message);
}

static Err_DEFUN sendBenc(Dict* message,
                               struct Sockaddr* dest,
                               struct Allocator* alloc,
                               struct Admin_pvt* admin,
                               int fd)
{
    Message_reset(admin->tempSendMsg);
    Err(BencMessageWriter_write(message, admin->tempSendMsg));
    Message_t* msg = Message_new(0, Message_getLength(admin->tempSendMsg) + 32, alloc);
    Err(Message_epush(msg, Message_bytes(admin->tempSendMsg), Message_getLength(admin->tempSendMsg)));
    Message_setAssociatedFd(msg, fd);
    return sendMessage(msg, dest, admin);
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
    uint64_t now = Time_currentTimeMilliseconds();
    int count = 0;
    for (int i = admin->map.count - 1; i >= 0; i--) {
        if (checkAddress(admin, i, now)) {
            count++;
        }
    }
    Log_debug(admin->logger, "Cleared [%d] expired sessions", count);
}

static int sendMessage0(Dict* message, String* txid, struct Admin* adminPub, int fd)
{
    struct Admin_pvt* admin = Identity_check((struct Admin_pvt*) adminPub);
    if (!admin) {
        return 0;
    }
    Assert_true(txid && txid->len >= sizeof(struct Sockaddr));
    uint16_t addrLen = 0;
    Bits_memcpy(&addrLen, txid->bytes, 2);
    Assert_true(txid->len >= addrLen);

    struct Allocator* alloc = NULL;
    if (admin->currentRequest) {
        alloc = Message_getAlloc(admin->currentRequest);
    } else {
        alloc = Allocator_child(admin->allocator);
    }
    struct Sockaddr* addr = Sockaddr_clone((struct Sockaddr*) txid->bytes, alloc);

    // if this is an async call, check if we've got any input from that client.
    // if the client is nresponsive then fail the call so logs don't get sent
    // out forever after a disconnection.
    if (!admin->currentRequest) {
        int index = Map_LastMessageTimeByAddr_indexForKey(&addr, &admin->map);
        uint64_t now = Time_currentTimeMilliseconds();
        if (index < 0 || checkAddress(admin, index, now)) {
            Allocator_free(alloc);
            return Admin_sendMessage_CHANNEL_CLOSED;
        }
    }

    // Bounce back the user-supplied txid.
    if (txid->len > addr->addrLen) {
        String* userTxid =
            String_newBinary(&txid->bytes[addr->addrLen], txid->len - addr->addrLen, alloc);
        Dict_putString(message, TXID, userTxid, alloc);
    }

    RTypes_Error_t* err = sendBenc(message, addr, alloc, admin, fd);
    if (err) {
        Log_warn(admin->logger, "Error sending benc: %s", Rffi_printError(err, alloc));
    }

    Dict_remove(message, TXID);

    if (!admin->currentRequest) {
        Allocator_free(alloc);
    }

    return 0;
}

int Admin_sendMessage(Dict* message, String* txid, struct Admin* adminPub)
{
    return sendMessage0(message, txid, adminPub, -1);
}

static inline bool authValid(Dict* message, Message_t* messageBytes, struct Admin_pvt* admin)
{
    String* cookieStr = Dict_getStringC(message, "cookie");
    uint32_t cookie = (cookieStr != NULL) ? strtoll(cookieStr->bytes, NULL, 10) : 0;
    if (!cookie) {
        int64_t* cookieInt = Dict_getIntC(message, "cookie");
        cookie = (cookieInt) ? *cookieInt : 0;
    }
    uint64_t nowSecs = Time_currentTimeSeconds();
    String* submittedHash = Dict_getStringC(message, "hash");
    if (cookie >  nowSecs || cookie < nowSecs - 20 || !submittedHash || submittedHash->len != 64) {
        return false;
    }

    uint8_t* hashPtr = CString_strstr(Message_bytes(messageBytes), submittedHash->bytes);

    if (!hashPtr || !admin->password) {
        return false;
    }

    uint8_t passAndCookie[64];
    snprintf((char*) passAndCookie, 64, "%s%u", admin->password->bytes, cookie);
    uint8_t hash[32];
    Rffi_crypto_hash_sha256(hash, passAndCookie, CString_strlen((char*) passAndCookie));
    Hex_encode(hashPtr, 64, hash, 32);

    Rffi_crypto_hash_sha256(hash, Message_bytes(messageBytes), Message_getLength(messageBytes));
    Hex_encode(hashPtr, 64, hash, 32);
    int res = crypto_verify_32(hashPtr, submittedHash->bytes);
    res |= crypto_verify_32(hashPtr + 32, submittedHash->bytes + 32);
    return res == 0;
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
        if (*Dict_getIntC(value, "required") == 0) {
            continue;
        }
        String* type = Dict_getStringC(value, "type");
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
    int64_t* page = Dict_getIntC(args, "page");
    uint32_t i = (page) ? *page * ENTRIES_PER_PAGE : 0;

    Dict* d = Dict_new(tempAlloc);
    Dict* functions = Dict_new(tempAlloc);
    int count = 0;
    for (; i < (uint32_t)admin->functionCount && count++ < ENTRIES_PER_PAGE; i++) {
        Dict_putDict(functions, admin->functions[i].name, admin->functions[i].args, tempAlloc);
    }
    if (count >= ENTRIES_PER_PAGE) {
        Dict_putIntC(d, "more", 1, tempAlloc);
    }
    Dict_putDictC(d, "availableFunctions", functions, tempAlloc);

    Admin_sendMessage(d, txid, &admin->pub);
}

static Err_DEFUN handleRequest(Dict* messageDict,
                          Message_t* message,
                          struct Sockaddr* src,
                          struct Allocator* allocator,
                          struct Admin_pvt* admin)
{
    String* query = Dict_getStringC(messageDict, "q");
    if (!query) {
        Log_info(admin->logger, "Got a non-query from admin interface");
        return NULL;
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
        snprintf(bytes, 32, "%u", (uint32_t) Time_currentTimeSeconds());
        String* theCookie = &(String) { .len = CString_strlen(bytes), .bytes = bytes };
        Dict_putString(d, cookie, theCookie, allocator);
        Admin_sendMessage(d, txid, &admin->pub);
        return NULL;
    }

    // If this is a permitted query, make sure the cookie is right.
    String* auth = String_CONST("auth");
    bool authed = false;
    if (String_equals(query, auth)) {
        if (!authValid(messageDict, message, admin)) {
            Dict* d = Dict_new(allocator);
            Dict_putStringCC(d, "error", "Auth failed.", allocator);
            Admin_sendMessage(d, txid, &admin->pub);
            return NULL;
        }
        query = Dict_getStringC(messageDict, "aq");
        authed = true;
    }

    if (String_equals(admin->password, String_CONST("NONE"))) {
        // If there's no password then we'll consider everything to be authed
        authed = true;
    }

    // Then sent a valid authed query, lets track their address so they can receive
    // asynchronous messages.
    int index = Map_LastMessageTimeByAddr_indexForKey(&src, &admin->map);
    uint64_t now = Time_currentTimeMilliseconds();
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

    Dict* args = Dict_getDictC(messageDict, "args");
    bool noFunctionsCalled = true;
    for (int i = 0; i < admin->functionCount; i++) {
        if (String_equals(query, admin->functions[i].name)
            && (authed || !admin->functions[i].needsAuth))
        {
            if (checkArgs(args, &admin->functions[i], txid, Message_getAlloc(message), admin)) {
                admin->functions[i].call(args, admin->functions[i].context, txid, Message_getAlloc(message));
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

    return NULL;
}

static Err_DEFUN handleMessage(Message_t* message,
                          struct Sockaddr* src,
                          struct Allocator* alloc,
                          struct Admin_pvt* admin)
{
    if (Defined(Log_KEYS)) {
        uint8_t lastChar = Message_bytes(message)[Message_getLength(message) - 1];
        Message_bytes(message)[Message_getLength(message) - 1] = '\0';
        Log_keys(admin->logger, "Got message from [%s] [%s]",
                 Sockaddr_print(src, alloc), Message_bytes(message));
        Message_bytes(message)[Message_getLength(message) - 1] = lastChar;
    }

    // handle non empty message data
    if (Message_getLength(message) > Admin_MAX_REQUEST_SIZE) {
        #define TOO_BIG "d5:error16:Request too big.e"
        #define TOO_BIG_STRLEN (sizeof(TOO_BIG) - 1)
        Bits_memcpy(Message_bytes(message), TOO_BIG, TOO_BIG_STRLEN);
        Err(Message_truncate(message, TOO_BIG_STRLEN));
        return sendMessage(message, src, admin);
    }

    int origMessageLen = Message_getLength(message);
    Dict* messageDict = NULL;
    const char* err = BencMessageReader_readNoExcept(message, alloc, &messageDict);
    if (err) {
        Log_warn(admin->logger,
                 "Unparsable data from [%s] content: [%s] error: [%s]",
                 Sockaddr_print(src, alloc),
                 Hex_print(Message_bytes(message), Message_getLength(message), alloc),
                 err);
        return NULL;
    }

    if (Message_getLength(message)) {
        Log_warn(admin->logger,
                 "Message from [%s] contained garbage after byte [%d] content: [%s]",
                 Sockaddr_print(src, alloc), Message_getLength(message), Message_bytes(message));
        return NULL;
    }

    // put the data back in the front of the message because it is used by the auth checker.
    Err(Message_eshift(message, origMessageLen));

    return handleRequest(messageDict, message, src, alloc, admin);
}

static Iface_DEFUN receiveMessage(Message_t* message, struct Iface* iface)
{
    struct Admin_pvt* admin = Identity_containerOf(iface, struct Admin_pvt, iface);
    struct Allocator* alloc = Allocator_child(admin->allocator);
    struct Sockaddr_storage addrStore;
    Err(AddrIface_popAddr(&addrStore, message));

    admin->currentRequest = message;
    RTypes_Error_t* err = handleMessage(message, Sockaddr_clone(&addrStore.addr, alloc), alloc, admin);
    admin->currentRequest = NULL;
    Allocator_free(alloc);
    return err;
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

static void importFd(Dict* args, void* vAdmin, String* txid, struct Allocator* requestAlloc)
{
    struct Admin_pvt* admin = Identity_check((struct Admin_pvt*) vAdmin);
    int fd = Message_getAssociatedFd(admin->currentRequest);
    Dict* res = Dict_new(requestAlloc);
    char* error = "none";
    if (fd < 0) {
        if (Defined(win32)) {
            error = "Admin_importFd() does not support win32";
        } else {
            error = "file descriptor was not attached to message";
        }
    } else {
        Dict_putIntC(res, "fd", fd, requestAlloc);
    }
    Dict_putStringCC(res, "error", error, requestAlloc);
    Admin_sendMessage(res, txid, &admin->pub);
}

static void exportFd(Dict* args, void* vAdmin, String* txid, struct Allocator* requestAlloc)
{
    struct Admin_pvt* admin = Identity_check((struct Admin_pvt*) vAdmin);
    int64_t* fd_p = Dict_getIntC(args, "fd");
    if (!fd_p || *fd_p < 0) {
        Dict* res = Dict_new(requestAlloc);
        Dict_putStringCC(res, "error", "invalid fd", requestAlloc);
        Admin_sendMessage(res, txid, &admin->pub);
        return;
    }
    int fd = *fd_p;
    Dict* res = Dict_new(requestAlloc);
    char* error = "none";
    if (fd < 0) {
        if (Defined(win32)) {
            error = "Admin_exportFd() does not support win32";
        } else {
            error = "file descriptor was not attached to message";
        }
    }
    Dict_putStringCC(res, "error", error, requestAlloc);
    sendMessage0(res, txid, &admin->pub, fd);
}

struct Admin* Admin_new(AddrIface_t* ai,
                        struct Log* logger,
                        EventBase_t* eventBase,
                        String* password)
{
    struct Allocator* alloc = ai->alloc;
    struct Admin_pvt* admin = Allocator_calloc(alloc, sizeof(struct Admin_pvt), 1);
    Identity_set(admin);
    admin->allocator = alloc;
    admin->logger = logger;
    admin->eventBase = eventBase;
    admin->map.allocator = alloc;
    admin->iface.send = receiveMessage;
    Iface_plumb(&admin->iface, ai->iface);
    admin->tempSendMsg = Message_new(0, Admin_MAX_RESPONSE_SIZE, alloc);

    admin->password = String_clone(password, alloc);

    Timeout_setInterval(clearExpiredAddresses, admin, TIMEOUT_MILLISECONDS * 3, eventBase, alloc);

    Admin_registerFunction("Admin_asyncEnabled", asyncEnabled, admin, false, NULL, &admin->pub);
    Admin_registerFunction("Admin_availableFunctions", availableFunctions, admin, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 0, .type = "Int" }
        }), &admin->pub);
    Admin_registerFunction("Admin_importFd", importFd, admin, true, NULL, &admin->pub);
    Admin_registerFunction("Admin_exportFd", exportFd, admin, true,
        ((struct Admin_FunctionArg[]) {
        { .name = "fd", .required = 1, .type = "Int" }
    }), &admin->pub);

    return &admin->pub;
}
