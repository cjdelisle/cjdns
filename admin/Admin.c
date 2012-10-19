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
#include "admin/angel/AngelChan.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "crypto/Crypto.h"
#include "dht/CJDHTConstants.h"
#include "exception/AbortHandler.h"
#include "exception/ExceptionHandler.h"
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
static String* INTEGER =      String_CONST_SO("Int");
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

/*       *Channels* between admin and core process
 *
 * Channels are identified by number; they are opened by a non empty message,
 * and closed by an empty message. A close on an open channel must be ACKed with
 * an empty message before it can be reopened.
 *
 * Channels are in one of three states: OPEN, CLOSED, WAIT_FOR_CLOSE
 *  - OPEN:
 *    + on empty message: send close, -> CLOSED
 *    + <close>: send close -> WAIT_FOR_CLOSE
 *    + <send data>: send the non-empty data
 *  - CLOSED (start state):
 *    + ignore empty messages
 *    + on non empty message: -> OPEN
 *    + <send data>: send the non-empty data -> OPEN
 *  - WAIT_FOR_CLOSE
 *    + ignore non empty messages (optionally send close again if this channel is invalid)
 *    + on empty message: -> CLOSED
 *    + CANNOT <send data> on this channel
 *
 * A channel is a byte stream; the message framing must not be used to identify
 * frames in the byte stream of a channel.
 * (Right now bencoding messages is the way to go)
 *
 * If one end doesn't want to handle a channel, it responds with an immediate close
 * on non empty messages, but it drops empty messages; such "invalid" channel are
 * never in the OPEN state.
 *
 * For valid channels (channels that are not closed immediately on open) you
 * have to keep track of the WAIT_FOR_CLOSE (and OPEN) state, but you can free all
 * data in the CLOSED state.
 *
 * By not ACKing a close message you force the other end to stay in the WAIT_FOR_CLOSE
 * state (and block reopening the channel).
 */

/* zero state ("default") is CLOSED */
enum Admin_ChannelState {
    Admin_ChannelState_OPEN = 1,
    Admin_ChannelState_CLOSED = 0,
    Admin_ChannelState_WAIT_FOR_CLOSE = 2
};


#pragma pack(4)
struct Admin_MessageHeader
{
    /**
     * some random magic known only to admin and core process to verify the framing
     * is still in sync.
     */
    uint64_t magic;

    /**
     * length of the message data following the header
     * zero length: close(d) communication channel
     */
    uint32_t length;

    /**
     * channel types:
     *   0x00000000 - 0x0000ffff: admin connections
     *      data exchanged: continous byte stream to/from the tcp connection
     *      byte stream represents requests/responses;
     *      each request/response is bencoded
     */
    uint32_t channelNum;
};
#define Admin_MessageHeader_SIZE 16
Assert_compileTime(sizeof(struct Admin_MessageHeader) == Admin_MessageHeader_SIZE);

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

struct Admin_Channel
{
    struct Allocator* allocator;
    uint32_t bufferLen;
    uint8_t* buffer;

    enum Admin_ChannelState state;

    /**
     * increase if channel gets closed, so old responses are not sent on a new channel
     * becomes part of the txid
     */
    uint32_t serial;
};

struct Admin
{
    struct event_base* eventBase;
    struct event* pipeEv;
    int inFd;
    int outFd;
    struct Function* functions;
    int functionCount;
    struct Allocator* allocator;
    struct sockaddr_storage address;
    int addressLength;
    uint64_t syncMagic;
    String* password;
    struct Log* logger;

    unsigned int haveMessageHeaderLen;
    struct Admin_MessageHeader messageHeader;

    struct Admin_Channel clientChannels[AngelChan_MAX_CONNECTIONS];

    /**
     * Becomes true after the admin process has sent it's first message.
     * In cjdroute2 this is unused.
     */
    bool initialized;
};

// close pipes to admin process
static void adminClosePipes(struct Admin* admin)
{
    EVUTIL_CLOSESOCKET(admin->inFd);
    admin->inFd = -1;
    EVUTIL_CLOSESOCKET(admin->outFd);
    admin->outFd = -1;
    event_free(admin->pipeEv);
}

/**
 * find a channel by number; could allocate channels on the fly if needed
 * right now only 0..AngelChan_MAX_CONNECTIONS-1 numbers are valid and allocated in the Admin struct
 */
