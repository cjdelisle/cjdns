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
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "net/ConverterV15.h"
#include "util/Identity.h"
#include "wire/SwitchHeader.h"
#include "wire/DataHeader.h"
#include "net/BalingWire.h"
#include "wire/ContentType.h"
#include "wire/Headers.h"
#include "util/Checksum.h"

#include "util/Hex.h"

struct ConverterV15_pvt
{
    struct ConverterV15 pub;
    struct SessionTable* sm;
    uint8_t myIp6[16];
    struct Log* log;
    Identity
};

/**
 * Incoming packet with a BalingWire header followed by a ContentHeader and then whatever
 * content.
 */
static int incomingFromUpperDistributorIf(struct Interface_Two* upperDistributorIf,
                                          struct Message* msg)
{
    struct ConverterV15_pvt* conv =
        Identity_containerOf(upperDistributorIf, struct ConverterV15_pvt, pub.upperDistributorIf);

    Assert_true(msg->length >= DataHeader_SIZE + BalingWire_InsideHeader_SIZE);

    struct BalingWire_InsideHeader* hdr = (struct BalingWire_InsideHeader*) msg->bytes;
    struct SessionTable_Session* sess = SessionTable_sessionForIp6(hdr->ip6, conv->sm);
    if (!hdr->version && (!sess->version || sess->version > 15)) {
        // If nothing is known about a node, fuckit, assume it's new !
        return Interface_send(&conv->pub.balingWireIf, msg);
    }

    struct DataHeader* dh = (struct DataHeader*) &hdr[1];
    enum ContentType type = DataHeader_getContentType(dh);

    if (type > 257) {
        Log_debug(conv->log, "DROP unconvertible type [%d]", type);
        return 0;
    }

    // My fears, come alive, in this place where I once died
    // daemons dreamin', goin' eyed, I just needed to
    // RE-ALIGN
    Message_shift(msg, -(DataHeader_SIZE + BalingWire_InsideHeader_SIZE), NULL);
    if (type == ContentType_CJDHT) {
        // push a udp header and then an ip header and then checksum the udp
        Message_shift(msg, Headers_UDPHeader_SIZE, NULL);
    }
    if (type != ContentType_IPTUN) {
        Message_shift(msg, Headers_IP6Header_SIZE, NULL);
    }
    Message_shift(msg, BalingWire_InsideHeader_SIZE, NULL);
    Bits_memmove(msg->bytes, hdr, BalingWire_InsideHeader_SIZE);
    hdr = (struct BalingWire_InsideHeader*) msg->bytes;


    if (type == ContentType_IPTUN) {
        return Interface_send(&conv->pub.balingWireIf, msg);
    }

    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) &hdr[1];
    Bits_memset(ip6, 0, Headers_IP6Header_SIZE);
    Headers_setIpVersion(ip6);
    ip6->hopLimit = 42;
    ip6->nextHeader = type;
    ip6->payloadLength_be = Endian_hostToBigEndian16(
        msg->length - BalingWire_InsideHeader_SIZE - Headers_IP6Header_SIZE);
    Bits_memcpyConst(ip6->destinationAddr, hdr->ip6, 16);
    Bits_memcpyConst(ip6->sourceAddr, conv->myIp6, 16);

    if (type == ContentType_CJDHT) {
        struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) &ip6[1];
        ip6->nextHeader = 17;
        udp->srcPort_be = 0;
        udp->destPort_be = 0;
        udp->length_be = Endian_hostToBigEndian16(msg->length -
                                                  BalingWire_InsideHeader_SIZE -
                                                  Headers_IP6Header_SIZE -
                                                  Headers_UDPHeader_SIZE);
        udp->checksum_be = 0;
        udp->checksum_be =
            Checksum_udpIp6(ip6->sourceAddr,
                            (uint8_t*) udp,
                            msg->length - BalingWire_InsideHeader_SIZE - Headers_IP6Header_SIZE);

        Log_debug(conv->log, "Converted CJDHT->v15");
    }

    //Log_debug(conv->log, "send [%s]", Hex_print(ip6, 32, msg->alloc));

    return Interface_send(&conv->pub.balingWireIf, msg);
}

//// --------------- Incoming, convert v15 to v16 --------------- ////

#define tryConvertDHT_OVERHEAD \
    (BalingWire_InsideHeader_SIZE + Headers_IP6Header_SIZE + Headers_UDPHeader_SIZE)
