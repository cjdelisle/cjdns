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
#ifndef Link_H
#define Link_H

#include "util/Assert.h"
#include "switch/PenaltyFloat.h"

/** 10 minute window for calculating latency and loss on the link. */
#define Link_TIME_WINDOW (1000 * 60 * 10)

struct Link
{
    /** Everything below here is signed by the sender. */
    uint8_t senderSig[64];
    uint8_t senderSigningKey[32];

    /** Everything below here is signed by the receiver. */
    uint8_t receiverSig[64];
    uint8_t receiverSigningKey[32];

    /**
     * Millisecond time according to the node who sends on this link, allows new packets
     * to replace old. Replacement logic should account for wrapping by checking for lower numbers
     * with different highest bit.
     */
    uint64_t receiversClock_be;

    /**
     * Number of milliseconds of average RTT latency for the path.
     * Average over the past Link_TIME_WINDOW milliseconds.
     * If one-way latency is calculable, double it for this number.
     */
    uint16_t meanLatency_be;

    /** 65535 == 100% packet loss in the past Link_TIME_WINDOW milliseconds, 0 == 0% */
    uint16_t fractionLostPackets_be;

    /**
     * 64 bit number representing number of bits per second which the link can handle.
     * Packed into 16 bits, high 7 bits are the exponent, low 9 are the mantissa.
     */
    uint16_t estimatedBandwidthPacked_be;

    uint16_t mtuBytes_be;

    uint16_t receiversVersion_be;
};
#define Link_SIZE 204
Assert_compileTime(sizeof(struct Link) == Link_SIZE);

Endian_SETTER_GETTER_BE(Link, 64, ReceiversClock, receiversClock_be);
Endian_SETTER_GETTER_BE(Link, 16, MeanLatency, meanLatency_be);
Endian_SETTER_GETTER_BE(Link, 16, FractionLostPackets, fractionLostPackets_be);
Endian_SETTER_GETTER_BE(Link, 16, EstimatedBandwidthPacked, estimatedBandwidthPacked_be);
Endian_SETTER_GETTER_BE(Link, 16, MtuBytes, mtuBytes_be);
Endian_SETTER_GETTER_BE(Link, 16, ReceiversVersion, receiversVersion_be);

static inline uint64_t Link_getEstimatedBandwidth(struct Link* l)
{
    return PenaltyFloat_unpack( Endian_bigEndianToHost16(l->estimatedBandwidthPacked_be) );
}
static inline void Link_setEstimatedBandwidth(struct Link* l, uint64_t value)
{
    l->estimatedBandwidthPacked_be = Endian_hostToBigEndian16( PenaltyFloat_pack(value) );
}

#endif