static struct Admin_Channel* adminChannelFindById(struct Admin* admin, uint32_t channelNum)
{
    if (channelNum < AngelChan_MAX_CONNECTIONS) {
        return &admin->clientChannels[channelNum];
    }

    return NULL;
}

/**
 * find a open channel by TXID from a response; also verifies the serial still matches,
 * so it drops old responses for a channel which got reset in the meantime
 * (the serial wraps around after 2^32 channel resets, which should be safe enough)
 */
static struct Admin_Channel* adminChannelFindByTxid(struct Admin* admin,
                                                String* txid,
                                                uint32_t* pChannelNum)
{
    if (!admin || !txid || txid->len < Admin_TxidPrefix_SIZE) {
        return NULL;
    }

    union Admin_TxidPrefix prefix;
    Bits_memcpyConst(prefix.raw, txid->bytes, Admin_TxidPrefix_SIZE);

    struct Admin_Channel* channel = adminChannelFindById(admin, prefix.values.channelNum);
    if (!channel
        || Admin_ChannelState_OPEN != channel->state
        || channel->serial != prefix.values.serial) {
        // invalid channel or not open or invalid serial
        return NULL;
    }

    if (pChannelNum) {
        *pChannelNum = prefix.values.channelNum;
    }

    return channel;
}

/**
 * send a message
 */
static void adminChannelSendData(struct Admin* admin,
                                 uint32_t channelNum,
                                 const void *data,
                                 uint32_t length)
{
    /* if this changes, we need to fragment the messages
    * into AngelChan_MAX_MESSAGE_SIZE chunks
    */
    Assert_compileTime(AngelChan_MAX_API_REQUEST_SIZE == AngelChan_MAX_MESSAGE_SIZE);

    Assert_true(length <= AngelChan_MAX_MESSAGE_SIZE);

    struct Admin_MessageHeader header = {
        .magic = admin->syncMagic,
        .length = length,
        .channelNum = channelNum
    };
    const uint8_t* buf = (const uint8_t*) data;

    // TODO: check result, buffer writes
    write(admin->outFd, &header, Admin_MessageHeader_SIZE);
    if (length > 0) {
        write(admin->outFd, buf, length);
    }
}

/**
 * public function to send responses
 */
int Admin_sendMessage(Dict* message, String* txid, struct Admin* admin)
{
    if (!admin) {
        return 0;
    }
    Assert_true(txid);

    uint32_t channelNum;
    struct Admin_Channel* channel = adminChannelFindByTxid(admin, txid, &channelNum);

    if (!channel) {
        return Admin_sendMessage_CHANNEL_CLOSED;
    }

    uint8_t buff[AngelChan_MAX_API_REQUEST_SIZE];

    uint8_t allocBuff[256];
    struct Allocator* allocator = BufferAllocator_new(allocBuff, 256);

    // Bounce back the user-supplied txid.
    String userTxid = {
        .bytes = txid->bytes + Admin_TxidPrefix_SIZE,
        .len = txid->len - Admin_TxidPrefix_SIZE
    };
    if (txid->len > Admin_TxidPrefix_SIZE) {
        Dict_putString(message, TXID, &userTxid, allocator);
    }

    struct Writer* w = ArrayWriter_new(buff, AngelChan_MAX_API_REQUEST_SIZE, allocator);
    StandardBencSerializer_get()->serializeDictionary(w, message);

    adminChannelSendData(admin, channelNum, buff, w->bytesWritten(w));

    return 0;
}

/**
 * close a channel (for example if an error happened or we received non-empty
 * messages on a invalid channel number)
 * also used to cleanup if we receive a close message
 */
static void adminChannelClose(struct Admin* admin, uint32_t channelNum)
{
    struct Admin_Channel* channel = adminChannelFindById(admin, channelNum);

    if (channel) {
        switch (channel->state) {
        case Admin_ChannelState_OPEN:
            break;
        case Admin_ChannelState_CLOSED:
        case Admin_ChannelState_WAIT_FOR_CLOSE:
            return; // already sent close, nothing to do
        }
        channel->state = Admin_ChannelState_WAIT_FOR_CLOSE;

        // clean up bufers
        channel->bufferLen = 0;
        channel->buffer = NULL;
        if (channel->allocator) {
            channel->allocator->free(channel->allocator);
            channel->allocator = NULL;
        }
    }
    adminChannelSendData(admin, channelNum, NULL, 0);
}

