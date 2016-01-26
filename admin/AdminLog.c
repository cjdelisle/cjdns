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
#include <stdarg.h> // for String_vprintf()

#include "admin/Admin.h"
#include "admin/AdminLog.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/String.h"
#include "crypto/random/Random.h"
#include "io/Writer.h"
#include "memory/BufferAllocator.h"
#include "util/log/Log.h"
#include "util/log/Log_impl.h"
#include "util/Hex.h"
#include "util/Identity.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"

#define MAX_SUBSCRIPTIONS 64
#define FILE_NAME_COUNT 32

struct Subscription
{
    /** The log level to match against, all higher levels will also be matched. */
    enum Log_Level logLevel;

    /** The line number within the file or 0 to match all lines. */
    int lineNum;

    /** The name of the file to match against or null to match any file. */
    const char* file;

    /** True if file can be compared with pointer comparison instead of strcmp. */
    bool internalFile;

    /**
     * Dropped messages because they are being sent too fast for UDP interface to handle.
     * Reset when the pipes unclog an a message is sent reporting the number of dropped messages.
     */
    int dropped;

    /** The transaction ID of the message which solicited this stream of logs. */
    String* txid;

    /** A hopefully unique (random) number identifying this stream. */
    String* streamId;

    /** An allocator which will live during the lifecycle of the Subscription */
    struct Allocator* alloc;
};

struct AdminLog
{
    struct Log pub;
    struct Subscription subscriptions[MAX_SUBSCRIPTIONS];
    int subscriptionCount;

    /** non-zero if we are logging at this very moment (reentrent logging is not allowed!) */
    int logging;

    struct Timeout* unpause;

    struct Admin* admin;
    struct Allocator* alloc;
    struct Random* rand;
    struct EventBase* base;
    Identity
};

static inline bool isMatch(struct Subscription* subscription,
                           struct AdminLog* logger,
                           enum Log_Level logLevel,
                           const char* file,
                           int line)
{
    if (subscription->file) {
        if (subscription->file == file) {
            // fall through
        } else if (!subscription->internalFile && !CString_strcmp(file, subscription->file)) {
            // It's the same name but so we'll swap the name for the internal name and then
            // it can be compared quickly with a pointer comparison.
            subscription->file = file;
            subscription->internalFile = true;
        } else {
            return false;
        }
    }

    if (logLevel < subscription->logLevel) {
        return false;
    }
    if (subscription->lineNum && line != subscription->lineNum) {
        return false;
    }
    return true;
}

static String* STREAM_ID = String_CONST_SO("streamId");
static String* TIME =      String_CONST_SO("time");
static String* LEVEL =     String_CONST_SO("level");
static String* STR_FILE =  String_CONST_SO("file");
static String* LINE =      String_CONST_SO("line");
static String* MESSAGE =   String_CONST_SO("message");

static Dict* makeLogMessage(struct Subscription* subscription,
                            struct AdminLog* logger,
                            enum Log_Level logLevel,
                            const char* file,
                            uint32_t line,
                            String* message,
                            struct Allocator* alloc)
{
    int64_t now = (int64_t) Time_currentTimeSeconds(logger->base);

    Dict* out = Dict_new(alloc);

    Dict_putString(out, STREAM_ID, subscription->streamId, alloc);
    Dict_putInt(out, TIME, now, alloc);
    Dict_putString(out, LEVEL, String_new(Log_nameForLevel(logLevel), alloc), alloc);
    Dict_putString(out, STR_FILE, String_new(file, alloc), alloc);
    Dict_putInt(out, LINE, line, alloc);
    Dict_putString(out, MESSAGE, message, alloc);

    return out;
}

static void removeSubscription(struct AdminLog* log, struct Subscription* sub)
{
    Allocator_free(sub->alloc);
    log->subscriptionCount--;
    if (log->subscriptionCount == 0 || sub == &log->subscriptions[log->subscriptionCount]) {
        return;
    }
    Bits_memcpy(sub,
                &log->subscriptions[log->subscriptionCount],
                sizeof(struct Subscription));
}

