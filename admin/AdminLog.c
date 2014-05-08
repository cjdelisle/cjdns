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
// needed for String_vprintf()
#include <stdarg.h>

#include "admin/Admin.h"
#include "admin/AdminLog.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "crypto/random/Random.h"
#include "io/Writer.h"
#include "memory/BufferAllocator.h"
#include "util/log/Log.h"
#include "util/log/Log_impl.h"
#include "util/Hex.h"


#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define MAX_SUBSCRIPTIONS 64
#define FILE_NAME_COUNT 32

struct Subscription
{
    /** The log level to match against, all higher levels will also be matched. */
    enum Log_Level level;

    /**
     * true if the file name is the internal name
     * which can be compared using a pointer equality check
     */
    bool internalName : 1;

    /** The line number within the file or 0 to match all lines. */
    int lineNum : 31;

    /** The name of the file to match against or null to match any file. */
    const char* file;

    /** The transaction ID of the message which solicited this stream of logs. */
    String* txid;

    /** A hopefully unique (random) number identifying this stream. */
    uint8_t streamId[8];

    /** An allocator which will live during the lifecycle of the Subscription */
    struct Allocator* alloc;
};

struct AdminLog
{
    struct Log pub;
    struct Subscription subscriptions[MAX_SUBSCRIPTIONS];
    uint32_t subscriptionCount;
    const char* fileNames[FILE_NAME_COUNT];
    struct Admin* admin;
    struct Allocator* alloc;
    struct Random* rand;
};

static inline const char* getShortName(const char* fullFilePath)
{
    const char* out = CString_strrchr(fullFilePath, '/');
    if (out) {
        return out + 1;
    }
    return fullFilePath;
}

static inline bool isMatch(struct Subscription* subscription,
                           struct AdminLog* logger,
                           enum Log_Level logLevel,
                           const char* file,
                           int line)
{
    if (subscription->file) {
        if (subscription->internalName) {
            if (file != subscription->file) {
                return false;
            }
        } else {
            const char* shortFileName = getShortName(file);
            if (CString_strcmp(shortFileName, subscription->file)) {
                return false;
            }

            // It's the same name but so we'll swap the name for the internal name and then
            // it can be compared quickly with a pointer comparison.
            subscription->file = shortFileName;
            subscription->internalName = true;
            for (int i = 0; i < FILE_NAME_COUNT; i++) {
                if (logger->fileNames[i] == shortFileName) {
                    break;
                }
                if (logger->fileNames[i] == NULL) {
                    logger->fileNames[i] = shortFileName;
                    logger->fileNames[(i + 1) % FILE_NAME_COUNT] = NULL;
                    break;
                }
            }
        }
    }

    if (logLevel < subscription->level) {
        return false;
    }
    if (subscription->lineNum && line != subscription->lineNum) {
        return false;
    }
    return true;
}

static Dict* makeLogMessage(struct Subscription* subscription,
                            struct AdminLog* logger,
                            enum Log_Level logLevel,
                            const char* fullFilePath,
                            uint32_t line,
                            const char* format,
                            va_list vaArgs,
                            struct Allocator* alloc)
{
    time_t now;
    time(&now);

    Dict* out = Dict_new(alloc);
    char* buff = Allocator_malloc(alloc, 20);
    Hex_encode((uint8_t*)buff, 20, subscription->streamId, 8);
    Dict_putString(out, String_new("streamId", alloc), String_new(buff, alloc), alloc);
    Dict_putInt(out, String_new("time", alloc), now, alloc);
    Dict_putString(out,
                   String_new("level", alloc),
                   String_new(Log_nameForLevel(logLevel), alloc),
                   alloc);
    const char* shortName = getShortName(fullFilePath);
    Dict_putString(out, String_new("file", alloc), String_new((char*)shortName, alloc), alloc);
    Dict_putInt(out, String_new("line", alloc), line, alloc);
    String* message = String_vprintf(alloc, format, vaArgs);

    // Strip all of the annoying \n marks in the log entries.
    if (message->len > 0 && message->bytes[message->len - 1] == '\n') {
        message->len--;
    }
    Dict_putString(out, String_new("message", alloc), message, alloc);