/**
 * handle a received channel close (the received header is in admin->messageHeader)
 * as invalid channels are never OPEN we never have to ACK a close on them
 */
static void adminChannelHandleClose(struct Admin* admin)
{
    uint32_t channelNum = admin->messageHeader.channelNum;
    struct Admin_Channel* channel = adminChannelFindById(admin, channelNum);

    if (channel) {
        switch (channel->state) {
        case Admin_ChannelState_OPEN:
            // close active channel
            adminChannelClose(admin, channelNum);
            // now the state is WAIT_FOR_CLOSE, set it to CLOSED
            channel->state = Admin_ChannelState_CLOSED;
            break;
        case Admin_ChannelState_WAIT_FOR_CLOSE:
            channel->state = Admin_ChannelState_CLOSED;
            channel->serial++;
            break;
        case Admin_ChannelState_CLOSED:
            // nothing to do
            break;
        }
    }
}

static inline bool authValid(Dict* message, uint8_t* buffer, uint32_t length, struct Admin* admin)
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

    uint8_t* hashPtr = (uint8_t*) strstr((char*) buffer, submittedHash->bytes);

    if (!hashPtr || !admin->password) {
        return false;
    }

    uint8_t passAndCookie[64];
    snprintf((char*) passAndCookie, 64, "%s%u", admin->password->bytes, cookie);
    uint8_t hash[32];
    crypto_hash_sha256(hash, passAndCookie, strlen((char*) passAndCookie));
    Hex_encode(hashPtr, 64, hash, 32);

    crypto_hash_sha256(hash, buffer, length);
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

static void handleRequestFromChild(struct Admin* admin,
                                   union Admin_TxidPrefix* txid_prefix,
                                   Dict* message,
                                   uint8_t* buffer,
                                   size_t amount,
                                   struct Allocator* allocator)
{
    String* query = Dict_getString(message, CJDHTConstants_QUERY);
    if (!query) {
        Log_info(admin->logger,
                 "Got a non-query from admin interface on channel [%u].",
                 admin->messageHeader.channelNum);
        adminChannelClose(admin, admin->messageHeader.channelNum);
        return;
    }

    // txid becomes the user supplied txid combined with the inter-process txid.
    String* userTxid = Dict_getString(message, TXID);
    uint32_t txidlen = ((userTxid) ? userTxid->len : 0) + Admin_TxidPrefix_SIZE;
    String* txid = String_newBinary(NULL, txidlen, allocator);
    Bits_memcpyConst(txid->bytes, txid_prefix->raw, Admin_TxidPrefix_SIZE);
    if (userTxid) {
        Bits_memcpy(txid->bytes + Admin_TxidPrefix_SIZE, userTxid->bytes, userTxid->len);
    }

    // If they're asking for a cookie then lets give them one.
    String* cookie = String_CONST("cookie");
    if (String_equals(query, cookie)) {
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
        if (!authValid(message, buffer, amount, admin)) {
            Dict* d = Dict_new(allocator);
            Dict_putString(d, String_CONST("error"), String_CONST("Auth failed."), allocator);
            Admin_sendMessage(d, txid, admin);
            return;
        }
        query = Dict_getString(message, String_CONST("aq"));
        authed = true;
    }

