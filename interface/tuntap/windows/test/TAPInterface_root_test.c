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

#include "interface/tuntap/windows/TAPInterface.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/events/EventBase.h"
#include "interface/Interface.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/Hex.h"
#include "util/events/Timeout.h"
#include "util/platform/Sockaddr.h"
#include "util/platform/netdev/NetDev.h"

uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct Allocator* alloc = iface->receiverContext;
    if (msg->length < 18) {
        printf("runt\n");
        return 0;
    }

    uint8_t from[13];
    uint8_t to[13];
    Hex_encode(from, 13, msg->bytes, 6);
    Message_shift(msg, -6);
    Hex_encode(to, 13, msg->bytes, 6);
    Message_shift(msg, -6);

    uint8_t type[5];
    Hex_encode(type, 5, msg->bytes, 2);
    Message_shift(msg, -2);

    int subtype = -1;
    if (!Bits_memcmp(type, "86dd", 4)) {
        Bits_memcpyConst(type, "ipv6", 5);
        subtype = msg->bytes[6];
    } else if (!Bits_memcmp(type, "0800", 4)) {
        Bits_memcpyConst(type, "ipv4", 5);
        subtype = msg->bytes[9];
    }

    uint8_t* buff = Allocator_malloc(alloc, msg->length * 2 + 1);
    Hex_encode(buff, msg->length * 2 + 1, msg->bytes, msg->length);

    if (msg->length > 45) {
        Bits_memcpy(buff+86, "...", 4);
    }

    printf("message [%s] [%s] [%s] [%d] [%s]\n", to, from, type, subtype, buff);
    return 0;
}

void fail(void* vAlloc)
{
printf("Test failed\n");
    struct Allocator* alloc = vAlloc;
    Allocator_free(alloc);
}

int main()
{
printf("init test");
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct EventBase* base = EventBase_new(alloc);

    char* ifName;
    struct Interface* iface = TAPInterface_new(NULL, &ifName, NULL, logger, base, alloc);
    iface->receiveMessage = receiveMessage;
    iface->receiverContext = alloc;

    struct Sockaddr_storage ss;
    Assert_true(!Sockaddr_parse("fd00::1", &ss));
    NetDev_addAddress(ifName, &ss.addr, 8, logger, NULL);

    Timeout_setTimeout(fail, alloc, 10000, base, alloc);

    EventBase_beginLoop(base);
printf("Test ended\n");
}
