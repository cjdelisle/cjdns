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
#include "memory/Allocator.h"
#include "net/PeerLink.h"
#include "util/Identity.h"
#include "util/Kbps.h"
#include "wire/SwitchHeader.h"
#include "util/events/Time.h"

#define ArrayList_TYPE struct Message
#define ArrayList_NAME Messages
#include "util/ArrayList.h"

struct PeerLink_pvt
{
    struct PeerLink pub;
    struct Allocator* alloc;
    struct EventBase* base;
    struct ArrayList_Messages* queue;
    struct Kbps sendBw;
    struct Kbps recvBw;
    Identity
};

struct Message* PeerLink_poll(struct PeerLink* peerLink)
{
    struct PeerLink_pvt* pl = Identity_check((struct PeerLink_pvt*) peerLink);
    struct Message* out = ArrayList_Messages_shift(pl->queue);
    if (!out) { return NULL; }
    Allocator_disown(pl->alloc, out->alloc);
    Kbps_accumulate(&pl->sendBw, Time_currentTimeMilliseconds(pl->base), out->length);
    return out;
}

int PeerLink_send(struct Message* msg, struct PeerLink* peerLink)
{
    struct PeerLink_pvt* pl = Identity_check((struct PeerLink_pvt*) peerLink);
    Allocator_adopt(pl->alloc, msg->alloc);
    ArrayList_Messages_add(pl->queue, msg);
    return pl->queue->length;
}

void PeerLink_recv(struct Message* msg, struct PeerLink* peerLink)
{
    struct PeerLink_pvt* pl = Identity_check((struct PeerLink_pvt*) peerLink);
    Kbps_accumulate(&pl->recvBw, Time_currentTimeMilliseconds(pl->base), msg->length);
}

void PeerLink_kbps(struct PeerLink* peerLink, struct PeerLink_Kbps* output)
{
    struct PeerLink_pvt* pl = Identity_check((struct PeerLink_pvt*) peerLink);
    uint32_t now = Time_currentTimeMilliseconds(pl->base);
    output->recvKbps = Kbps_accumulate(&pl->recvBw, now, Kbps_accumulate_NO_PACKET);
    output->sendKbps = Kbps_accumulate(&pl->sendBw, now, Kbps_accumulate_NO_PACKET);
}

struct PeerLink* PeerLink_new(struct EventBase* base, struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct PeerLink_pvt* pl = Allocator_calloc(alloc, sizeof(struct PeerLink_pvt), 1);
    Identity_set(pl);
    pl->base = base;
    pl->alloc = alloc;
    pl->queue = ArrayList_Messages_new(alloc);
    return &pl->pub;
}
