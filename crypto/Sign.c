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

#include "crypto_hash_sha512.h"

#include "crypto/sign/crypto_sign_ed25519.h"
#include "crypto/sign/ge.h"
#include "crypto/sign/sc.h"
Linker_require("./crypto/sign/fe_0.c");
Linker_require("./crypto/sign/fe_1.c");
Linker_require("./crypto/sign/fe_add.c");
Linker_require("./crypto/sign/fe_cmov.c");
Linker_require("./crypto/sign/fe_copy.c");
Linker_require("./crypto/sign/fe_frombytes.c");
Linker_require("./crypto/sign/fe_invert.c");
Linker_require("./crypto/sign/fe_isnegative.c");
Linker_require("./crypto/sign/fe_isnonzero.c");
Linker_require("./crypto/sign/fe_mul.c");
Linker_require("./crypto/sign/fe_neg.c");
Linker_require("./crypto/sign/fe_pow22523.c");
Linker_require("./crypto/sign/fe_sq.c");
Linker_require("./crypto/sign/fe_sq2.c");
Linker_require("./crypto/sign/fe_sub.c");
Linker_require("./crypto/sign/fe_tobytes.c");
Linker_require("./crypto/sign/ge_add.c");
Linker_require("./crypto/sign/ge_double_scalarmult.c");
Linker_require("./crypto/sign/ge_frombytes.c");
Linker_require("./crypto/sign/ge_madd.c");
Linker_require("./crypto/sign/ge_msub.c");
Linker_require("./crypto/sign/ge_p1p1_to_p2.c");
Linker_require("./crypto/sign/ge_p1p1_to_p3.c");
Linker_require("./crypto/sign/ge_p2_0.c");
Linker_require("./crypto/sign/ge_p2_dbl.c");
Linker_require("./crypto/sign/ge_p3_0.c");
Linker_require("./crypto/sign/ge_p3_dbl.c");
Linker_require("./crypto/sign/ge_p3_to_cached.c");
Linker_require("./crypto/sign/ge_p3_to_p2.c");
Linker_require("./crypto/sign/ge_p3_tobytes.c");
Linker_require("./crypto/sign/ge_precomp_0.c");
Linker_require("./crypto/sign/ge_scalarmult_base.c");
Linker_require("./crypto/sign/ge_sub.c");
Linker_require("./crypto/sign/ge_tobytes.c");
Linker_require("./crypto/sign/sc_muladd.c");
Linker_require("./crypto/sign/sc_reduce.c");
Linker_require("./crypto/sign/open.c");

// This is fairly streight forward, we're taking a curve25519 private key and
// interpreting it as an ed25519 key. This works in conjunction with the public
// key converter Sign_publicSigningKeyToCurve25519() which is able to re-derive
// the encryption key from a public signing key.
void Sign_signingKeyPairFromCurve25519(uint8_t keypairOut[64], uint8_t secretCryptoKey[32])
{
    Bits_memcpy(keypairOut, secretCryptoKey, 32);

    // The lower 3 bits are always cleared in both curve25519 and ed25519 keys before use
    // see: https://crypto.stackexchange.com/a/12614
    keypairOut[0] &= 248;

    // You will notice that ed25519 uses &= 63 (setting bit number 354 to zero) while
    // curve25519 scalarmult uses &= 127, allowing bit number 254 to be potentially 1,
    // this might look as though the keys are different but since the next line flags bit
    // number 254 always to 1, there is no difference here between the way curve25519
    // implementations and ed25519 implementations work.
    keypairOut[31] &= 63;

    // Bit number 254 is always set in both curve25519 and ed25519 keys before use
    // see: https://crypto.stackexchange.com/a/11818
    keypairOut[31] |= 64;

    // This is just doing the same thing as vanilla ed25519 crypto_sign_keypair()
    // computation with the exception that we don't hash the private key before
    // computation.
    ge_p3 A;
    ge_scalarmult_base(&A, keypairOut);
    ge_p3_tobytes(&keypairOut[32], &A);
}