    Dict* args = Dict_getDict(message, String_CONST("args"));
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

// This is unused in cjdroute2, it only exists for legacy cjdroute.
static void inFromChildInitialize(struct Admin* admin)
{
    uint8_t buffer[sizeof(struct sockaddr_storage) + sizeof(int) + 8];
    ssize_t amount = read(admin->inFd, buffer, sizeof(buffer));

    if (amount != sizeof(buffer)) {
        Log_error(admin->logger, "unexpected length");
        adminClosePipes(admin);
    } else if (memcmp(buffer, "abcd", 4)) {
        Log_error(admin->logger, "bad magic");
        adminClosePipes(admin);
    } else if (memcmp(&buffer[sizeof(struct sockaddr_storage) + sizeof(int) + 4], "wxyz", 4)) {
        Log_error(admin->logger, "bad magic");
        adminClosePipes(admin);
    } else {
        Bits_memcpyConst(&admin->addressLength, &buffer[4], sizeof(int));
        Bits_memcpyConst(&admin->address,
                        &buffer[4 + sizeof(int)],
                        sizeof(struct sockaddr_storage));
        admin->initialized = true;
    }
    event_base_loopbreak(admin->eventBase);
}

static void inFromChildDecode(struct Admin* admin, struct Admin_Channel* channel)
{
    uint8_t* buffer = channel->buffer;
    uint32_t bufferLen = channel->bufferLen;

    union Admin_TxidPrefix prefix = {
        .values = {
            .channelNum = admin->messageHeader.channelNum,
            .serial = channel->serial
        }
    };

    while (bufferLen > 0) {
        /* skip newlines and spaces between requests */
        while (bufferLen > 0 && ('\r' == *buffer || '\n' == *buffer || ' ' == *buffer)) {
            buffer++;
            bufferLen--;
        }
        if (0 == bufferLen) {
            break;
        }

        struct Allocator* allocator = admin->allocator->child(admin->allocator);

        struct Reader* reader = ArrayReader_new(buffer, bufferLen, allocator);
        Dict message;

        int res = StandardBencSerializer_get()->parseDictionary(reader, allocator, &message);
        if (-2 == res) {
            Log_debug(admin->logger,
                      "Need more data to decode bencoded request on channel [%u].",
                      admin->messageHeader.channelNum);
            break; // need more data
        }
        if (res) {
            Log_info(admin->logger,
                     "Got unparsable data from admin interface on channel [%u].",
                     admin->messageHeader.channelNum);
            adminChannelClose(admin, admin->messageHeader.channelNum);
            return; // buffer is already cleared
        }

        uint32_t amount = reader->bytesRead(reader);

        handleRequestFromChild(admin, &prefix, &message, buffer, amount, allocator);
        buffer += amount;
        bufferLen -= amount;

        allocator->free(allocator);
    }

    if (0 != bufferLen && buffer != channel->buffer) {
        // move data to start of buffer, so we can append new data
        memmove(channel->buffer, buffer, bufferLen);
    }
    channel->bufferLen = bufferLen;
}

/**
 * Doesn't read more than maxRead bytes into buf[*have..bufSize]
 * Increments *have by amount of read bytes
 *
 * Buffer mustn't be full, and maxRead > 0
 *
 * return values:
 * n>0: read n new bytes
 *  -1: eof/network or read error - closed
 *  -2: EAGAIN
 */
static int inFromChildFillBuffer(struct Admin* admin,
                                 void *buf,
                                 uint32_t bufSize,
                                 uint32_t* have,
                                 uint32_t maxRead)
{
    char *charBuf = (char*) buf;
    Assert_true(*have < bufSize);
    Assert_true(maxRead > 0);

    if (maxRead > bufSize - *have) {
        maxRead = bufSize - *have;
    }

    Assert_always(maxRead > 0);
    Assert_always(maxRead <= INT_MAX);

    errno = 0;
    ssize_t amount = read(admin->inFd, charBuf + *have, maxRead);

    if (amount < 0 && (EAGAIN == errno || EWOULDBLOCK == errno)) {
        return -2;
    } else if (0 == amount) {
        Log_error(admin->logger, "Connection to admin process closed unexpectedly");
        adminClosePipes(admin);
        return -1;
    } else if (amount < 0) {
        Log_error(admin->logger, "Broken pipe to admin process, [%s]", strerror(errno));
        adminClosePipes(admin);
        return -1;
    }

    Assert_always(amount > 0);
    Assert_true(amount <= (ssize_t) maxRead);

    *have += amount;
    Assert_true(*have <= bufSize);

    return amount;
}

// only in parent
static void inFromChildRead(struct Admin* admin, struct Admin_Channel* channel)
{
    Assert_true(admin->messageHeader.length > 0);

    if (!channel->buffer) {
        Assert_true(NULL == channel->allocator);
        channel->allocator = admin->allocator->child(admin->allocator);
        channel->buffer =
            channel->allocator->malloc(AngelChan_MAX_API_REQUEST_SIZE, channel->allocator);
        Assert_true(0 == channel->bufferLen);
    }

    Assert_true(channel->bufferLen < AngelChan_MAX_API_REQUEST_SIZE);

    int amount = inFromChildFillBuffer(admin, channel->buffer, AngelChan_MAX_API_REQUEST_SIZE,
                                       &channel->bufferLen, admin->messageHeader.length);
    if (amount < 0) {
        return;
    }

    admin->messageHeader.length -= amount;
    if (0 == admin->messageHeader.length) {
        admin->haveMessageHeaderLen = 0;
    }

    inFromChildDecode(admin, channel);

    if (AngelChan_MAX_API_REQUEST_SIZE == channel->bufferLen) {
        // couldn't decode the request, but the buffer is full
        Log_error(admin->logger, "Request too large, closing channel [%u]",
                  admin->messageHeader.channelNum);
        adminChannelClose(admin, admin->messageHeader.channelNum);
    }

    if (0 == channel->bufferLen && channel->allocator) {
        // reached end of buffer, free it
        channel->allocator->free(channel->allocator);
        channel->allocator = NULL;
        channel->buffer = NULL;
    }
}

// only in parent
static void inFromChildSkipMmsg(struct Admin* admin)
{
    Assert_true(admin->messageHeader.length > 0);
    uint8_t buffer[AngelChan_MAX_MESSAGE_SIZE];
    uint32_t have = 0;

    int amount = inFromChildFillBuffer(admin, buffer, AngelChan_MAX_MESSAGE_SIZE,
                                       &have, admin->messageHeader.length);
    if (amount < 0) {
        return;
    }

    admin->messageHeader.length -= amount;
    if (0 == admin->messageHeader.length) {
        admin->haveMessageHeaderLen = 0;
    }
}

// only in parent
static void inFromChild(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct Admin* admin = (struct Admin*) vcontext;

    if (!admin->initialized) {
        inFromChildInitialize(admin);
        return;
    }

    int newMessage = 0;

    if (admin->haveMessageHeaderLen < Admin_MessageHeader_SIZE) {
        int amount = inFromChildFillBuffer(admin, &admin->messageHeader, Admin_MessageHeader_SIZE,
                                           &admin->haveMessageHeaderLen, Admin_MessageHeader_SIZE);
        if (amount < 0) {
            return;
        }

        if (admin->haveMessageHeaderLen == Admin_MessageHeader_SIZE) {
            newMessage = 1;
            // got complete header
            if (admin->syncMagic != admin->messageHeader.magic) {
                Log_error(admin->logger, "wrong magic from admin process");
                adminClosePipes(admin);
                return;
            }
            if (admin->messageHeader.length > AngelChan_MAX_MESSAGE_SIZE) {
                Log_error(admin->logger, "message from admin process too large");
                adminClosePipes(admin);
                return;
            }
            if (0 == admin->messageHeader.length) {
                // empty message -> close channel
                adminChannelHandleClose(admin);
                admin->haveMessageHeaderLen = 0;
                return;
            }
        }
    }

    if (admin->haveMessageHeaderLen == Admin_MessageHeader_SIZE) {
        // handle non empty message data
        struct Admin_Channel* channel =
            adminChannelFindById(admin, admin->messageHeader.channelNum);
        if (!channel) {
            if (newMessage) {
                Log_info(admin->logger,
                         "message from admin process on invalid channel [%u]",
                         admin->messageHeader.channelNum);
                // send close
                adminChannelSendData(admin, admin->messageHeader.channelNum, NULL, 0);
            }
            /* ignore data */
            inFromChildSkipMmsg(admin);
        } else {
            switch (channel->state) {
            case Admin_ChannelState_CLOSED:
                channel->state = Admin_ChannelState_OPEN;
                inFromChildRead(admin, channel);
                break;
            case Admin_ChannelState_OPEN:
                inFromChildRead(admin, channel);
                break;
            case Admin_ChannelState_WAIT_FOR_CLOSE:
                /* ignore data */
                inFromChildSkipMmsg(admin);
                break;
            }
        }
    }
}

struct ChildContext;

struct Connection {
    struct event* read; /** NULL: socket closed */
    evutil_socket_t socket; /** -1: channel (to core) closed */
    struct ChildContext* context;
};

struct ChildContext
{
    unsigned int haveMessageHeaderLen, haveMessageLen;
    struct Admin_MessageHeader messageHeader;
    uint8_t message[AngelChan_MAX_MESSAGE_SIZE];

