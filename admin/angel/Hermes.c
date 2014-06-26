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
#include "admin/angel/Hermes.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "memory/Allocator.h"
#include "interface/Interface.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "util/events/Event.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "util/Hex.h"

#define REQ_TIMEOUT 10000

struct Request
{
    struct Allocator* alloc;
    Hermes_onResponse onResponse;
    void* onResponseContext;
    struct Hermes* hermes;
    uint32_t handle;
    Identity
};

#define Map_NAME RequestSet
#define Map_VALUE_TYPE struct Request*
#define Map_ENABLE_HANDLES
#include "util/Map.h"

struct Hermes
{
    struct Interface* iface;
    struct Allocator* alloc;
    struct EventBase* eventBase;
    struct Map_RequestSet requestSet;
    struct Log* logger;
    Identity
};

/** Called when the request allocator is freed. */
static int removeReqFromSet(struct Allocator_OnFreeJob* job)
{
    struct Request* req = Identity_check((struct Request*) job->userData);
    struct Hermes* h = Identity_check(req->hermes);
    int index = Map_RequestSet_indexForHandle(req->handle, &h->requestSet);
    if (index > -1) {
        Map_RequestSet_remove(index, &h->requestSet);
    } else {
        Log_error(h->logger, "request with handle [%u] missing from set", req->handle);
    }
    return 0;
}

static void timeout(void* vrequest)
{
    struct Request* req = Identity_check((struct Request*) vrequest);
    Dict resp = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST("timeout")), NULL);
    req->onResponse(&resp, req->onResponseContext);
    Allocator_free(req->alloc);
}

static void receiveMessage2(struct Message* msg, struct Hermes* hermes, struct Allocator* tempAlloc)
{
    #ifdef Log_KEYS
        char lastChr = msg->bytes[msg->length - 1];
        msg->bytes[msg->length - 1] = '\0';
        Log_keys(hermes->logger, "Got message from angel [%s%c]", msg->bytes, lastChr);
        msg->bytes[msg->length - 1] = lastChr;
    #else
        Log_debug(hermes->logger, "Got message from angel");
    #endif

    struct Reader* reader = ArrayReader_new(msg->bytes, msg->length, tempAlloc);
    Dict d;
    if (StandardBencSerializer_get()->parseDictionary(reader, tempAlloc, &d)) {
        Log_warn(hermes->logger, "Failed to parse message from angel");
        return;
    }

    String* txid = Dict_getString(&d, String_CONST("txid"));
    uint32_t handle;
    if (!txid || txid->len != 8 || 4 != Hex_decode((uint8_t*)&handle, 4, (uint8_t*)txid->bytes, 8))
    {
        Log_warn(hermes->logger, "Message from angel; txid missing or unrecognized");
        return;
    }

    int index = Map_RequestSet_indexForHandle(handle, &hermes->requestSet);
    if (index < 0) {
        Log_warn(hermes->logger, "Message from angel references nonexistant request");
        return;
    }

    struct Request* req = Identity_check((struct Request*) hermes->requestSet.values[index]);
    req->onResponse(&d, req->onResponseContext);
    Allocator_free(req->alloc);
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct Hermes* hermes = Identity_check((struct Hermes*) iface->receiverContext);
    struct Allocator* alloc = Allocator_child(hermes->alloc);
    receiveMessage2(msg, hermes, alloc);
    Allocator_free(alloc);
    return 0;
}

void Hermes_callAngel(Dict* message,
                      Hermes_onResponse onResponse,
                      void* onResponseContext,
                      struct Allocator* alloc,
                      struct Except* eh,
                      struct Hermes* hermes)
{
    Identity_check(hermes);

    #define PADDING 32
    #define BUFF_SIZE 1024
    struct {
        uint8_t padding[PADDING];
        uint8_t message[BUFF_SIZE];
    } buff = { .padding = {0}, .message = {0} };

    struct Allocator* reqAlloc = Allocator_child(alloc);
    struct Request* req = Allocator_clone(reqAlloc, (&(struct Request) {
        .alloc = reqAlloc,
        .onResponse = onResponse,
        .onResponseContext = onResponseContext,
        .hermes = hermes
    }));

    Identity_set(req);

    int index = Map_RequestSet_put(&req, &hermes->requestSet);
    Allocator_onFree(reqAlloc, removeReqFromSet, req);

    uint32_t handle = hermes->requestSet.handles[index];
    req->handle = handle;
    uint8_t handleHex[9];
    Hex_encode(handleHex, 9, (uint8_t*)&handle, 4);
    Dict_putString(message, String_CONST("txid"), String_CONST((char*)handleHex), reqAlloc);

    struct Writer* writer = ArrayWriter_new(buff.message, BUFF_SIZE, reqAlloc);
    if (StandardBencSerializer_get()->serializeDictionary(writer, message)) {
        Except_throw(eh, "Failed to serialize message");
    }

    // Remove the txid string so there is not a dangling pointer in the message.
    Dict_remove(message, String_CONST("txid"));

    // This is done in a strange way but it is to prevent "possible buffer overflow" errors
    struct Message* m = &(struct Message) {
        .bytes = (uint8_t*) &buff,
        .length = writer->bytesWritten + PADDING,
        .padding = 0
    };
    m->capacity = m->length;
    Message_shift(m, -PADDING, NULL);

    Log_debug(hermes->logger, "Sending [%d] bytes to angel [%s].", m->length, m->bytes);

    m = Message_clone(m, reqAlloc);

    int ret = Interface_sendMessage(hermes->iface, m);
    if (ret) {
        Except_throw(eh, "Failed to send message to angel [%d]", ret);
    }

    // Use interval as defensive programming
    // the Allocator_free() in the timeout callback deactivates it.
    Timeout_setInterval(timeout, req, REQ_TIMEOUT, hermes->eventBase, reqAlloc);
}

struct Hermes* Hermes_new(struct Interface* angelIface,
                          struct EventBase* eventBase,
                          struct Log* logger,
                          struct Allocator* alloc)
{
    struct Hermes* out = Allocator_clone(alloc, (&(struct Hermes) {
        .iface = angelIface,
        .alloc = alloc,
        .eventBase = eventBase,
        .requestSet = {
            .allocator = alloc
        },
        .logger = logger
    }));
    Identity_set(out);
    angelIface->receiveMessage = receiveMessage;
    angelIface->receiverContext = out;
    return out;
}
