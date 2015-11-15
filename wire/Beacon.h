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
#ifndef Beacon_H
#define Beacon_H

#include "util/Assert.h"
#include "util/Endian.h"

struct Beacon_SignedPart
{
    /** Sig covers everything below here. */
    uint8_t sig[64];
    uint8_t signingKey[32];

    uint8_t cryptoKey[32];

    uint64_t beaconersClock_be;

    uint32_t peerCount_be;
    uint32_t estimatedCoreness_be;

    /**
     * Number of hops between this node and it's chosen landmark.
     * Zero if this node believes itself to be a landmark.
     */
    uint32_t hopsToChosenLandmark_be;
}
#define Beacon_SignedPart_SIZE 148
Assert_compileTime(sizeof(struct Beacon_SignedPart) == Beacon_SignedPart_SIZE);

struct Beacon
{
    /** This number is the number of the beacon in the set of beacons sent to the given peer */
    uint32_t sequenceNumber_be;

    uint32_t hopsFromBeaconer_be;

    uint16_t meanLatency_be;

    uint16_t fractionLostPackets_be;

    uint16_t estimatedBandwidthPacked_be;

    uint16_t mtuBytes_be;

    struct Beacon_SignedPart signedPart;
};
#define Beacon_SIZE 164
Assert_compileTime(sizeof(struct Beacon) == Beacon_SIZE);

struct Beacon_GossupMsg
{
    /**
     * firstSequenceNumberAcknoledged_be means the lowest sequence number which has not yet been
     * acknoledged, bitfieldAcknoledged allows up to 32 beacons with similar sequence
     * numbers to be acknoledged at the same time, each bit represents 1 sequence number higher
     * than firstSequenceNumberAcknoledged_be being acknoledged.
     */
    uint32_t bitfieldAcknoledged_be;
    uint32_t firstSequenceNumberAcknoledged_be;

};

Endian_SETTER_GETTER_BE(Becaon, 32, SequenceNumber, sequenceNumber_be);
Endian_SETTER_GETTER_BE(Beacon, 32, HopsFromBeaconer, hopsFromBeaconer_be);
Endian_SETTER_GETTER_BE(Beacon, 16, MeanLatency, meanLatency_be);
Endian_SETTER_GETTER_BE(Beacon, 16, FractionLostPackets, fractionLostPackets_be);
Endian_SETTER_GETTER_BE(Beacon, 16, EstimatedBandwidthPacked, estimatedBandwidthPacked_be);
Endian_SETTER_GETTER_BE(Beacon, 16, MtuBytes, mtuBytes_be);

Endian_SETTER_GETTER_BE(Beacon, 64, BeaconersClock, signedPart.beaconersClock_be);
Endian_SETTER_GETTER_BE(Beacon, 32, PeerCount, signedPart.peerCount_be);
Endian_SETTER_GETTER_BE(Beacon, 32, EstimatedCoreness, signedPart.estimatedCoreness_be);
Endian_SETTER_GETTER_BE(Beacon, 32, HopsToChosenLandmark, signedPart.hopsToChosenLandmark_be);

#endif
