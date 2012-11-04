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
#include "crypto/Random.h"
#include "interface/PipeInterface.h"
#include "wire/Error.h"
#include "util/Errno.h"
#include "util/Time.h"
#include "util/Timeout.h"
#include "util/log/Log.h"

#include <unistd.h>
#include <event2/event.h>

#define PING_FREQUENCY_MILLISECONDS 3000
#define LAG_MAX_BEFORE_DISCONNECT 10000

static char* IDENTITY;

enum FramedMessageType
{
    FramedMessageType_PING,
    FramedMessageType_PONG,
    FramedMessageType_DATA
};

struct FramedMessage
{
    uint64_t syncMagic;
    enum FramedMessageType type;
    uint32_t length;
};
#define FramedMessage_SIZE 16
Assert_compileTime(FramedMessage_SIZE == sizeof(struct FramedMessage));

struct PipeInterface_pvt
{
    /** Public API. */
    struct PipeInterface pub;

    /** Must match IDENTITY, used to check if the wrong interface was passed. */
    char* identity;

    /** Outgoing pipe file descriptor. */
    int outPipe;

    /** Incoming pipe file descriptor. */
    int inPipe;

    /** The current amount of message which has been received. */
    uint32_t messageReceived;

    /** When the last message was sent. */
    uint64_t timeOfLastMessage;

    /** the event base for getting the current time. */
    struct event_base* eventBase;

    /** The interval for pinging to make sure the pipe is still open. */
    struct Timeout* pingInterval;

    struct event* pipeEvent;

    struct Log* logger;

    /** If true, the libevent event loop should be broken when the session is established. */
    bool isWaiting;

    /** location for storing the message. */
    struct {
        uint8_t padding[PipeInterface_PADDING];
        union {
            struct FramedMessage header;
            uint8_t bytes[PipeInterface_MAX_MESSAGE_SIZE];
        } as;
    } message;

    /** Sent before each message to allow the ends to resynchronize if sync is lost. */
    uint64_t syncMagic;
};

static uint8_t sendRaw(struct Message* m, struct PipeInterface_pvt* context)
{
    // TODO: buffer this if it cannot all be sent.
    if (write(context->outPipe, m->bytes, m->length) != m->length) {
        return Error_LINK_LIMIT_EXCEEDED;
    }
    return Error_NONE;
}

static uint8_t sendMessage(struct Message* m, struct Interface* iface)
{
    struct PipeInterface_pvt* context = (struct PipeInterface_pvt*) iface;
    Assert_true(context->identity == IDENTITY);

    if (context->pub.state == PipeInterface_State_INITIALIZING) {
        Log_warn(context->logger, "Failed to send message because PipeInterface is not ready");
        return Error_UNDELIVERABLE;
    }

    uint32_t length = m->length;
    Message_shift(m, FramedMessage_SIZE);
    struct FramedMessage* fm = (struct FramedMessage*) m->bytes;
    fm->syncMagic = context->syncMagic;
    fm->length = length;
    fm->type = FramedMessageType_DATA;

    return sendRaw(m, context);
}

static uint32_t doRead(int pipe, void* buffer, uint32_t maxLength, struct Except* eh)
{
    ssize_t amount = read(pipe, buffer, maxLength);
    if (amount < 1) {
        enum Errno err = Errno_get();
        if (Errno_EAGAIN == err) {
            return 0;
        }
        if (amount < 0) {
            Except_raise(eh, PipeInterface_DISCONNECTED,
                         "Connection Lost [%s]", Errno_strerror(err));
        }
        Except_raise(eh, PipeInterface_DISCONNECTED, "PipeInterface Connection Lost.");
    }
    return amount;
}

static bool tryReestablish(struct PipeInterface_pvt* context)
{
    uint8_t* nextFrame = Bits_memmem(context->message.as.bytes,
                                     &context->syncMagic,
                                     context->messageReceived,
                                     sizeof(context->syncMagic));

    if (nextFrame) {
        uint8_t* endOfMessage = context->message.as.bytes + context->messageReceived;
        uint32_t newLength = endOfMessage - nextFrame;
        Bits_memmove(context->message.as.bytes, nextFrame, newLength);
        context->messageReceived = newLength;
        context->pub.state = PipeInterface_State_ESTABLISHED;
        Log_debug(context->logger, "Reestablished synchronization, off by [%u] bytes",
                  (uint32_t) (nextFrame - context->message.as.bytes));
        return true;
    }
    return false;
}

static void sendEmpty(struct PipeInterface_pvt* context, enum FramedMessageType type)
{
    union {
        uint8_t bytes[FramedMessage_SIZE];
        struct FramedMessage header;
    } data = {
        .header = {
            .syncMagic = context->syncMagic,
            .type = type,
            .length = 0
        }
    };
    struct Message m = {
        .length = FramedMessage_SIZE,
        .bytes = data.bytes,
        .padding = 0
    };
    sendRaw(&m, context);
}

static void handlePing(struct PipeInterface_pvt* context)
{
    sendEmpty(context, FramedMessageType_PONG);
}

static void sendPing(struct PipeInterface_pvt* context)
{
    sendEmpty(context, FramedMessageType_PING);
}


static void handleData(struct PipeInterface_pvt* context, uint32_t length)
{
    if (context->pub.generic.receiveMessage) {
        struct Message m = {
            .length = length,
            .bytes = context->message.as.bytes + FramedMessage_SIZE,
            .padding = PipeInterface_PADDING
        };
        context->pub.generic.receiveMessage(&m, &context->pub.generic);
    }
}

