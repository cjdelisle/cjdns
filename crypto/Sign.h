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
#ifndef Sign_H
#define Sign_H

#include "crypto/random/Random.h"
#include "wire/Message.h"
#include "util/Linker.h"
Linker_require("crypto/Sign.c");

void Sign_signingKeyPairFromCurve25519(uint8_t keypairOut[64], uint8_t secretCryptoKey[32]);

/** Pushes 64 bit sig to message. */
void Sign_signMsg(uint8_t keyPair[64], struct Message* msg, struct Random* rand);

/** returns 0 and pops sig if signature check passes, zeros message content if it fails! */
int Sign_verifyMsg(uint8_t publicSigningKey[32], struct Message* msg);

int Sign_publicSigningKeyToCurve25519(uint8_t curve25519keyOut[32], uint8_t publicSigningKey[32]);

void Sign_publicKeyFromKeyPair(uint8_t publicKey[32], uint8_t keyPair[64]);

#endif