    struct Connection connections[AngelChan_MAX_CONNECTIONS];

    uint64_t syncMagic;

    /** The event which listens for new connections. */
    struct event* socketEvent;

    /** For talking with the parent process. */
    struct event* dataFromParent;
    int inFd;
    int outFd;

    struct event_base* eventBase;
    struct Allocator* allocator;
};

// Only happens in Admin process.
/**
 * send message via pipe to core process
 */
static void sendParent(struct ChildContext* context, uint32_t channelNum, uint32_t len, void* data)
{
    struct Admin_MessageHeader messageHeader = {
        .magic = context->syncMagic,
        .length = len,
        .channelNum = channelNum
    };

    // TODO: buffer writes

    size_t amountWritten;
    amountWritten = write(context->outFd, &messageHeader, Admin_MessageHeader_SIZE);
    if (amountWritten != Admin_MessageHeader_SIZE) {
        printf("Admin process failed to write data across pipe to main process.");
        exit(0);
    }

    if (len > 0) {
        amountWritten = write(context->outFd, data, len);
        if (amountWritten != len) {
            printf("Admin process failed to write data across pipe to main process.");
            exit(0);
        }
    }
}

// Only happens in Admin process.
/**
 * handle message on the pipe from core process
 */
static void incomingFromParent(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct ChildContext* context = (struct ChildContext*) vcontext;
    errno = 0;

    if (context->haveMessageHeaderLen < Admin_MessageHeader_SIZE) {
        ssize_t amount =
            read(context->inFd,
                 context->haveMessageHeaderLen + (char*)&context->messageHeader,
                 Admin_MessageHeader_SIZE - context->haveMessageHeaderLen);
        if (amount < 1) {
            if (EAGAIN == errno || EWOULDBLOCK == errno) {
                return;
            }
            if (amount < 0) {
                perror("broken pipe");
            } else {
                fprintf(stderr, "admin connection closed\n");
            }
            exit(0);
        }
        context->haveMessageHeaderLen += amount;
        if (context->haveMessageHeaderLen == Admin_MessageHeader_SIZE) {
            // got complete header, reset message
            context->haveMessageLen = 0;
            if (context->syncMagic != context->messageHeader.magic) {
                fprintf(stderr, "wrong magic on admin connection\n");
                exit(0);
            }
            if (context->messageHeader.length > AngelChan_MAX_MESSAGE_SIZE) {
                fprintf(stderr, "message too large on admin connection\n");
                exit(0);
            }
        }
    }

    if (context->haveMessageHeaderLen == Admin_MessageHeader_SIZE) {
        if (context->haveMessageLen < context->messageHeader.length) {
            ssize_t amount = read(context->inFd,
                                  context->message + context->haveMessageLen,
                                  context->messageHeader.length - context->haveMessageLen);
            if (amount < 1) {
                if (EAGAIN == errno || EWOULDBLOCK == errno) {
                    return;
                }
                if (amount < 0) {
                    perror("broken pipe");
                } else {
                    fprintf(stderr, "admin connection closed\n");
                }
                exit(0);
            }
            context->haveMessageLen += amount;
        }
        if (context->haveMessageLen == context->messageHeader.length) {
            // got complete message, reset header. new header will reset message later
            context->haveMessageHeaderLen = 0;

            if (context->messageHeader.channelNum <= 0xffff) {
                // message for admin connections
                uint32_t connNumber = context->messageHeader.channelNum;
                if (connNumber >= AngelChan_MAX_CONNECTIONS) {
                    fprintf(stderr, "got message for connection #%u, max connections is %d\n",
                            connNumber, AngelChan_MAX_CONNECTIONS);
                    return;
                }

                struct Connection* conn = &context->connections[connNumber];
                if (-1 == conn->socket) {
                    fprintf(stderr, "got message for closed channel #%u", connNumber);
                    return;
                }

                if (0 == context->haveMessageLen) {
                    /* close channel / recv ACK for close */
                    if (NULL != conn->read) {
                        // send close ACK
                        sendParent(context, connNumber, 0, NULL);
                        EVUTIL_CLOSESOCKET(conn->socket);
                        event_free(conn->read);
                        conn->read = NULL;
                    }
                    conn->socket = -1;
                } else if (NULL == conn->read) {
                    /* drop message - channel is closed, wait for close ACK */
                } else {
                    ssize_t sent;
                    sent = send(conn->socket, context->message, context->haveMessageLen, 0);
                    if (sent != (ssize_t) context->haveMessageLen) {
                        // All errors lead to closing the socket.
                        EVUTIL_CLOSESOCKET(conn->socket);
                        event_free(conn->read);
                        conn->read = NULL;
                        // send close channel
                        sendParent(context, connNumber, 0, NULL);
                        // set conn->socket = -1 later when we recv close ACK
                    }
                }
            }
        }
    }
}

// Only happens in Admin process.
/**
 * handle incoming tcp data from client connections in the admin process
 */
static void incomingFromClient(evutil_socket_t socket, short eventType, void* vconn)
{
    struct Connection* conn = (struct Connection*) vconn;
    struct ChildContext* context = conn->context;
    uint8_t buf[AngelChan_MAX_MESSAGE_SIZE];
    uint32_t connNumber = conn - context->connections;

    errno = 0;
    ssize_t result = recv(socket, buf, sizeof(buf), 0);

    if (result > 0) {
        sendParent(context, connNumber, result, buf);
    } else if (result < 0 && (EAGAIN  == errno || EWOULDBLOCK == errno)) {
        return;
    } else {
        // The return value will be 0 when the peer has performed an orderly shutdown.
        EVUTIL_CLOSESOCKET(conn->socket);
        event_free(conn->read);
        conn->read = NULL;
        // send close channel
        sendParent(context, connNumber, 0, NULL);
        // set conn->socket = -1 later when we recv close ACK
    }
}

static struct Connection* newConnection(struct ChildContext* context, evutil_socket_t fd)
{
    struct Connection* conn = NULL;
    for (int i = 0; i < AngelChan_MAX_CONNECTIONS; i++) {
        if (context->connections[i].read == NULL && context->connections[i].socket == -1) {
            conn = &context->connections[i];
            break;
        }
    }

