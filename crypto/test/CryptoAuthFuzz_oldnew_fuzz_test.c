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
#include "crypto/test/TestCa.h"
#include "crypto/test/CryptoAuthFuzz.h"
#include "test/FuzzTest.h"

void* CJDNS_FUZZ_INIT(struct Allocator* alloc, struct Random* rand)
{
    return CryptoAuthFuzz_init(alloc, rand, TestCa_Config_OLD_NEW);
}
void CJDNS_FUZZ_MAIN(void* vctx, struct Message* fuzz)
{
    CryptoAuthFuzz_main(vctx, fuzz);
}