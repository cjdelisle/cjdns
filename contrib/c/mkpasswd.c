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
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"

#include <stdio.h>
#include <unistd.h>

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    uint8_t password[32];
    Random_base32(rand, password, 32);
    printf("%s\n", password);
    return 0;
}

