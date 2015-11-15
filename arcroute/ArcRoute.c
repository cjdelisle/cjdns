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

#define ArcRoute_BEACON_NUM_RETRIES 3
#define ArcRoute_BEACON_MAX_QUEUE 24

struct BeaconEntry {
    struct Beacon beacon;
    uint64_t lastTimeSent;
    uint32_t numSends;
    struct Allocator* alloc;
};
#define ArrayList_TYPE struct BeaconEntry
#define ArrayList_NAME OfBeacons
#define ArrayList_INITIAL_CAPACITY ArcRoute_BEACON_MAX_QUEUE
#include "util/ArrayList.h"

struct ArcRoute_pvt;

struct PeerState {
    struct ArrayList_OfBeacons* beacons;
    char* peerName;
    struct ArcRoute_pvt* arp;
    struct Allocator* alloc;
};
#define ArrayList_TYPE struct PeerState
#define ArrayList_NAME OfPeers

struct ArcRoute_pvt
{
    struct ArcRoute pub;

    struct ArrayList_OfPeers* peers;

    struct Timeout* beaconCycle;
    struct Allocator* beaconCycleAlloc;

    struct Log* log;

    struct Allocator* alloc;
    Identity
};

static int worseBeacon(struct BeaconEntry* negativeIfWorse, struct BeaconEntry* positiveIfWorse)
{
    // evict stuff we've already sent first
    if (negativeIfWorse->numSends != positiveIfWorse->numSends) {
        return negativeIfWorse->numSends > positiveIfWorse->numSends ? -1 : 1;
    }
    uint64_t negClock = Beacon_getBeaconersClock(&negativeIfWorse->beacon);
    uint64_t posClock = Beacon_getBeaconersClock(&positiveIfWorse->beacon);
    if (negClock != posClock) {
        return negClock < posClock ? 1 : -1;
    }
    return Bits_memcmp(&negativeIfWorse->beacon, &positiveIfWorse->beacon, Beacon_SIZE);
}

static void putBeacon(struct PeerState* ps, struct Beacon* b, int index)
{
    Assert_true(index <= ps->beacons->length && index > -1);

    struct Allocator* beaconAlloc = Allocator_child(ps->alloc);
    struct BeaconEntry* newbb = Allocator_calloc(beaconAlloc, sizeof(struct BeaconEntry), 1);
    Bits_memcpy(&newbb->beacon, b, Beacon_SIZE);
    newbb->alloc = beaconAlloc;

    struct BeaconEntry* bb = ArrayList_OfBeacons_get(ps->beacons, index);
    ArrayList_OfBeacons_put(ps->beacons, index, newbb)
    if (bb) {
        Allocator_free(bb->alloc);
    }
}

static void addBeaconToPeer(struct PeerState* ps, struct Beacon* b)
{
    struct BeaconEntry* worstBeacon = NULL;
    int worstBeaconIndex = -1;
    for (int i = 0; i < ps->beacons->length; i++) {
        struct BeaconEntry* bb = ArrayList_OfBeacons_get(ps->beacons, i);
        Assert_true(bb);
        if (!Bits_memcmp(b->signedPart->signingKey, bb->beacon.signedPart.signingKey, 32)) {
            if (worseBeacon(&bb->beacon, b) < 0) {
                putBeacon(ps, b, i);
            }
            return;
        }
        if (!worstBeacon || worseBeacon(&bb->beacon, worstBeacon) < 0) {
            worstBeacon = bb->beacon;
            worstBeaconIndex = i;
        }
    }
    if (ps->beacons->length < ArcRoute_BEACON_MAX_QUEUE) {
        putBeacon(ps, b, ps->beacons->length);
        return;
    }
    Log_warn(ps->log, "Evicting a beacon because list for peer [%s] is full", peerName);
    Assert_true(worstBeaconIndex > -1);
    if (worseBeacon(b, worstBeacon) > 0) {
        putBeacon(ps, b, worstBeaconIndex);
    }
}

static void forwardBeacon(struct ArcRoute_pvt* arp, struct Beacon* b)
{
    for (int i = 0; i < arp->peers.length; i++) {

    }
}

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

    if (msg->length < Control_Header_SIZE + Control_ArcMsg_HEADER_SIZE) {
        Log_debug(ctx->log, "DROP runt");
        return NULL;
    }

    struct Control* ctrl = (struct Control*) msg->bytes;
    if (ctrl->header.type_be == Control_ARCREQ_be) {
        if (ctrl->arc.magic != Control_ArcMsg_REQMAGIC) {
            Log_debug(ctx->log, "DROP bad magic");
            return NULL;
        }
        ///

    } else if (ctrl->header.type_be == Control_ARCRES_be) {
        if (ctrl->arc.magic != Control_ArcMsg_RESMAGIC) {
            Log_debug(ctx->log, "DROP bad magic");
            return NULL;
        }
        ///
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
