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
#include "util/Bits.h"
#include "util/Pinger.h"
#include "crypto/random/Random.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"

struct Ping
{
    struct Pinger_Ping pub;
    struct Pinger* pinger;
    struct Timeout* timeout;
    String* data;
    int64_t timeSent;
    uint64_t cookie;
    Pinger_SEND_PING(sendPing);
    Pinger_ON_RESPONSE(onResponse);
    Identity
};

#define Map_ENABLE_HANDLES
#define Map_VALUE_TYPE struct Ping*
#define Map_NAME OutstandingPings
#include "util/Map.h"

struct Pinger
{
    struct Map_OutstandingPings outstandingPings;
    struct EventBase* eventBase;
    struct Random* rand;
    struct Log* logger;
    struct Allocator* allocator;

    /** Make all handles for different pingers wildly different to simplify debugging. */
    uint32_t baseHandle;
};

static void callback(String* data, struct Ping* ping)
{
    uint32_t now = Time_currentTimeMilliseconds(ping->pinger->eventBase);
    ping->onResponse(data, now - ping->timeSent, ping->pub.context);

    // Flag the freePing function to tell it that the ping was not terminated by the user...
    ping->timeSent = 0;

    Allocator_free(ping->pub.pingAlloc);
}

static void timeoutCallback(void* vping)
{
    struct Ping* p = Identity_check((struct Ping*) vping);
    if (!p->timeSent) {
        // The ping came in at the same time that the timeout was scheduled to happen.
        return;
    }
    int64_t now = Time_currentTimeMilliseconds(p->pinger->eventBase);
    long long diff = ((long long) now) - ((long long)p->timeSent);
    Assert_true(diff < 1000000000);
    Log_debug(p->pinger->logger, "Ping timeout for [%u] in [%lld] ms", p->pub.handle, diff);
    callback(NULL, p);
}

static int freePing(struct Allocator_OnFreeJob* job)
{
    struct Ping* p = Identity_check((struct Ping*) job->userData);

    if (p->timeSent) {
        //Log_debug(p->pinger->logger, "Ping cancelled [%u]", p->pub.handle);
    }

    int index = Map_OutstandingPings_indexForHandle(p->pub.handle - p->pinger->baseHandle,
                                                    &p->pinger->outstandingPings);
    Assert_true(index > -1);
    Map_OutstandingPings_remove(index, &p->pinger->outstandingPings);
    return 0;
}

static void asyncSendPing(void* vping)
{
    struct Ping* p = Identity_check((struct Ping*) vping);
    //Log_debug(p->pinger->logger, "Sending ping [%u]", p->pub.handle);
    p->sendPing(p->data, p->pub.context);
}

struct Pinger_Ping* Pinger_newPing(String* data,
                                   Pinger_ON_RESPONSE(onResponse),
                                   Pinger_SEND_PING(sendPing),
                                   uint32_t timeoutMilliseconds,
                                   struct Allocator* allocator,
                                   struct Pinger* pinger)
{
    struct Allocator* alloc = Allocator_child(allocator);

    struct Ping* ping = Allocator_clone(alloc, (&(struct Ping) {
        .pub = {
            .pingAlloc = alloc,
        },
        .sendPing = sendPing,
        .pinger = pinger,
        .timeSent = Time_currentTimeMilliseconds(pinger->eventBase),
        .onResponse = onResponse
    }));
    Identity_set(ping);

    int pingIndex = Map_OutstandingPings_put(&ping, &pinger->outstandingPings);
    ping->pub.handle = pinger->outstandingPings.handles[pingIndex] + pinger->baseHandle;

    ping->cookie = Random_uint64(pinger->rand);

    // Prefix the data with the handle and cookie
    String* toSend = String_newBinary(NULL, ((data) ? data->len : 0) + 12, alloc);
    Bits_memcpy(toSend->bytes, &ping->pub.handle, 4);
    Bits_memcpy(&toSend->bytes[4], &ping->cookie, 8);
    if (data) {
        Bits_memcpy(toSend->bytes + 12, data->bytes, data->len);
    }
    ping->data = toSend;

    Allocator_onFree(alloc, freePing, ping);

    ping->timeout =
        Timeout_setTimeout(timeoutCallback, ping, timeoutMilliseconds, pinger->eventBase, alloc);

    Timeout_setTimeout(asyncSendPing, ping, 0, pinger->eventBase, alloc);

    return &ping->pub;
}

void Pinger_pongReceived(String* data, struct Pinger* pinger)
{
    if (data->len < 12) {
        Log_debug(pinger->logger, "Invalid ping response, too short");
        return;
    }
    uint32_t handle;
    Bits_memcpy(&handle, data->bytes, 4);
    int index = Map_OutstandingPings_indexForHandle(handle - pinger->baseHandle,
                                                    &pinger->outstandingPings);
    if (index < 0) {
        Log_debug(pinger->logger, "Invalid ping response handle [%u].", handle);
    } else {
        data->len -= 4;
        data->bytes += 4;
        uint64_t cookie;
        Bits_memcpy(&cookie, data->bytes, 8);
        struct Ping* p = Identity_check((struct Ping*) pinger->outstandingPings.values[index]);
        if (cookie != p->cookie) {
            Log_debug(pinger->logger, "Ping response with invalid cookie");
            return;
        }
        data->len -= 8;
        data->bytes += 8;
        callback(data, p);
    }
}

struct Pinger* Pinger_new(struct EventBase* eventBase,
                          struct Random* rand,
                          struct Log* logger,
                          struct Allocator* alloc)
{
    struct Pinger* out = Allocator_clone(alloc, (&(struct Pinger) {
        .outstandingPings = {
            .allocator = alloc
        },
        .rand = rand,
        .eventBase = eventBase,
        .logger = logger,
        .allocator = alloc
    }));
    out->baseHandle = Random_uint32(rand);
    return out;
}
