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
#ifndef Crypto_H
#define Crypto_H

#include "wire/Message.h"

#include <stdint.h>

/**
 * Initialize the random number generator.
 * This must be called before doing anything else.
 */
void Crypto_init();

void randombytes(unsigned char* buffer, unsigned long long size); // CHECKFILES_IGNORE

/**
 * Get random Base32 text, great for password material.
 *
 * @param output the buffer to write the output to.
 * @param length the number of bytes to write.
 */
void Crypto_randomBase32(uint8_t* output, uint32_t length);

#endif