static void unpause(void* vAdminLog)
{
    struct AdminLog* log = Identity_check((struct AdminLog*) vAdminLog);
    // dirty reentrence.
    Assert_true(!log->logging);
    bool noneDropped = true;
    for (int i = log->subscriptionCount - 1; i >= 0; i--) {
        int dropped = log->subscriptions[i].dropped;
        if (!dropped) { continue; }
        noneDropped = false;
        log->subscriptions[i].dropped = 0;
        Log_warn((struct Log*) log,
                 "UDPInterface cannot handle the logging, [%d] messages dropped", dropped);
        if (log->subscriptions[i].dropped) {
            // oh well, we'll try again later.
            log->subscriptions[i].dropped += dropped;
        }
    }
    if (noneDropped && false) {
        Timeout_clearTimeout(log->unpause);
        log->unpause = NULL;
    }
}

static void doLog(struct Log* genericLog,
                  enum Log_Level logLevel,
                  const char* fullFilePath,
                  int line,
                  const char* format,
                  va_list args)
{
    struct AdminLog* log = Identity_check((struct AdminLog*) genericLog);
    String* message = NULL;
    struct Allocator* logLineAlloc = NULL;

    if (log->logging) { return; }
    log->logging++;

    for (int i = log->subscriptionCount - 1; i >= 0; i--) {
        if (!isMatch(&log->subscriptions[i], log, logLevel, fullFilePath, line)) { continue; }
        if (log->subscriptions[i].dropped) {
            log->subscriptions[i].dropped++;
            continue;
        }
        if (!message) {
            logLineAlloc = Allocator_child(log->alloc);
            message = String_vprintf(logLineAlloc, format, args);
            // Strip all of the annoying \n marks in the log entries.
            if (message->len > 0 && message->bytes[message->len - 1] == '\n') {
                message->len--;
            }
        }
        Dict* d = makeLogMessage(&log->subscriptions[i],
                                 log,
                                 logLevel,
                                 fullFilePath,
                                 line,
                                 message,
                                 logLineAlloc);
        int ret = Admin_sendMessage(d, log->subscriptions[i].txid, log->admin);
        if (ret == Admin_sendMessage_CHANNEL_CLOSED) {
            removeSubscription(log, &log->subscriptions[i]);
        } else if (ret) {
            log->subscriptions[i].dropped++;
            if (!log->unpause) {
                log->unpause = Timeout_setInterval(unpause, log, 10, log->base, log->alloc);
            }
        }
    }
    if (logLineAlloc) {
        Allocator_free(logLineAlloc);
    }

    Assert_true(!--log->logging);
}

static void subscribe(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct AdminLog* log = Identity_check((struct AdminLog*) vcontext);
    String* levelName = Dict_getString(args, String_CONST("level"));
    enum Log_Level level = (levelName) ? Log_levelForName(levelName->bytes) : Log_Level_DEBUG;
    int64_t* lineNumPtr = Dict_getInt(args, String_CONST("line"));
    String* fileStr = Dict_getString(args, String_CONST("file"));
    if (fileStr && !fileStr->len) { fileStr = NULL; }
    char* error = "2+2=5";
    if (level == Log_Level_INVALID) {
        level = Log_Level_KEYS;
    }
    if (lineNumPtr && *lineNumPtr < 0) {
        error = "Invalid line number, must be positive or 0 to signify any line is acceptable.";
    } else if (log->subscriptionCount >= MAX_SUBSCRIPTIONS) {
        error = "Max subscription count reached.";
    } else {
        struct Subscription* sub = &log->subscriptions[log->subscriptionCount];
        sub->logLevel = level;
        sub->alloc = Allocator_child(log->alloc);
        String* fileStrCpy = String_clone(fileStr, sub->alloc);
        sub->file = (fileStrCpy) ? fileStrCpy->bytes : NULL;
        sub->lineNum = (lineNumPtr) ? *lineNumPtr : 0;
        sub->txid = String_clone(txid, sub->alloc);
        uint8_t streamId[8];
        Random_bytes(log->rand, streamId, 8);
        uint8_t streamIdHex[20];
        Hex_encode(streamIdHex, 20, streamId, 8);
        sub->streamId = String_new(streamIdHex, sub->alloc);
        Dict response = Dict_CONST(
            String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
            String_CONST("streamId"), String_OBJ(sub->streamId), NULL
        ));
        Admin_sendMessage(&response, txid, log->admin);
        log->subscriptionCount++;
        return;
    }

    Dict response = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST(error)), NULL
    );
    Admin_sendMessage(&response, txid, log->admin);
}