    return out;
}

static void removeSubscription(struct AdminLog* log, struct Subscription* sub)
{
    Allocator_free(sub->alloc);
    log->subscriptionCount--;
    if (log->subscriptionCount == 0 || sub == &log->subscriptions[log->subscriptionCount]) {
        return;
    }
    Bits_memcpyConst(sub,
                     &log->subscriptions[log->subscriptionCount],
                     sizeof(struct Subscription));
}

static void doLog(struct Log* genericLog,
                  enum Log_Level logLevel,
                  const char* fullFilePath,
                  int line,
                  const char* format,
                  va_list args)
{
    struct AdminLog* log = (struct AdminLog*) genericLog;
    Dict* message = NULL;
    #define ALLOC_BUFFER_SZ 4096
    uint8_t allocBuffer[ALLOC_BUFFER_SZ];

    for (int i = 0; i < (int)log->subscriptionCount; i++) {
        if (isMatch(&log->subscriptions[i], log, logLevel, fullFilePath, line)) {
            if (!message) {
                struct Allocator* alloc = BufferAllocator_new(allocBuffer, ALLOC_BUFFER_SZ);
                message = makeLogMessage(&log->subscriptions[i],
                                         log,
                                         logLevel,
                                         fullFilePath,
                                         line,
                                         format,
                                         args,
                                         alloc);
            }
            int ret = Admin_sendMessage(message, log->subscriptions[i].txid, log->admin);
            if (ret) {
                removeSubscription(log, &log->subscriptions[i]);
            }
        }
    }
}

static void subscribe(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct AdminLog* log = (struct AdminLog*) vcontext;
    String* levelName = Dict_getString(args, String_CONST("level"));
    enum Log_Level level = (levelName) ? Log_levelForName(levelName->bytes) : Log_Level_DEBUG;
    int64_t* lineNumPtr = Dict_getInt(args, String_CONST("line"));
    String* fileStr = Dict_getString(args, String_CONST("file"));
    const char* file = (fileStr && fileStr->len > 0) ? fileStr->bytes : NULL;
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
        sub->level = level;
        sub->alloc = Allocator_child(log->alloc);
        if (file) {
            int i;
            for (i = 0; i < FILE_NAME_COUNT; i++) {
                if (log->fileNames[i] && !CString_strcmp(log->fileNames[i], file)) {
                    file = log->fileNames[i];
                    sub->internalName = true;
                    break;
                }
            }
            if (i == FILE_NAME_COUNT) {
                file = String_new(file, sub->alloc)->bytes;
                sub->internalName = false;
            }
        }
        sub->file = file;
        sub->lineNum = (lineNumPtr) ? *lineNumPtr : 0;
        sub->txid = String_clone(txid, sub->alloc);
        Random_bytes(log->rand, (uint8_t*) sub->streamId, 8);
        uint8_t streamIdHex[20];
        Hex_encode(streamIdHex, 20, sub->streamId, 8);
        Dict response = Dict_CONST(
            String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
            String_CONST("streamId"), String_OBJ(String_CONST((char*)streamIdHex)), NULL
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
    struct AdminLog* log = (struct AdminLog*) vcontext;
    String* streamIdHex = Dict_getString(args, String_CONST("streamId"));
    uint8_t streamId[8];
    char* error = NULL;
    if (streamIdHex->len != 16 || Hex_decode(streamId, 8, (uint8_t*)streamIdHex->bytes, 16) != 8) {
        error = "Invalid streamId.";
    } else {
        error = "No such subscription.";
        for (int i = 0; i < (int)log->subscriptionCount; i++) {
            if (!Bits_memcmp(streamId, log->subscriptions[i].streamId, 8)) {
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

struct Log* AdminLog_registerNew(struct Admin* admin, struct Allocator* alloc, struct Random* rand)
{
    struct AdminLog* log = Allocator_clone(alloc, (&(struct AdminLog) {
        .pub = {
            .print = doLog
        },
        .admin = admin,
        .alloc = alloc,
        .rand = rand
    }));

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

    return &log->pub;
}
