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
#if 1
int main(int argc, char** argv)
{
    return 0;
}
#else
#include "interface/tuntap/windows/NDPServer.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/Hex.h"
#include "util/events/Timeout.h"
#include "util/platform/Sockaddr.h"
#include "util/platform/netdev/NetDev.h"

static uint8_t receiveMessage(struct Message* msg, struct Iface* iface)
{
    struct Allocator* alloc = iface->receiverContext;
    if (msg->length < 20) {
        printf("runt\n");
        return 0;
    }
    // ethernet padding.
    Message_shift(msg, -2, NULL);

    uint8_t from[13];
    uint8_t to[13];
    Hex_encode(from, 13, msg->bytes, 6);
    Message_shift(msg, -6, NULL);
    Hex_encode(to, 13, msg->bytes, 6);
    Message_shift(msg, -6, NULL);

    uint8_t type[5];
    Hex_encode(type, 5, msg->bytes, 2);
    Message_shift(msg, -2, NULL);

    int subsubtype = -1;
    int subtype = -1;
//    int typeCode = -1;
    if (!Bits_memcmp(type, "86dd", 4)) {
        Bits_memcpy(type, "ipv6", 5);
        subtype = msg->bytes[6];
//        typeCode = 6;
        if (subtype == 58) {
          subsubtype = msg->bytes[40];
        }
    } else if (!Bits_memcmp(type, "0800", 4)) {
return 0;
        Bits_memcpy(type, "ipv4", 5);
        subtype = msg->bytes[9];
//        typeCode = 4;
    } else {
return 0;
    }
//       6000000000183aff0000000000000000000000000000000fff0200000000000000000001ff000018 870
//6000000000201101fd000000000000000000000000000001ff020000000000000000000000010003 eee914...
//6000000000083aff00000000000000000000000000000000ff020000000000000000000000000002 85007b
//6000000000203aff fd000000000000000000000000000001 ff0200000000000000000001ff000002 8700.

    int len = msg->length * 2 + 1;
    uint8_t* buff = Allocator_malloc(alloc, len + 2);
    Hex_encode(buff, len, msg->bytes, msg->length);
/*    if (typeCode == 6 && len > 86) {
        Bits_memmove(&buff[82], &buff[81], len - 81);
        Bits_memmove(&buff[49], &buff[48], len - 48);
        Bits_memmove(&buff[17], &buff[16], len - 16);
        buff[80] = buff[48] = buff[16] = ' ';
    }*/

    if (msg->length > 45) {
        Bits_memcpy(buff+86, "...", 4);
    }

    printf("[%s] [%s] [%s] [%02d] [%03d] [%s]\n", to, from, type, subtype, subsubtype, buff);
    return 0;
}

static void fail(void* vAlloc)
{
printf("Test failed\n");
    struct Allocator* alloc = vAlloc;
    Allocator_free(alloc);
}

int main(int argc, char** argv)
{
printf("init test");
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct EventBase* base = EventBase_new(alloc);

    char* ifName;
    struct Iface* iface = TAPInterface_new(NULL, &ifName, NULL, logger, base, alloc);
    struct NDPServer* ndp = NDPServer_new(iface, alloc);
    ndp->generic.receiveMessage = receiveMessage;
    ndp->generic.receiverContext = alloc;
    ndp->advertisePrefix[0] = 0xfd;
    ndp->prefixLen = 8;

    struct Sockaddr_storage ss;
    Assert_true(!Sockaddr_parse("fd00::1", &ss));
    NetDev_addAddress(ifName, &ss.addr, 8, logger, NULL);

    Timeout_setTimeout(fail, alloc, 10000, base, alloc);

    EventBase_beginLoop(base);
printf("Test ended\n");
    return 0;
}
#endif