static void unsubscribe(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct AdminLog* log = Identity_check((struct AdminLog*) vcontext);
    String* streamIdHex = Dict_getString(args, String_CONST("streamId"));
    uint8_t streamId[8];
    char* error = NULL;
    if (streamIdHex->len != 16 || Hex_decode(streamId, 8, (uint8_t*)streamIdHex->bytes, 16) != 8) {
        error = "Invalid streamId.";
    } else {
        error = "No such subscription.";
        for (int i = 0; i < (int)log->subscriptionCount; i++) {
            if (String_equals(streamIdHex, log->subscriptions[i].streamId)) {
                removeSubscription(log, &log->subscriptions[i]);
                error = "none";
                break;
            }
        }
    }

    Dict response = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST(error)), NULL
    );
    Admin_sendMessage(&response, txid, log->admin);
}

static void logMany(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct AdminLog* log = Identity_check((struct AdminLog*) vcontext);
    int64_t* countPtr = Dict_getInt(args, String_CONST("count"));
    uint32_t count = *countPtr;
    for (uint32_t i = 0; i < count; i++) {
        Log_debug((struct Log*)log, "This is message number [%d] of total [%d]", i, count);
    }

    Dict response = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), NULL
    );
    Admin_sendMessage(&response, txid, log->admin);
}

static void subscriptions(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct AdminLog* log = Identity_check((struct AdminLog*) vcontext);
    Dict* msg = Dict_new(alloc);
    List* entries = List_new(alloc);
    Dict_putList(msg, String_CONST("entries"), entries, alloc);
    for (int i = 0; i < (int)log->subscriptionCount; i++) {
        Dict* entry = Dict_new(alloc);
        struct Subscription* sub = &log->subscriptions[i];
        Dict_putString(entry, LEVEL, String_new(Log_nameForLevel(sub->logLevel), alloc), alloc);
        if (sub->file) {
            Dict_putString(entry, STR_FILE, String_new(sub->file, alloc), alloc);
        }
        Dict_putInt(entry, LINE, sub->lineNum, alloc);
        Dict_putInt(entry, String_CONST("dropped"), sub->dropped, alloc);
        Dict_putInt(entry, String_CONST("internalFile"), sub->internalFile, alloc);
        Dict_putString(entry, String_CONST("streamId"), sub->streamId, alloc);
        List_addDict(entries, entry, alloc);
    }
    Admin_sendMessage(msg, txid, log->admin);
}

struct Log* AdminLog_registerNew(struct Admin* admin,
                                 struct Allocator* alloc,
                                 struct Random* rand,
                                 struct EventBase* base)
{
    struct AdminLog* log = Allocator_clone(alloc, (&(struct AdminLog) {
        .pub = {
            .print = doLog
        },
        .admin = admin,
        .alloc = alloc,
        .rand = rand,
        .base = base
    }));
    Identity_set(log);

    Admin_registerFunction("AdminLog_subscribe", subscribe, log, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "level", .required = 0, .type = "String" },
            { .name = "line", .required = 0, .type = "Int" },
            { .name = "file", .required = 0, .type = "String" }
        }), admin);

    Admin_registerFunction("AdminLog_unsubscribe", unsubscribe, log, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "streamId", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("AdminLog_subscriptions", subscriptions, log, true, NULL, admin);

    Admin_registerFunction("AdminLog_logMany", logMany, log, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "count", .required = 1, .type = "Int" }
        }), admin);

    return &log->pub;
}
