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
#include "crypto/Sign.h"
#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/log/FileWriterLog.h"

#include <sodium/crypto_scalarmult_curve25519.h>

int main()
{
    struct Allocator* alloc = Allocator_new(1048576);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct Random* rand = NULL;
    Err_assert(Random_new(&rand, alloc, logger));

    uint8_t curve25519private[32];
    Random_bytes(rand, curve25519private, 32);
    uint8_t curve25519public[32];
    crypto_scalarmult_curve25519_base(curve25519public, curve25519private);

    uint8_t signingKeyPair[64];
    Sign_signingKeyPairFromCurve25519(signingKeyPair, curve25519private);
    Message_t* msg = Message_new(0, 512, alloc);
    Err_assert(Message_epush(msg, "hello world", 12));
    Sign_signMsg(signingKeyPair, msg, rand);

    uint8_t curve25519publicB[32];
    Assert_true(!Sign_verifyMsg(&signingKeyPair[32], msg));
    Assert_true(!Sign_publicSigningKeyToCurve25519(curve25519publicB, &signingKeyPair[32]));
    Assert_true(!Bits_memcmp(curve25519publicB, curve25519public, 32));

    Allocator_free(alloc);
    return 0;
}