    if (!conn) {
        return NULL;
    }

    conn->read = event_new(context->eventBase, fd, EV_READ | EV_PERSIST, incomingFromClient, conn);
    conn->socket = fd;
    conn->context = context;

    if (!conn->read) {
        return NULL;
    }

    event_add(conn->read, NULL);
    return conn;
}

// Happens only in the admin process.
static void acceptConn(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct ChildContext* context = (struct ChildContext*) vcontext;

    struct sockaddr_storage ss;
    ev_socklen_t slen = sizeof(ss);
    evutil_socket_t fd = accept(socket, (struct sockaddr*)&ss, &slen);
    if (fd < 0) {
        perror("acceptConn() fd < 0");
        return;
    } else if (fd > (evutil_socket_t) FD_SETSIZE) {
        EVUTIL_CLOSESOCKET(fd);
        return;
    }

    evutil_make_socket_nonblocking(fd);

    struct Connection* conn = newConnection(context, fd);
    if (!conn) {
        EVUTIL_CLOSESOCKET(fd);
    }
}

// only in child
static void child(struct sockaddr_storage* addr,
                  int addrLen,
                  char* user,
                  struct ChildContext* context)
{
    for (int i = 0; i < AngelChan_MAX_CONNECTIONS; i++) {
        context->connections[i].socket = -1;
    }

