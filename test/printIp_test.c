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

#include "crypto/random/Random.h"
#include "memory/MallocAllocator.h"
#include "util/log/FileWriterLog.h"
#include "util/log/Log.h"
#include "util/AddrTools.h"
#include "util/Assert.h"
#include "util/Bits.h"

#include <stdio.h>

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct Random* rand = Random_new(alloc, NULL, NULL);
    struct Log* log = FileWriterLog_new(stdout, alloc);

    uint8_t ip[16];
    uint8_t printedIp[40];
    uint8_t printedShortIp[40];
    uint8_t ipFromFull[16];
    uint8_t ipFromShort[16];

    for (int i = 0; i < 1024; ++i) {
        Random_bytes(rand, ip, 16);

        for (int j = 0; j < 16; j++) {
            // make the random result have lots of zeros since that's what we're looking for.
            ip[j] = (ip[j] % 2) ? 0 : ip[j];
        }

        AddrTools_printIp(printedIp, ip);
        AddrTools_printShortIp(printedShortIp, ip);
        //printf("%s\n%s\n\n", printedIp, printedShortIp);

        AddrTools_parseIp(ipFromFull, printedIp);
        AddrTools_parseIp(ipFromShort, printedShortIp);

        Log_debug(log, "print/parse %s", printedIp);

        Assert_true(0 == Bits_memcmp(ip, ipFromFull, 16));
        Assert_true(0 == Bits_memcmp(ipFromFull, ipFromShort, 16));
    }

    return 0;
}
