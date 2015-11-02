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
#include "arcroute/ArcRoute.h"
#include "util/Identity.h"
#include "util/events/Timeout.h"
#include "util/events/Time.h"

struct ArcRoute
{
    struct Iface controlIf;
};

struct ArcRoute_pvt
{
    struct ArcRoute pub;

    struct Timeout* beaconCycle;
    struct Allocator* beaconCycleAlloc;

    struct Allocator* alloc;
    Identity
};

/** Plus or minus 4k */
static uint32_t aroundSixteenThousandMilliseconds(struct Random* rand)
{
    return ( Random_uint16(rand) >> 3 ) + 8192;
}

static Iface_DEFUN messageFromControlHandler(struct Message* msg, struct Iface* iface)
{
    struct ArcRoute_pvt* ctx = Identity_check((struct ArcRoute_pvt*) iface);
    struct SwitchHeader* switchHeader = (struct SwitchHeader*) msg->bytes;
    ctx->incomingLabel = Endian_bigEndianToHost64(switchHeader->label_be);
    ctx->incomingVersion = 0;
    Message_shift(msg, -SwitchHeader_SIZE, NULL);

    uint32_t handle = Message_pop32(msg, NULL);
    Assert_true(handle == 0xffffffff);

    struct Control* ctrl = (struct Control*) msg->bytes;
    if (ctrl->header.type_be == Control_PONG_be) {
    }
}

static void beaconCycle(void* vArcRoute)
{
    struct ArcRoute_pvt* arp = Identity_check((struct ArcRoute_pvt*) vArcRoute);

    Allocator_free(arp->beaconCycleAlloc);
    struct Allocator* bsa = arp->beaconCycleAlloc = Allocator_child(arp->alloc);
    arp->beaconCycle = Timeout_setTimeout(
        beaconCycle, arp, aroundSixteenThousandMilliseconds(arp->rand), arp->base, bsa);


}

struct ArcRoute* ArcRoute_new(struct EventBase* event,
                              struct Random* rand,
                              struct Allocator* allocator,
                              uint8_t privateKey[32])
{

}
