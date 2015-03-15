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
#include "wire/SwitchHeader.h"

struct PeerLink_pvt
{
    struct PeerLink pub;
    struct Allocator* alloc;
    struct EventBase* base;
    Identity
};


struct Message* PeerLink_poll(struct PeerLink* peerLink)
{
}

int PeerLink_send(struct Message* msg, struct PeerLink* peerLink)
{
}

struct Message* PeerLink_recv(struct Message* msg, struct PeerLink* pl)
{
}

struct PeerLink* PeerLink_new(struct EventBase* base, struct Allocator* alloc)
{
    struct PeerLink_pvt* pl = Allocator_calloc(alloc, sizeof(struct PeerLink_pvt*), 1);
    Identity_set(pl);
    pl->base = base;
    pl->alloc = alloc;
    return pl->pub;
}
