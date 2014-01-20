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
#ifndef SwitchPinger_H
#define SwitchPinger_H

#include "benc/String.h"
#include "dht/Address.h"
#include "crypto/random/Random.h"
#include "interface/Interface.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("net/SwitchPinger.c")

#include <stdint.h>

#define SwitchPinger_DEFAULT_MAX_CONCURRENT_PINGS 50

enum SwitchPinger_Result
{
    /** Ping responded to ok. */
    SwitchPinger_Result_OK,

    /** Response label differs from sent label. */
    SwitchPinger_Result_LABEL_MISMATCH,

    /** Response contains different data than what was sent. */
    SwitchPinger_Result_WRONG_DATA,

    /** Instead of a normal response, got an error control packet. */
    SwitchPinger_Result_ERROR_RESPONSE,

    /** A sub-set of ERROR_RESPONSE where the route contains a loop. */
    SwitchPinger_Result_LOOP_ROUTE,

    /** Ping timeout. */
    SwitchPinger_Result_TIMEOUT
};


/**
 * Callback which will be called when the ping response comes back.
 *
 * @param result SwitchPinger_Result_OK if all went well,
 *                     otherwise SwitchPinger_Result_*
 * @param label the label as of the responding node in host order.
 * @param data the content of the ping response.
 * @param millisecondsLag the number of milliseconds since the original ping was sent.
 * @param nodeVersion the version of the node which was pinged.
 * @param onResponseContext a context which was provided to SwitchPinger_ping().
 */
typedef void (* SwitchPinger_ResponseCallback)(enum SwitchPinger_Result result,
                                               uint64_t label,
                                               String* data,
                                               uint32_t millisecondsLag,
                                               uint32_t nodeVersion,
                                               void* onResponseContext);

struct SwitchPinger_Ping
{
    /**
     * The allocator which is set by SwitchPinger_ping() and can be used to
     * allocate space which will be freed when the ping completes.
     */
    struct Allocator* pingAlloc;

    /**
     * This is NULL by default and is set by the caller of Pinger_ping(),
     * when onResponse() is called, whatever this is, will be given to it.
     */
    void* onResponseContext;
};

struct SwitchPinger;

String* SwitchPinger_resultString(enum SwitchPinger_Result result);

/**
 * Allocate a ping message.
 *
 * @param label the HOST ORDER label of the node to send the ping message to.
 * @param data the content of the ping to send, if NULL, an empty string will be
 *             returned in the response.
 * @param timeoutMilliseconds how long to wait before failing the ping.
 * @param onResponse the callback after the on pong or timeout.
 * @param alloc free this to cancel the ping.
 * @param ctx the pinger
 */
struct SwitchPinger_Ping* SwitchPinger_newPing(uint64_t label,
                                               String* data,
                                               uint32_t timeoutMilliseconds,
                                               SwitchPinger_ResponseCallback onResponse,
                                               struct Allocator* alloc,
                                               struct SwitchPinger* ctx);

struct SwitchPinger* SwitchPinger_new(struct Interface* iface,
                                      struct EventBase* eventBase,
                                      struct Random* rand,
                                      struct Log* logger,
                                      struct Address* myAddr,
                                      struct Allocator* alloc);

#endif
