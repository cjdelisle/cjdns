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

#include "interface/ICMP6Generator_pvt.h"
#include "util/Checksum.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "wire/Headers.h"
#include "wire/Error.h"

#include <stddef.h>
#include <stdbool.h>

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


static inline uint8_t numForType(enum ICMP6Generator_Type type)
{
    switch (type) {
        case ICMP6Generator_Type_NO_ROUTE_TO_HOST: return 1; /* ICMPV6_DEST_UNREACH */
        case ICMP6Generator_Type_PACKET_TOO_BIG: return 2; /* ICMPV6_PKT_TOOBIG */
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
    Message_shift(msg, Headers_ICMP6Header_SIZE, NULL);
    struct Headers_ICMP6Header* icmp6 = (struct Headers_ICMP6Header*) msg->bytes;
    Message_shift(msg, Headers_IP6Header_SIZE, NULL);
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
    ip6->nextHeader = 58; /* IPPROTO_ICMPV6 */
    ip6->hopLimit = 64;

    Bits_memcpyConst(ip6->sourceAddr, sourceAddr, 16);
    Bits_memcpyConst(ip6->destinationAddr, destAddr, 16);

    icmp6->checksum = Checksum_icmp6(ip6->sourceAddr, (uint8_t*)icmp6, contentLen);
}

static uint8_t sendFragmented(struct ICMP6Generator_pvt* ctx,
                              struct Message* msg,
                              uint32_t mtu,
                              int offsetBytes)
{
    uint64_t msgHeader[(Headers_IP6Header_SIZE + Headers_IP6Fragment_SIZE) / 8];
    Assert_true(msg->length > (int)sizeof(msgHeader));
    Bits_memcpyConst(msgHeader, msg->bytes, sizeof(msgHeader));

    const int headersSize = (Headers_IP6Header_SIZE + Headers_IP6Fragment_SIZE);
    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) msg->bytes;
    struct Headers_IP6Fragment* frag = (struct Headers_IP6Fragment*) (&ip6[1]);
    Message_shift(msg, -headersSize, NULL);

    // prepare next message.
    struct Message* nextMessage = &(struct Message) {
        .bytes = msg->bytes,
        .length = msg->length,
        .padding = msg->padding
    };
    int nextMessageOffsetBytes = offsetBytes + (((mtu - headersSize) / 8) * 8);
    Message_shift(nextMessage, -(nextMessageOffsetBytes - offsetBytes), NULL);

    msg->length -= nextMessage->length;
    ip6->payloadLength_be = Endian_hostToBigEndian16(msg->length + Headers_IP6Fragment_SIZE);
    Message_shift(msg, headersSize, NULL);

    // sanity check
    #ifdef PARANOIA
        Assert_true(!Bits_memcmp(&msg->bytes[msg->length], nextMessage->bytes, 8));
        uint64_t msgNextPartFirstLong = ((uint64_t*)nextMessage->bytes)[0];
    #endif

    // Set the required fields.
    // RFC-2460 includes the fragment header in the offset so we have to add another 8 bytes.
    Headers_IP6Fragment_setOffset(frag, offsetBytes / 8);
    Headers_IP6Fragment_setMoreFragments(frag, true);
    Interface_receiveMessage(&ctx->pub.internal, msg);

    // sanity check
    Assert_ifParanoid(!Bits_memcmp(&msgNextPartFirstLong, nextMessage->bytes, 8));

    Message_shift(nextMessage, sizeof(msgHeader), NULL);
    Bits_memcpyConst(nextMessage->bytes, msgHeader, sizeof(msgHeader));

    if (nextMessage->length > (int)mtu) {
        return sendFragmented(ctx, nextMessage, mtu, nextMessageOffsetBytes);
    }

    // Set the required fields for the last fragment.
    ip6 = (struct Headers_IP6Header*) nextMessage->bytes;
    frag = (struct Headers_IP6Fragment*) (nextMessage->bytes + Headers_IP6Header_SIZE);
    Headers_IP6Fragment_setOffset(frag, nextMessageOffsetBytes / 8);
    // If the kernel did some fragmentation of it's own, we don't want to set the "last fragment"
    // flag so we'll leave it as it is.
    //Headers_IP6Fragment_setMoreFragments(frag, false);
    ip6->payloadLength_be = Endian_hostToBigEndian16(nextMessage->length - Headers_IP6Header_SIZE);

    return Interface_receiveMessage(&ctx->pub.internal, nextMessage);
}

/** Message from the external (TUN facing) interface. */
static uint8_t incoming(struct Message* msg, struct Interface* iface)
{
    struct ICMP6Generator_pvt* ctx =
        Identity_check((struct ICMP6Generator_pvt*)
            (((uint8_t*)iface) - offsetof(struct ICMP6Generator, external)));

    // TODO(cjd): calculate this on a per-node basis.
    int mtu = ctx->mtu;

    // source address for packets coming from the router.
    const uint8_t magicAddr[] = { 0xfc,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 };

    struct Headers_IP6Header* header = (struct Headers_IP6Header*) msg->bytes;
    if (msg->length >= Headers_IP6Header_SIZE
        && Headers_getIpVersion(header) == 6
        && msg->length > mtu)
    {
        if (header->nextHeader == Headers_IP6Fragment_TYPE) {
            return sendFragmented(ctx, msg, mtu, 0);
        }
        uint8_t destAddr[16];
        Bits_memcpyConst(destAddr, header->sourceAddr, 16);
        ICMP6Generator_generate(msg,
                                magicAddr,
                                destAddr,
                                ICMP6Generator_Type_PACKET_TOO_BIG,
                                mtu);

        Interface_receiveMessage(&ctx->pub.external, msg);
        return Error_NONE;
    }
    return Interface_receiveMessage(&ctx->pub.internal, msg);
}

/** Message from the internal (Ducttape facing) interface. */
static uint8_t outgoing(struct Message* msg, struct Interface* iface)
{
    struct ICMP6Generator_pvt* ctx =
        Identity_check((struct ICMP6Generator_pvt*)
            (((uint8_t*)iface) - offsetof(struct ICMP6Generator, internal)));

    return Interface_receiveMessage(&ctx->pub.external, msg);
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
        },
        .mtu = EXPECTED_MTU - CJDNS_OVERHEAD
    }));
    Identity_set(out);
    return &out->pub;
}