static bool handleMessage(struct PipeInterface_pvt* context)
{
    if (context->messageReceived < FramedMessage_SIZE) {
        Log_debug(context->logger, "Dropping runt");
        return false;
    }

    if (context->pub.state == PipeInterface_State_INITIALIZING) {
        if (context->message.as.header.type == FramedMessageType_PONG) {
            sendPing(context);
        }
    }

    if (context->message.as.header.syncMagic != context->syncMagic) {

        if (context->pub.state == PipeInterface_State_INITIALIZING) {
            if (context->message.as.header.syncMagic < context->syncMagic) {
                context->syncMagic = context->message.as.header.syncMagic;
            }
        } else {
            Log_debug(context->logger, "Lost synchronization");
            context->pub.state = PipeInterface_State_LOST;
            bool ret = tryReestablish(context);
            sendPing(context);
            if (!ret) {
                return false;
            }
        }
    } else if (context->pub.state == PipeInterface_State_INITIALIZING) {
        Log_debug(context->logger, "[%p] Established Synchronization", (void*) context);
        context->pub.state = PipeInterface_State_ESTABLISHED;
    } else if (context->isWaiting) {
        context->isWaiting = false;
        event_base_loopbreak(context->eventBase);
    }

    uint32_t length = context->message.as.header.length + FramedMessage_SIZE;
    uint32_t remainingLength = context->messageReceived - length;
    if (context->messageReceived < length) {
        Log_debug(context->logger, "Dropping runt");
        return false;
    }

    if (context->message.as.header.type == FramedMessageType_PING) {
        handlePing(context);
    } else if (context->message.as.header.type == FramedMessageType_DATA) {
        handleData(context, length - FramedMessage_SIZE);
    } else if (context->message.as.header.type == FramedMessageType_PONG) {
        // do nothing
    } else {
        Log_info(context->logger, "Got unknown message");
    }

    context->messageReceived = remainingLength;
    if (remainingLength) {
        Bits_memmove(context->message.as.bytes,
                     context->message.as.bytes + length,
                     remainingLength);
        return true;
    }

    return false;
}

static void getMessageDesynced(struct PipeInterface_pvt* context)
{
    context->messageReceived = doRead(context->inPipe,
                                      context->message.as.bytes,
                                      PipeInterface_MAX_MESSAGE_SIZE,
                                      context->pub.exceptionHandler);
    if (tryReestablish(context)) {
        handleMessage(context);
    }
}

static void getMessage(struct PipeInterface_pvt* context)
{
    context->messageReceived += doRead(context->inPipe,
                                       context->message.as.bytes + context->messageReceived,
                                       PipeInterface_MAX_MESSAGE_SIZE - context->messageReceived,
                                       context->pub.exceptionHandler);

    enum PipeInterface_State state = context->pub.state;

    while (handleMessage(context));

    if (state == PipeInterface_State_INITIALIZING) {
        context->messageReceived = 0;
    }
}

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct PipeInterface_pvt* context = vcontext;
    if (context->pub.state == PipeInterface_State_LOST) {
        getMessageDesynced(context);
    } else {
        getMessage(context);
    }

    context->timeOfLastMessage = Time_currentTimeMilliseconds(context->eventBase);
}

static void handleTimeout(void* vcontext)
{
    struct PipeInterface_pvt* context = vcontext;
    uint64_t lag = (Time_currentTimeMilliseconds(context->eventBase) - context->timeOfLastMessage);
    if (lag > (UINT64_MAX >> 1)) {
        // backwards clock
        return;
    }

    if (lag > LAG_MAX_BEFORE_DISCONNECT
        && (context->pub.state != PipeInterface_State_INITIALIZING
            || lag > LAG_MAX_BEFORE_DISCONNECT * 2))
    {
        Except_raise(context->pub.exceptionHandler,
                     PipeInterface_TIMEOUT,
                     "Ping timeout");
    }
    if (lag > PING_FREQUENCY_MILLISECONDS * 2) {
        sendPing(context);
    }
}

static void onFree(void* vcontext)
{
    struct PipeInterface_pvt* context = vcontext;
    event_free(context->pipeEvent);
}

void PipeInterface_waitUntilReady(struct PipeInterface* pif)
{
    if (pif->state == PipeInterface_State_INITIALIZING) {
        struct PipeInterface_pvt* context = (struct PipeInterface_pvt*) pif;
        context->isWaiting = true;
        event_base_dispatch(context->eventBase);
    }
}

struct PipeInterface* PipeInterface_new(int inPipe,
                                        int outPipe,
                                        struct event_base* eventBase,
                                        struct Log* logger,
                                        struct Allocator* alloc,
                                        struct Random* rand)
{
    struct PipeInterface_pvt* context =
        alloc->clone(sizeof(struct PipeInterface_pvt), alloc, &(struct PipeInterface_pvt) {
            .pub = {
                .generic = {
                    .sendMessage = sendMessage
                },
                .state = PipeInterface_State_INITIALIZING
            },
            .identity = IDENTITY,
            .outPipe = outPipe,
            .inPipe = inPipe,
            .timeOfLastMessage = Time_currentTimeMilliseconds(eventBase),
            .eventBase = eventBase,
            .logger = logger
        });

    Log_info(logger, "Creating new PipeInterface [%p]", (void*)context);

    context->pingInterval = Timeout_setInterval(handleTimeout,
                                                context,
                                                PING_FREQUENCY_MILLISECONDS,
                                                eventBase,
                                                alloc);

    context->pipeEvent = event_new(eventBase, inPipe, EV_READ | EV_PERSIST, handleEvent, context);
    event_add(context->pipeEvent, NULL);
    alloc->onFree(onFree, context, alloc);

    Random_bytes(rand, (uint8_t*) &context->syncMagic, 8);

    sendPing(context);
    return &context->pub;
}
