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

#include "node_build/dependencies/cnacl/crypto_sign/ed25519/ref10/ge.h"
#include "node_build/dependencies/cnacl/crypto_sign/ed25519/ref10/sc.h"
#include "crypto_hash_sha512.h"
#include "crypto_sign_ed25519.h"

#if crypto_sign_ed25519_open != crypto_sign_ed25519_ref10_open
    Assert_compileTime(crypto_sign_ed25519_open == crypto_sign_ed25519_ref10_open);
#endif

void Sign_signingKeyPairFromCurve25519(uint8_t keypairOut[64], uint8_t secretCryptoKey[32])
{
    Bits_memcpy(keypairOut, secretCryptoKey, 32);
    keypairOut[0] &= 248;
    keypairOut[31] &= 63;
    keypairOut[31] |= 64;
    ge_p3 A;
    ge_scalarmult_base(&A, keypairOut);
    ge_p3_tobytes(&keypairOut[32], &A);
}

void Sign_publicKeyFromKeyPair(uint8_t publicSigningKey[32], uint8_t keyPair[64])
{
    Bits_memcpy(publicSigningKey, &keyPair[32], 32);
}

void Sign_signMsg(uint8_t keyPair[64], struct Message* msg, struct Random* rand)
{
    // az is set to the secret key followed by another secret value
    // which since we don't have a secret seed in this algorithm is just the
    // hash of the secret key and 32 bytes of random
    uint8_t az[64];
    uint8_t r[64];
    ge_p3 R;
    uint8_t hram[64];

    Bits_memcpy(az, keyPair, 32);
    Random_bytes(rand, &az[32], 32);
    crypto_hash_sha512(az,az,64);
    Bits_memcpy(az, keyPair, 32);
    az[0] &= 248;
    az[31] &= 63;
    az[31] |= 64;

    // hash message + secret number
    Message_push(msg, &az[32], 32, NULL);
    crypto_hash_sha512(r, msg->bytes, msg->length);

    // Replace secret number with public key
    Bits_memcpy(msg->bytes, &keyPair[32], 32);

    // push pointMul(r) to message
    sc_reduce(r);
    ge_scalarmult_base(&R,r);
    Message_shift(msg, 32, NULL);
    ge_p3_tobytes(msg->bytes,&R);

    crypto_hash_sha512(hram, msg->bytes, msg->length);
    sc_reduce(hram);
    sc_muladd(&msg->bytes[32], hram, az, r);
}

int Sign_verifyMsg(uint8_t publicSigningKey[32], struct Message* msg)
{
    if (msg->length < 64) { return -1; }
    struct Allocator* alloc = Allocator_child(msg->alloc);
    uint8_t* buff = Allocator_malloc(alloc, msg->length);
    unsigned long long ml = msg->length;
    int ret = crypto_sign_ed25519_open(buff, &ml, msg->bytes, msg->length, publicSigningKey);
    Allocator_free(alloc);
    if (ret) {
        return -1;
    }
    Message_pop(msg, NULL, 64, NULL);
    return 0;
}

int Sign_publicSigningKeyToCurve25519(uint8_t curve25519keyOut[32], uint8_t publicSigningKey[32])
{
    ge_p3 A;
    fe    x;
    fe    one_minus_y;

    if (ge_frombytes_negate_vartime(&A, publicSigningKey) != 0) {
        return -1;
    }
    fe_1(one_minus_y);
    fe_sub(one_minus_y, one_minus_y, A.Y);
    fe_invert(one_minus_y, one_minus_y);
    fe_1(x);
    fe_add(x, x, A.Y);
    fe_mul(x, x, one_minus_y);
    fe_tobytes(curve25519keyOut, x);

    return 0;
}