    context->dataFromParent =
        event_new(context->eventBase,
                  context->inFd,
                  EV_READ | EV_PERSIST,
                  incomingFromParent,
                  context);

    event_add(context->dataFromParent, NULL);

    if (!addr->ss_family) {
        addr->ss_family = AF_INET;
        // Apple gets mad if the length is wrong.
        addrLen = sizeof(struct sockaddr_in);
    }

    evutil_socket_t listener = socket(addr->ss_family, SOCK_STREAM, 0);

    if (listener < 0) {
        perror("socket()");
    }

    evutil_make_listen_socket_reuseable(listener);

    if (bind(listener, (struct sockaddr*) addr, addrLen) < 0) {
        perror("bind()");
        return;
    }

    if (getsockname(listener, (struct sockaddr*) addr, (ev_socklen_t*) &addrLen)) {
        perror("getsockname()");
    }

    if (listen(listener, 16)<0) {
        perror("listen");
        return;
    }

    evutil_make_socket_nonblocking(listener);

    context->socketEvent =
        event_new(context->eventBase, listener, EV_READ | EV_PERSIST, acceptConn, context);
    event_add(context->socketEvent, NULL);

    if (!context->eventBase) {
        exit(-1);
    }

    // Write back the sockaddr_storage struct so the other end knows our port.
    uint8_t buff[sizeof(struct sockaddr_storage) + sizeof(int) + 8];
    Bits_memcpyConst(buff, "abcd", 4);
    Bits_memcpyConst(&buff[4], &addrLen, sizeof(int));
    Bits_memcpyConst(&buff[4 + sizeof(int)], addr, sizeof(struct sockaddr_storage));
    Bits_memcpyConst(&buff[4 + sizeof(int) + sizeof(struct sockaddr_storage)], "wxyz", 4);
    write(context->outFd, buff, sizeof(buff));

