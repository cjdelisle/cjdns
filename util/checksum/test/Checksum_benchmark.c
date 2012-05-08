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
#include "crypto/Crypto.h"
#include "util/Assert.h"
#include "util/checksum/Checksum_impl0.h"
#include "util/checksum/Checksum_impl1.h"
#include "util/Time.h"

#define CYCLES 1000000
#define MAX_SIZE 8192

int main()
{
    uint8_t buffer[MAX_SIZE];
    uint16_t size;
    randombytes((uint8_t*) &size, 2);
    size %= MAX_SIZE;
    size &= ~1;
    randombytes(buffer, size);
    uint64_t startTime;
    uint64_t timeDiff;

    printf("Cycles: %u\n", CYCLES);
    for (int j = 0; j < 2; j++) {
        printf("bytes: %u\n", (int) size);
        startTime = Time_currentTimeMilliseconds(NULL);
        for (int i = 0; i < CYCLES; i++) {
            buffer[buffer[0]] = Checksum_impl0_complete(Checksum_impl0_step(buffer, size, 0));
        }
        timeDiff = Time_currentTimeMilliseconds(NULL) - startTime;
        printf("impl0: %dms.\n", (int) timeDiff);


        uint64_t startTime = Time_currentTimeMilliseconds(NULL);
        for (int i = 0; i < CYCLES; i++) {
            buffer[buffer[0]] = Checksum_impl1_complete(Checksum_impl1_step(buffer, size, 0));
        }
        timeDiff = Time_currentTimeMilliseconds(NULL) - startTime;
        printf("impl1: %dms.\n", (int) timeDiff);

        size--;
    }
}