static inline bool tryConvertDHT(struct Message* msg, struct Headers_IP6Header* ip6)
{
    if (msg->length < tryConvertDHT_OVERHEAD) { return false; }
    struct BalingWire_InsideHeader* bih = (struct BalingWire_InsideHeader*) msg->bytes;
    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) &ip6[1];
    if (udp->srcPort_be || udp->destPort_be) { return false; }
    Message_shift(msg, -tryConvertDHT_OVERHEAD, NULL);
    struct DataHeader dh = {
        .contentType_be = Endian_hostToBigEndian16(ContentType_CJDHT),
        .versionAndFlags = DataHeader_CURRENT_VERSION << 4
    };
    Message_push(msg, &dh, DataHeader_SIZE, NULL);
    Message_shift(msg, BalingWire_InsideHeader_SIZE, NULL);
    Bits_memmoveConst(msg->bytes, bih, BalingWire_InsideHeader_SIZE);
    return true;
}

/**
 * Incoming packet with a BalingWire header and under that either an ipv6 or ipv4 header
 * depending on whether it's destine for TUN/DHT or IpTunnel.
 */
static int incomingFromBalingWireIf(struct Interface_Two* balingWireIf, struct Message* msg)
{
    struct ConverterV15_pvt* conv =
        Identity_containerOf(balingWireIf, struct ConverterV15_pvt, pub.balingWireIf);

    Log_debug(conv->log, "incomingFromBalingWireIf");

    if (msg->length < BalingWire_InsideHeader_SIZE + 8) {
        Log_debug(conv->log, "DROP runt");
        return 0;
    }

    struct BalingWire_InsideHeader* bih = (struct BalingWire_InsideHeader*) msg->bytes;

    uint8_t* ipPtr = (uint8_t*) &bih[1];

    Log_debug(conv->log, "recv [%s]", Hex_print(ipPtr, 32, msg->alloc));

    int ipVer = Headers_getIpVersion(ipPtr);
    if (ipVer == DataHeader_CURRENT_VERSION) {
    Log_debug(conv->log, "0");
        return Interface_send(&conv->pub.upperDistributorIf, msg);
    }

    if (ipVer == 6) {
        if (msg->length < BalingWire_InsideHeader_SIZE + Headers_IP6Header_SIZE) {
            Log_debug(conv->log, "DROP runt");
            return 0;
        }
        struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) ipPtr;
        if (ip6->sourceAddr[0] == 0xfc && ip6->destinationAddr[0] == 0xfc) {
    Log_debug(conv->log, "tryConvertDHT()");
            if (tryConvertDHT(msg, ip6)) {
                return Interface_send(&conv->pub.upperDistributorIf, msg);
            }
    Log_debug(conv->log, "tryConvertDHT(fail)");
            Message_pop(msg, NULL, BalingWire_InsideHeader_SIZE + Headers_IP6Header_SIZE, NULL);
            struct DataHeader dh = {
                .contentType_be = Endian_hostToBigEndian16(ip6->nextHeader),
                .versionAndFlags = DataHeader_CURRENT_VERSION << 4
            };
            Message_push(msg, &dh, DataHeader_SIZE, NULL);
            Message_shift(msg, BalingWire_InsideHeader_SIZE, NULL);
            Bits_memmoveConst(msg->bytes, bih, BalingWire_InsideHeader_SIZE);
            return Interface_send(&conv->pub.upperDistributorIf, msg);
        }
    Log_debug(conv->log, "iptunnel?");
    } else if (ipVer != 4) {
        Log_debug(conv->log, "DROP unknown packet ip version");
        return 0;
    }

    Message_shift(msg, DataHeader_SIZE, NULL);
    Bits_memmoveConst(msg->bytes, bih, BalingWire_InsideHeader_SIZE);

    bih = (struct BalingWire_InsideHeader*) msg->bytes;
    ipPtr = (uint8_t*) &bih[1];

    struct DataHeader* dh = (struct DataHeader*) &bih[1];
    Bits_memset(dh, 0, DataHeader_SIZE);
    dh->contentType_be = Endian_hostToBigEndian16(ContentType_IPTUN);
    dh->versionAndFlags = DataHeader_CURRENT_VERSION << 4;
    return Interface_send(&conv->pub.upperDistributorIf, msg);
}

struct ConverterV15* ConverterV15_new(struct Allocator* alloc,
                                      struct Log* log,
                                      struct SessionTable* sm,
                                      uint8_t myIp6[16])
{
    struct ConverterV15_pvt* out = Allocator_calloc(alloc, sizeof(struct ConverterV15_pvt), 1);
    out->pub.upperDistributorIf.send = incomingFromUpperDistributorIf;
    out->pub.balingWireIf.send = incomingFromBalingWireIf;
    out->log = log;
    out->sm = sm;
    Bits_memcpyConst(out->myIp6, myIp6, 16);
    Identity_set(out);
    return &out->pub;
}
