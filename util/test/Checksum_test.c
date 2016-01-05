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
#include "util/Checksum.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Hex.h"

#include "util/Assert.h"
#include <stdio.h>


#define UDP_PACKET_HEX    /* vvvv --- IPv4 header checksum  */\
    "4500""0033""7a95""4000""4011""f877""c0a8""0102""0402""0201" \
    /* UDP checksum -- vvvv */\
    "9e05""0035""001f""c7dd" \
    /* content */\
    "221a""0100""0001""0000""0000""0000""0564""6562""6f38""0000""1c00""01"

static const uint8_t* udpPacketHex = (uint8_t*) UDP_PACKET_HEX;
#define UDP_PACKET_SIZE ((sizeof(UDP_PACKET_HEX)-1)/2)

static void checksumAlgorithmTest()
{
    uint8_t packetBuff[UDP_PACKET_SIZE + 1];
    uint8_t* packet = packetBuff + ((uintptr_t)packetBuff % 2);
    Hex_decode(packet, UDP_PACKET_SIZE, udpPacketHex, UDP_PACKET_SIZE * 2);

    //operating on the ip checksum which is easy to compute.
    uint16_t checksum;
    Bits_memcpy(&checksum, &packet[8], 2);

    packet[8] = 0;
    packet[9] = 0;

    uint16_t calcatedSum = Checksum_engine(packet, 20);
    //printf("%2x == %2x", checksum, calcatedSum);
    Assert_true(checksum == calcatedSum);
}



#define UDP6_PACKET_HEX \
    /* Packet type --- vv */ \
    "6000""0000""0019""1140" \
    /* Source addr. */ \
    "fce5""de17""cbde""c87b""5289""0556""8b83""c9c8" \
    /* Dest addr. */ \
    "fc00""0000""0000""0000""0000""0000""0000""0001" \
    /* UDP checksum -- vvvv */ \
    "b4a9""0035""0019""4972" \
    /* Content */ \
    "e4e4""0100""0001""0000""0000""0000""0000""0200" \
    "01"
static const uint8_t* udp6PacketHex = (uint8_t*) UDP6_PACKET_HEX;
#define UDP6_PACKET_SIZE ((sizeof(UDP6_PACKET_HEX)-1)/2)

static void udp6ChecksumTest()
{
    uint8_t packet[UDP6_PACKET_SIZE + 3];
    Hex_decode(packet, UDP6_PACKET_SIZE, udp6PacketHex, UDP6_PACKET_SIZE * 2);

    // add some evil at the end to check for buffer overrun
    packet[UDP6_PACKET_SIZE] = 0x00;
    packet[UDP6_PACKET_SIZE + 1] = 0x00;
    packet[UDP6_PACKET_SIZE + 2] = 0x00;

    uint16_t udp6Checksum;
    Bits_memcpy(&udp6Checksum, &packet[46], 2);

    packet[46] = 0;
    packet[47] = 0;

    uint16_t calcatedSum = Checksum_udpIp6(&packet[8], &packet[40], 25);
    printf("%2x == %2x", udp6Checksum, calcatedSum);
    Assert_true(udp6Checksum == calcatedSum);
}


#define PING6_PACKET_HEX \
    /* Packet type --- vv */ \
    "6000""0000""0040""3a40" \
    /* Source Addr. */\
    "fce5""de17""cbde""c87b""5289""0556""8b83""c9c8" \
    /* Dest Addr. */\
    "fc00""0000""0000""0000""0000""0000""0000""0001" \
    /*     vvvv -- ICMP6 Checksum */\
    "8000""1d00" \
    /* Content */\
    "792e""0020""11d3""a04f""0000""0000""57bb""0800" \
    "0000""0000""1011""1213""1415""1617""1819""1a1b" \
    "1c1d""1e1f""2021""2223""2425""2627""2829""2a2b" \
    "2c2d""2e2f""3031""3233""3435""3637"
static const uint8_t* ping6PacketHex = (uint8_t*) PING6_PACKET_HEX;
#define PING6_PACKET_SIZE ((sizeof(PING6_PACKET_HEX)-1)/2)


static void icmp6ChecksumTest()
{
    uint8_t packet[PING6_PACKET_SIZE];
    Hex_decode(packet, PING6_PACKET_SIZE, ping6PacketHex, PING6_PACKET_SIZE * 2);

    uint16_t checksum;
    Bits_memcpy(&checksum, &packet[42], 2);

    packet[42] = 0;
    packet[43] = 0;

    uint16_t calcatedSum = Checksum_icmp6(&packet[8], &packet[40], 64);
    //printf("%2x == %2x", checksum, calcatedSum);
    Assert_true(checksum == calcatedSum);
}


int main()
{
    checksumAlgorithmTest();
    udp6ChecksumTest();
    icmp6ChecksumTest();
    return 0;
}