void Sign_publicKeyFromKeyPair(uint8_t publicSigningKey[32], uint8_t keyPair[64])
{
    Bits_memcpy(publicSigningKey, &keyPair[32], 32);
}

// This function is here because cjdns traditionally did not include signing, it only
// has a key for encryption so when signing was implemented, in order not to break
// everyone who has a cjdroute.conf file already, we needed to be able to convert
// the encryption key to a signing key.
// That in itself is fairly easy, and considered to be safe, but unfortunately nacl
// and libsodium ed25519 implementations hash the private key before each use, making
// it impossible to use our private key derived from a curve25519 encryption private key.
//
// The reason why nacl hashes the private key is to expand 32 bytes of entropy into 64
// bytes, half of which is used as the actual signing key and half of which is used as
// a secret random value which when combined with the hash of the message creates a
// value r that is unpredictable and different for each message signed. It's important
// to note that if r is the same for two different messages then there is a way for an
// attacker to mathmatically derive the private key.
//
// What we do here instead is take the hash of the actual private key used to sign with
// plus some random "belt and suspenders" bytes. Generally speaking, we should consider
// that there is no more dissernable relationship between the public key and the sha512
// of the private key than there is between the public key and the second half of the
// same sha512 which produces the private key, but this is walking off of the beaten path
// and throwing in a little bit of random each message should not make the situation any
// worse.
void Sign_signMsg(uint8_t keyPair[64], struct Message* msg, struct Random* rand)
{
    // az is set to the secret key followed by another secret value
    // which since we don't have a secret seed in this algorithm is just the
    // hash of the secret key and 32 bytes of random
    uint8_t az[64];
    uint8_t r[64];
    ge_p3 R;
    uint8_t hram[64];

    // First we need to derive a unique random value, we'll do this by hashing the secret
    // key plus 32 bytes of random, whereas crypto_sign() achieves this by taking half of
    // the hash of the secret key that is input to it.
    Bits_memcpy(az, keyPair, 32);
    Random_bytes(rand, &az[32], 32);
    crypto_hash_sha512(az,az,64);

    // Ok, now az contains 64 bytes of unique random value, the upper 32 bytes needs to
    // be set to the actual secret key that we're going to use for signing.
    Bits_memcpy(az, keyPair, 32);

    // The reason for these numbers being masked off is explained above, but this is no
    // different from crypto_sign()
    az[0] &= 248;
    az[31] &= 63;
    az[31] |= 64;

    // hash message + secret number, this is the same as crypto_sign()
    // If there isn't enough space in the message, we abort the process
    Er_assert(Message_epush(msg, &az[32], 32));
    crypto_hash_sha512(r, msg->bytes, msg->length);

    // Replace secret number with public key, this is the same as crypto_sign()
    Bits_memcpy(msg->bytes, &keyPair[32], 32);

    // Now we scalar multiply the hash of the message + unique secret and push that
    // to the message, nothing different from crypto_sign()
    sc_reduce(r);
    ge_scalarmult_base(&R,r);
    // If there isn't enough space in the message, we abort the process
    Er_assert(Message_eshift(msg, 32));
    ge_p3_tobytes(msg->bytes,&R);

    // This final step is the same as crypto_sign()
    // Overwrite the public key which the verifier will replace in order to recompute
    // the hash.
    crypto_hash_sha512(hram, msg->bytes, msg->length);
    sc_reduce(hram);
    sc_muladd(&msg->bytes[32], hram, az, r);
}

// For verify, we're just using the normal sign_open() function, nothing special here.
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
    Er_assert(Message_epop(msg, NULL, 64));
    return 0;
}

// This is a copy of libsodium's implementation:
// https://github.com/jedisct1/libsodium/blob/eae4add8de435a7fad08eab4f6e7cbfa9209a692/
//    src/libsodium/crypto_sign/ed25519/ref10/keypair.c#L45
// Note that in newer versions Libsodium added a checks
// * ge25519_has_small_order - refusing signatures made with weak public keys
// * ge25519_is_on_main_subgroup - refusing signatures from keys on different subgroups
// These additions are specific to Libsodium, they not in the original NACL ed25519
// implementation, nor in tweetnacl.
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