    if (user) {
        Security_setUser(user, NULL, AbortHandler_INSTANCE);
    }

    event_base_dispatch(context->eventBase);
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

struct Admin* Admin_new(int fromAngelFd,
                        int toAngelFd,
                        struct Allocator* alloc,
                        struct Log* logger,
                        struct event_base* eventBase,
                        String* password,
                        uint8_t syncMagic[8])
{
    struct Admin* admin = alloc->calloc(sizeof(struct Admin), 1, alloc);
    admin->inFd = fromAngelFd;
    admin->outFd = toAngelFd;
    admin->allocator = alloc;
    admin->logger = logger;
    admin->functionCount = 0;
    admin->eventBase = eventBase;
    admin->password = password;
    admin->pipeEv = event_new(eventBase, fromAngelFd, EV_READ | EV_PERSIST, inFromChild, admin);
    admin->initialized = true;
    event_add(admin->pipeEv, NULL);
    Bits_memcpyConst(&admin->syncMagic, syncMagic, 8);
    return admin;
}

struct Admin* Admin_newProc(struct sockaddr_storage* addr,
                            int addrLen,
                            String* password,
                            char* user,
                            struct event_base* eventBase,
                            struct ExceptionHandler* eh,
                            struct Log* logger,
                            struct Allocator* allocator)
{
    if (!password) {
        uint8_t buff[32];
        Crypto_randomBase32(buff, 32);
        password = String_new((char*)buff, allocator);
    }

    uint64_t syncMagic;
    randombytes((uint8_t*) &syncMagic, 8);

    errno = 0;
    int pipes[2][2];
    if (pipe(pipes[0]) || pipe(pipes[1])) {
        eh->exception(__FILE__ " Failed to create pipes.", errno, eh);
    }

    int pgid = getpid();
    int pid = fork();
    if (pid < 0) {
        eh->exception(__FILE__ " Failed to fork()", errno, eh);
    }

    bool isChild = (pid == 0);

    int inFd = pipes[isChild][0];
    close(pipes[!isChild][0]);

    int outFd = pipes[!isChild][1];
    close(pipes[isChild][1]);

    if (isChild) {
        // the child is the admin process now
        // TODO: make parent the admin/angel

        // Set the process group so that children will not
        // become orphaned if the parent gets signal 11 err um 9.
        setpgid(0, pgid);

        struct ChildContext context;
        memset(&context, 0, sizeof(struct ChildContext));
        context.inFd = inFd;
        context.outFd = outFd;
        context.allocator = allocator;
        event_reinit(eventBase);
        context.eventBase = eventBase;
        context.syncMagic = syncMagic;
        child(addr, addrLen, user, &context);
        fprintf(stderr, "Admin process exiting.");
        exit(0);
    }

    setpgid(pid, pgid);

    struct Admin* admin = allocator->calloc(sizeof(struct Admin), 1, allocator);
    admin->inFd = inFd;
    admin->outFd = outFd;
    admin->allocator = allocator;
    admin->logger = logger;
    admin->functionCount = 0;
    admin->eventBase = eventBase;
    admin->password = password;
    Bits_memcpyConst(&admin->address, addr, sizeof(struct sockaddr_storage));
    admin->addressLength = addrLen;
    admin->pipeEv = event_new(eventBase, inFd, EV_READ | EV_PERSIST, inFromChild, admin);
    event_add(admin->pipeEv, NULL);
    admin->syncMagic = syncMagic;

    event_base_dispatch(eventBase);

    return admin;
}

void Admin_getConnectInfo(struct sockaddr_storage** addrPtr,
                          int* addrLenPtr,
                          String** passwordPtr,
                          struct Admin* admin)
{
    if (addrPtr) {
        *addrPtr = &admin->address;
    }
    if (addrLenPtr) {
        *addrLenPtr = admin->addressLength;
    }
    if (passwordPtr) {
        *passwordPtr = admin->password;
    }
}
