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
#include "crypto/random/Random.h"
#include "wire/Message.h"
#include "util/Base10.h"
#include "util/Assert.h"
#include "util/CString.h"

#include <stdio.h>

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    struct Message* msg = Message_new(0, 32, alloc);

    uint8_t buff[32] = {0};
    for (int i = 0; i < 1000; i++) {
        // zero it
        Message_reset(msg);
        Message_push(msg, NULL, 32, NULL);
        Message_reset(msg);

        long long num = Random_int64(rand);
        Base10_write(msg, num, NULL);

        snprintf(buff, 31, "%lld", num);

        Assert_true(msg->length == (int)CString_strlen(buff));
        Assert_true(!Bits_memcmp(msg->bytes, buff, msg->length));

        int64_t read = Base10_read(msg, NULL);
        Assert_true(read == num);
        Assert_true(msg->length == 0);
    }

    Allocator_free(alloc);
    return 0;
}
