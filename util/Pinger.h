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
#ifndef Pinger_H
#define Pinger_H

#include "benc/String.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "crypto/random/Random.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("util/Pinger.c");

/**
 * On pong received callback.
 * This is called when a response to the ping comes in.
 *
 * @param data the response content or NULL if the ping timed out, if Pinger_ping() was called with
 *             null, this will be an empty string.
 * @param milliseconds the number of milliseconds between the sending of the ping and this event.
 * @param context the context which is set in the Pinger_Ping struct returned by Pinger_ping().
 */
#define Pinger_ON_RESPONSE(x) \
    void (* x)(String* data, uint32_t milliseconds, void* context)

/**
 * Sender callback.
 * The pinger uses this user provided function to send the ping.
 * This function does not return, in the event of error, it is the user's responsibility to
 * communicate the error through the context back to the function which called Pinger_ping().
 *
 * @param data the content of the ping to send.
 * @param context the context which is set in the Pinger_Ping struct returned by Pinger_ping().
 */
#define Pinger_SEND_PING(x) void (* x)(String* data, void* context)


struct Pinger;

struct Pinger_Ping
{
    /**
     * The allocator which is set by Pinger_ping() and can be used to
     * allocate space which will be freed when the ping completes.
     */
    struct Allocator* pingAlloc;

    /** How the ping will be identified on the wire. */
    uint32_t handle;

    /**
     * This is NULL by default and is set by the caller of Pinger_ping(),
     * when sendPing() and onResponse() are called, whatever this is, will be passed to them.
     */
    void* context;
};

/**
 * @param data this is the bytes that you want to be reflected back to you.
 * @param onResponse this function will be called when the ping is responded to or times out.
 * @param sendPing the function which will be called to send the ping out to the other node.
 * @param timeoutMilliseconds the number of milliseconds to wait before timeout.
 * @param allocator cancel the ping by freeing this allocator.
 * @param pinger
 * @return a new Pinger_Ping if all goes well, NULL if there is no space.
 */
struct Pinger_Ping* Pinger_newPing(String* data,
                                   Pinger_ON_RESPONSE(onResponse),
                                   Pinger_SEND_PING(sendPing),
                                   uint32_t timeoutMilliseconds,
                                   struct Allocator* allocator,
                                   struct Pinger* pinger);

/**
 * Function to call when data comes in which appears to be a ping response.
 *
 * @param data the data as it comes in.
 * @param pinger the pinger context.
 */
void Pinger_pongReceived(String* data, struct Pinger* pinger);

/**
 * Create a new pinger.
 *
 * @param eventBase
 * @param logger
 * @param alloc
 */
struct Pinger* Pinger_new(struct EventBase* eventBase,
                          struct Random* rand,
                          struct Log* logger,
                          struct Allocator* alloc);


#endif
