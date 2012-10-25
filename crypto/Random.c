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
#include "crypto/Random.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Base32.h"

#include <event2/util.h>

static char Random_IDENTITY;

struct Random {
    char* identity;
};

static void randomBytes(struct Random* rand, uint8_t* location, uint64_t count)
{
    evutil_secure_rng_get_bytes(location, count);
}

void Random_bytes(struct Random* rand, uint8_t* location, uint64_t count)
{
    Assert_true(rand->identity == &Random_IDENTITY);
    randomBytes(rand, location, count);
}

void Random_base32(struct Random* rand, uint8_t* output, uint32_t length)
{
    Assert_true(rand->identity == &Random_IDENTITY);
    uint32_t index = 0;
    for (;;) {
        uint8_t bin[16];
        randomBytes(rand, bin, 16);
        int ret = Base32_encode(&output[index], length - index, bin, 16);
        if (ret == Base32_TOO_BIG || index + ret == length) {
            break;
        }
        index += ret;
    }
    output[length - 1] = '\0';
}

struct Random* Random_new(struct Allocator* alloc, struct Except* eh)
{
    if (evutil_secure_rng_init() != 0) {
        Except_raise(eh, Random_new_INIT_FAILED,
                     "Unable to initialize secure random number generator");
    }
    return alloc->clone(sizeof(struct Random), alloc, &(struct Random) {
        .identity = &Random_IDENTITY
    });
}
