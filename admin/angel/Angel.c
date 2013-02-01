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
#include "admin/angel/Angel.h"
#include "benc/String.h"
#include "memory/Allocator.h"
#include "interface/Interface.h"
#include "interface/PipeInterface.h"
#include "interface/Interface.h"
#include "util/platform/libc/strlen.h"
#include "util/platform/Socket.h"
#include "util/events/Event.h"
#include "util/Bits.h"
#include "util/Errno.h"
#include "util/log/Log.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "wire/Message.h"
#include "wire/Error.h"

struct AngelContext
{
    struct Interface* coreIface;
    struct EventBase* eventBase;
    struct Allocator* alloc;
    struct Log* logger;
};

static void handleMessageForAngel(struct Message* message, struct AngelContext* context)
{
    Log_debug(context->logger, "Got message for angel with content [%s]", message->bytes);
    char* angelExit = "d1:q10:Angel_exite";
    if (message->length == (int32_t)strlen(angelExit)
        && !Bits_memcmp((char*)message->bytes, angelExit, message->length))
    {
        Log_info(context->logger, "Got request to exit");
        exit(0);
    }
}

/**
 * send message via pipe to core process
 *
static void sendToCore(struct Message* message, uint32_t connNumber, struct AngelContext* context)
{
    Message_shift(message, 4);
    Bits_memcpyConst(message->bytes, &connNumber, 4);
    //Log_debug(context->logger, "sending Message to core");
    context->coreIface->sendMessage(message, context->coreIface);
}*/

/**
 * handle message on the pipe from core process
 */
static uint8_t receiveMessage(struct Message* message, struct Interface* iface)
{
    struct AngelContext* context = iface->receiverContext;

    Assert_true(message->length >= 4);

    uint32_t handle;
    Bits_memcpyConst(&handle, message->bytes, 4);
    Message_shift(message, -4);

    if (handle == 0xffffffff) {
        handleMessageForAngel(message, context);
        return Error_NONE;
    }

    return Error_NONE;
}

void Angel_start(struct Interface* coreIface,
                 struct EventBase* eventBase,
                 struct Log* logger,
                 struct Allocator* alloc)
{
    struct AngelContext ctx = {
        .eventBase = eventBase,
        .logger = logger,
        .coreIface = coreIface,
        .alloc = alloc
    };

    coreIface->receiveMessage = receiveMessage;
    coreIface->receiverContext = &ctx;

    EventBase_beginLoop(eventBase);
}
