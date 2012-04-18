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
#include "dht/Ducttape.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"

#include "test/TestFramework.h"

#include <stdio.h>

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

    struct Ducttape* dt = TestFramework_setUp();

    struct Allocator* allocator = MallocAllocator_new(85000);
    uint16_t buffLen = sizeof(struct Ducttape_IncomingForMe) + 8 + strlen(evilBenc);
    uint8_t* buff = allocator->malloc(buffLen, allocator);

    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) (buff + Headers_SwitchHeader_SIZE);
    uint8_t* herPublicKey = (uint8_t*) "0123456789abcdefghijklmnopqrstuv";
    AddressCalc_addressForPublicKey(ip6->sourceAddr, herPublicKey);

    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) (ip6 + 1);
    ip6->hopLimit = 0;
    ip6->nextHeader = 17;
    udp->sourceAndDestPorts = 0;
    udp->length_be = Endian_hostToBigEndian16(strlen(evilBenc));

    strcpy((char*)(udp + 1), evilBenc);

    struct Message m = { .bytes = buff, .length = buffLen, .padding = 0 };

    Ducttape_injectIncomingForMe(&m, dt, herPublicKey);
}
