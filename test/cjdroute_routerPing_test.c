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
#include "util/Base32.h"
#include "util/Checksum.h"
#include "util/CString.h"
#include "test/TestFramework.h"
#include "net/Ducttape_pvt.h"

#include <stdio.h>

#define PADDING 512

static uint8_t catchResponse(struct Message* msg, struct Interface* iface)
{
    iface->receiverContext = msg;
    return 0;
}

int main()
{
    return 0; // TODO(cjd): make this work again

    char* pingBenc = "d1:q4:ping4:txid4:abcd1:pi2ee";
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct TestFramework* tf = TestFramework_setUp("0123456789abcdefghijklmnopqrstuv", alloc, NULL);
    struct Ducttape_pvt* dt = (struct Ducttape_pvt*) tf->ducttape;

    struct Allocator* allocator = MallocAllocator_new(85000);
    uint16_t buffLen = sizeof(struct Ducttape_IncomingForMe) + 8 + CString_strlen(pingBenc);
    uint8_t* buff = Allocator_calloc(allocator, buffLen+PADDING, 1);
    struct Headers_SwitchHeader* sh = (struct Headers_SwitchHeader*) (buff + PADDING);
    sh->label_be = Endian_hostToBigEndian64(4);
    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) &sh[1];

    uint8_t herPublicKey[32];
    Base32_decode(herPublicKey, 32,
                  (uint8_t*) "0z5tscp8td1sc6cv4htp7jbls79ltqxw9pbg190x0kbm1lguqtx0", 52);
    AddressCalc_addressForPublicKey(ip6->sourceAddr, herPublicKey);

    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) &ip6[1];
    ip6->hopLimit = 0;
    ip6->nextHeader = 17;
    udp->srcPort_be = 0;
    udp->destPort_be = 0;
    udp->length_be = Endian_hostToBigEndian16(CString_strlen(pingBenc));

    CString_strncpy((char*)(udp + 1), pingBenc, CString_strlen(pingBenc));

    dt->switchInterface.receiveMessage = catchResponse;
    dt->switchInterface.receiverContext = NULL;

    // bad checksum
    udp->checksum_be = 1;
    struct Message m = {
        .bytes = buff+PADDING,
        .length = buffLen,
        .padding = PADDING,
        .alloc = alloc
    };
    Ducttape_injectIncomingForMe(&m, &dt->pub, herPublicKey);
    Assert_true(!dt->switchInterface.receiverContext);

    // zero checksum
    udp->checksum_be = 0;
    struct Message m2 = {
        .bytes = buff+PADDING,
        .length = buffLen,
        .padding = PADDING,
        .alloc = alloc
    };
    Ducttape_injectIncomingForMe(&m2, &dt->pub, herPublicKey);
    Assert_true(!dt->switchInterface.receiverContext);

    // good checksum
    udp->checksum_be =
        Checksum_udpIp6(ip6->sourceAddr,
                        (uint8_t*)udp,
                        CString_strlen(pingBenc) + Headers_UDPHeader_SIZE);
    struct Message m3 = {
        .bytes = buff+PADDING,
        .length = buffLen,
        .padding = PADDING,
        .alloc = alloc
    };
    Ducttape_injectIncomingForMe(&m3, &dt->pub, herPublicKey);
    Assert_true(dt->switchInterface.receiverContext);

    Allocator_free(alloc);
    Allocator_free(allocator);
    return 0;
}
