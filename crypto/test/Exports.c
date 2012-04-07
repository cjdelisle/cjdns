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
#define CryptoAuth_addUser Export_CryptoAuth_addUser
#define CryptoAuth_getUser Export_CryptoAuth_getUser
#define CryptoAuth_new Export_CryptoAuth_new
#define CryptoAuth_wrapInterface Export_CryptoAuth_wrapInterface
#define CryptoAuth_setAuth Export_CryptoAuth_setAuth
#define CryptoAuth_getPublicKey Export_CryptoAuth_getPublicKey
#define CryptoAuth_getHerPublicKey Export_CryptoAuth_getHerPublicKey
#define CryptoAuth_getSession Exports_CryptoAuth_getSession
#define CryptoAuth_reset Exports_CryptoAuth_reset
#define CryptoAuth_flushUsers Exports_CryptoAuth_flushUsers
#define CryptoAuth_getState Exports_CryptoAuth_getState

#include "crypto/CryptoAuth.c"

/**
 * This is for allowing the unit testing of functions internal to CryptoAuth
 * while still allowing them to be inlined in larger functions.
 */


void Exports_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32])
{
    encryptRndNonce(nonce, msg, secret);
}

int Exports_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32])
{
    return decryptRndNonce(nonce, msg, secret);
}

uint8_t Exports_encryptHandshake(struct Message* message, struct Wrapper* wrapper)
{
    return encryptHandshake(message, wrapper);
}

void Exports_receiveMessage(struct Message* received, struct Interface* interface)
{
    receiveMessage(received, interface);
}
