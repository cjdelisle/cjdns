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
#include "crypto/AddressCalc.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "net/Ducttape.h"
#include "util/CString.h"

#include "test/TestFramework.h"

#include <stdio.h>

#define PADDING 512

int main()
{
    // This is valid benc but it takes over 75k of memory (on an amd64)
    // to build a structure representing it.
    char* evilBenc =
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "d1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:ad1:a"
        "3:lol"
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"
        "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";

    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct Ducttape* dt =
        TestFramework_setUp("0123456789abcdefghijklmnopqrstuv", alloc, NULL)->ducttape;

    // This has to be limited because we are checking for an OOM issue.
    struct Allocator* allocator = MallocAllocator_new(85000);
    uint16_t buffLen = sizeof(struct Ducttape_IncomingForMe) + 8 + CString_strlen(evilBenc);
    uint8_t* buff = Allocator_calloc(allocator, buffLen + PADDING, 1);

    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) (buff + Headers_SwitchHeader_SIZE);
    uint8_t* herPublicKey = (uint8_t*) "0123456789abcdefghijklmnopqrstuv";
    AddressCalc_addressForPublicKey(ip6->sourceAddr, herPublicKey);

    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) (ip6 + 1);
    ip6->hopLimit = 0;
    ip6->nextHeader = 17;
    udp->srcPort_be = 0;
    udp->destPort_be = 0;
    udp->length_be = Endian_hostToBigEndian16(CString_strlen(evilBenc));

    CString_strncpy((char*)(udp + 1), evilBenc, CString_strlen(evilBenc));

    struct Message m = { .bytes = buff+PADDING, .length = buffLen, .padding = PADDING };

    Ducttape_injectIncomingForMe(&m, dt, herPublicKey);

    Allocator_free(alloc);
    Allocator_free(allocator);
    return 0;
}
