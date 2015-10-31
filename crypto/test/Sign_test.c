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
#include "crypto/Sign.h"
#include "crypto/random/Random.h"
#include "memory/MallocAllocator.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/log/FileWriterLog.h"

#include "crypto_scalarmult_curve25519.h"

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1048576);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);

    uint8_t curve25519private[32];
    Random_bytes(rand, curve25519private, 32);
    uint8_t curve25519public[32];
    crypto_scalarmult_curve25519_base(curve25519public, curve25519private);

    uint8_t signingKeyPair[64];
    Sign_signingKeyPairFromCurve25519(signingKeyPair, curve25519private);
    struct Message* msg = Message_new(0, 512, alloc);
    Message_push(msg, "hello world", 12, NULL);
    Sign_signMsg(signingKeyPair, msg, rand);

    uint8_t curve25519publicB[32];
    Assert_true(!Sign_verifyMsg(&signingKeyPair[32], msg));
    Assert_true(!Sign_publicSigningKeyToCurve25519(curve25519publicB, &signingKeyPair[32]));
    Assert_true(!Bits_memcmp(curve25519publicB, curve25519public, 32));
    return 0;
}
