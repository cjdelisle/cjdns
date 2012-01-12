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
#ifndef EXPORTS_H
#define EXPORTS_H

#include <stdint.h>

#include "crypto/CryptoAuth_struct.h"
#include "wire/Message.h"

void Exports_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

int Exports_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

uint8_t Exports_encryptHandshake(struct Message* message, struct Wrapper* wrapper);

void Exports_receiveMessage(struct Message* received, struct Interface* interface);

#endif
