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

#include "interface/ICMP6Generator.h"
#include "util/Checksum.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "wire/Headers.h"
#include "wire/Error.h"

#include <stddef.h>
#ifdef Linux
    #include <linux/ipv6.h>
    #include <linux/icmpv6.h>
#else
    #include <netinet/in.h>
    #include <netinet/ip_icmp.h>
#endif

/** MTU at switch layer, not including switch header overhead. */
#define EXPECTED_MTU \
    (1492 /* PPPoE */                                   \
        - Headers_IP4Header_SIZE                        \
        - Headers_UDPHeader_SIZE                        \
        - 20 /* CryptoAuth in ESTABLISHED state. */ )


#define CJDNS_OVERHEAD \
    (Headers_SwitchHeader_SIZE                                           \
        + 4 /* handle */                                                 \
        + Headers_CryptoAuth_SIZE                                        \
        /* IPv6 header goes here but operating system subtracts it. */   \
        + Headers_CryptoAuth_SIZE)


struct ICMP6Generator_pvt
{
    struct ICMP6Generator pub;

    Identity
};

static inline uint8_t numForType(enum ICMP6Generator_Type type)
{
    switch (type) {
        case ICMP6Generator_Type_NO_ROUTE_TO_HOST: return ICMPV6_DEST_UNREACH;
        case ICMP6Generator_Type_PACKET_TOO_BIG: return ICMPV6_PKT_TOOBIG;
        default: return 0;
    }
}

/**
 * Generate an ICMPv6 message.
 * The message parameter must contain all content which will be beneath the ICMPv6 header
 * including the MTU in the case of a "packet too big" message.
 *
 * @param message a message containing the content. This message must have enough padding
 *                to contain an additional ICMP header and IPv6 header totaling 44 bytes.
 * @param sourceAddr the IPv6 address which this ICMP message will be said to have come from.
 * @param destAddr the IPv6 address which this ICMP message will be directed to.
 * @param type the ICMP message type/code for this message.
 * @param mtu the MTU value for this message.
*/
void ICMP6Generator_generate(struct Message* msg,
                             const uint8_t* restrict sourceAddr,
                             const uint8_t* restrict destAddr,
                             enum ICMP6Generator_Type type,
                             uint32_t mtu)
{
    Message_shift(msg, Headers_ICMP6Header_SIZE);
    struct Headers_ICMP6Header* icmp6 = (struct Headers_ICMP6Header*) msg->bytes;
    Message_shift(msg, Headers_IP6Header_SIZE);
    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) msg->bytes;

    if (ICMP6Generator_MIN_IPV6_MTU < msg->length) {
        msg->length = ICMP6Generator_MIN_IPV6_MTU;
    }
    uint16_t contentLen = msg->length - Headers_IP6Header_SIZE;

    icmp6->type = numForType(type);
    icmp6->code = 0;
    icmp6->checksum = 0;
    icmp6->additional = Endian_hostToBigEndian32(mtu);

    ip6->versionClassAndFlowLabel = 0;
    Headers_setIpVersion(ip6);
    ip6->flowLabelLow_be = 0;
    ip6->payloadLength_be = Endian_hostToBigEndian16(contentLen);
    ip6->nextHeader = IPPROTO_ICMPV6;
    ip6->hopLimit = 64;

    Bits_memcpyConst(ip6->sourceAddr, sourceAddr, 16);
    Bits_memcpyConst(ip6->destinationAddr, destAddr, 16);

    icmp6->checksum = Checksum_icmp6(ip6->sourceAddr, (uint8_t*)icmp6, contentLen);
}

/** Message from the external (TUN facing) interface. */
static uint8_t incoming(struct Message* msg, struct Interface* iface)
{
    struct ICMP6Generator_pvt* ctx =
        Identity_cast((struct ICMP6Generator_pvt*)
            (((uint8_t*)iface) - offsetof(struct ICMP6Generator, external)));

    // source address for packets coming from the router.
    const uint8_t magicAddr[] = { 0xfc,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 };

    struct Headers_IP6Header* header = (struct Headers_IP6Header*) msg->bytes;
    // TODO calculate this on a per-node basis.
    if (msg->length >= Headers_IP6Header_SIZE
        && Headers_getIpVersion(header) == 6
        && msg->length > (EXPECTED_MTU - CJDNS_OVERHEAD))
    {
        uint8_t destAddr[16];
        Bits_memcpyConst(destAddr, header->sourceAddr, 16);
        ICMP6Generator_generate(msg,
                                magicAddr,
                                destAddr,
                                ICMP6Generator_Type_PACKET_TOO_BIG,
                                (EXPECTED_MTU - CJDNS_OVERHEAD));

        Interface_sendMessage(msg, &ctx->pub.external);
        return Error_NONE;
    }
    return Interface_sendMessage(msg, &ctx->pub.internal);
}

/** Message from the internal (Ducttape facing) interface. */
static uint8_t outgoing(struct Message* msg, struct Interface* iface)
{
    struct ICMP6Generator_pvt* ctx =
        Identity_cast((struct ICMP6Generator_pvt*)
            (((uint8_t*)iface) - offsetof(struct ICMP6Generator, internal)));

    return Interface_sendMessage(msg, &ctx->pub.external);
}

struct ICMP6Generator* ICMP6Generator_new(struct Allocator* alloc)
{
    struct ICMP6Generator_pvt* out = Allocator_clone(alloc, (&(struct ICMP6Generator_pvt) {
        .pub = {
            .external = {
                .sendMessage = incoming
            },
            .internal = {
                .sendMessage = outgoing
            }
        }
    }));
    Identity_set(out);
    return &out->pub;
}
