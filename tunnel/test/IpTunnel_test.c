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
#include "memory/MallocAllocator.h"
#include "io/FileWriter.h"
#include "interface/tuntap/TUNMessageType.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"
#include "util/events/EventBase.h"
#include "crypto/random/Random.h"
#include "crypto/AddressCalc.h"
#include "tunnel/IpTunnel.h"
#include "util/Bits.h"
#include "util/Checksum.h"
#include "util/CString.h"
#include "wire/Message.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"

static uint8_t* fakePubKey = (uint8_t*) "abcdefghijklmnopqrstuvwxyz012345";
static uint8_t nodeCjdnsIp6[16];
static uint8_t* fakeIp6ToGive = (uint8_t*) "\xfd\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1";
static int called = 0;

static uint8_t responseWithIpCallback(struct Message* message, struct Interface* iface)
{
    struct IpTunnel_PacketInfoHeader* pi = (struct IpTunnel_PacketInfoHeader*) message->bytes;
    Assert_true(!Bits_memcmp(nodeCjdnsIp6, pi->nodeIp6Addr, 16));
    Assert_true(!Bits_memcmp(fakePubKey, pi->nodeKey, 32));

    Message_shift(message, -IpTunnel_PacketInfoHeader_SIZE, NULL);
    struct Headers_IP6Header* ip = (struct Headers_IP6Header*) message->bytes;
    Assert_true(Headers_getIpVersion(ip) == 6);
    uint16_t length = Endian_bigEndianToHost16(ip->payloadLength_be);
    Assert_true(length + Headers_IP6Header_SIZE == message->length);
    Assert_true(ip->nextHeader == 17);
    Assert_true(Bits_isZero(ip->sourceAddr, 32));

    Message_shift(message, -Headers_IP6Header_SIZE, NULL);
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message->bytes;
    Assert_true(!Checksum_udpIp6(ip->sourceAddr, message->bytes, length));

    Assert_true(uh->srcPort_be == 0);
    Assert_true(uh->destPort_be == 0);
    Assert_true(Endian_bigEndianToHost16(uh->length_be) + Headers_UDPHeader_SIZE == length);

    Message_shift(message, -Headers_UDPHeader_SIZE, NULL);
    char* expectedResponse =
        "d"
          "9:addresses" "d"
            "3:ip6" "16:\xfd\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"
          "e"
          "4:txid" "4:abcd"
        "e";
    Assert_true(message->length == (int32_t) CString_strlen(expectedResponse));
    Assert_true(!Bits_memcmp(message->bytes, expectedResponse, message->length));
    called = 1;
    return 0;
}

static uint8_t messageToTun(struct Message* message, struct Interface* iface)
{
    Assert_true(TUNMessageType_pop(message, NULL) == Ethernet_TYPE_IP6);
    struct Headers_IP6Header* ip = (struct Headers_IP6Header*) message->bytes;
    Assert_true(Headers_getIpVersion(ip) == 6);
    uint16_t length = Endian_bigEndianToHost16(ip->payloadLength_be);
    Assert_true(length + Headers_IP6Header_SIZE == message->length);
    Assert_true(ip->nextHeader == 17);
    Assert_true(!Bits_memcmp(ip->sourceAddr, fakeIp6ToGive, 16));
    called = 1;
    return 0;
}

int main()
{
    AddressCalc_addressForPublicKey(nodeCjdnsIp6, fakePubKey);
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Writer* w = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(w, alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);
    struct EventBase* eb = EventBase_new(alloc);

    struct IpTunnel* ipTun = IpTunnel_new(logger, eb, alloc, rand, NULL);
    struct Sockaddr_storage ip6ToGive;
    Sockaddr_parse("fd01:0101:0101:0101:0101:0101:0101:0101", &ip6ToGive);
    IpTunnel_allowConnection(fakePubKey, &ip6ToGive.addr, NULL, ipTun);

    struct Message* message;
    Message_STACK(message, 64, 512);
    message->alloc = alloc;

    const char* requestForAddresses =
        "d"
          "1:q" "21:IpTunnel_getAddresses"
          "4:txid" "4:abcd"
        "e";
    CString_strcpy((char*)message->bytes, requestForAddresses);
    message->length = CString_strlen(requestForAddresses);

    Message_shift(message, Headers_UDPHeader_SIZE, NULL);
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message->bytes;

    uh->srcPort_be = 0;
    uh->destPort_be = 0;
    uh->length_be = Endian_hostToBigEndian16(message->length - Headers_UDPHeader_SIZE);
    uint16_t* checksum = &uh->checksum_be;
    *checksum = 0;
    uint32_t length = message->length;

    Message_shift(message, Headers_IP6Header_SIZE, NULL);
    struct Headers_IP6Header* ip = (struct Headers_IP6Header*) message->bytes;

    ip->versionClassAndFlowLabel = 0;
    ip->flowLabelLow_be = 0;
    ip->payloadLength_be = Endian_hostToBigEndian16(length);
    ip->nextHeader = 17;
    ip->hopLimit = 255;
    Bits_memset(ip->sourceAddr, 0, 32);
    Headers_setIpVersion(ip);

    Message_shift(message, IpTunnel_PacketInfoHeader_SIZE, NULL);
    struct IpTunnel_PacketInfoHeader* pi = (struct IpTunnel_PacketInfoHeader*) message->bytes;

    Bits_memcpyConst(pi->nodeIp6Addr, nodeCjdnsIp6, 16);
    Bits_memcpyConst(pi->nodeKey, fakePubKey, 32);

    *checksum = Checksum_udpIp6(ip->sourceAddr, (uint8_t*) uh, length);

    ipTun->nodeInterface.receiveMessage = responseWithIpCallback;
    ipTun->nodeInterface.sendMessage(message, &ipTun->nodeInterface);
    Assert_true(called);
    called = 0;

    // Now create a message for someone else.
    Message_shift(message,
        Headers_UDPHeader_SIZE
        + Headers_IP6Header_SIZE
        + IpTunnel_PacketInfoHeader_SIZE,
        NULL);
    Bits_memcpyConst(ip->sourceAddr, fakeIp6ToGive, 16);
    // This can't be zero.
    Bits_memset(ip->destinationAddr, 1, 16);

    ipTun->tunInterface.receiveMessage = messageToTun;
    ipTun->nodeInterface.sendMessage(message, &ipTun->nodeInterface);
    Assert_true(called);

    Allocator_free(alloc);
    return 0;
}
