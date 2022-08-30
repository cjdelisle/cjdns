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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "crypto/Key.h"
#include "crypto/random/Random.h"
#include "memory/MallocAllocator.h"
#include "crypto/AddressCalc.h"
#include "util/AddrTools.h"
#include "util/Base32.h"
#include "util/Hex.h"

#include <signal.h>
#include <stdio.h>

// This is invoked from makekeys.rs
int makekeys_main(int argc, char** argv);
int makekeys_main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    uint8_t privateKey[32];
    uint8_t publicKey[32];
    uint8_t publicKeyBase32[53];
    uint8_t ip[16];
    uint8_t hexPrivateKey[65];
    uint8_t printedIp[40];

#ifndef win32
    signal(SIGPIPE,SIG_DFL);
#endif

    for (;;) {
        Key_gen(ip, publicKey, privateKey, rand);
        Hex_encode(hexPrivateKey, 65, privateKey, 32);
        Base32_encode(publicKeyBase32, 53, publicKey, 32);
        AddrTools_printIp(printedIp, ip);
        printf("%s %s %s.k\n", hexPrivateKey, printedIp, publicKeyBase32);
        fflush(stdout);
    }
    return 0;
}

