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
#ifndef AddressCalc_H
#define AddressCalc_H

#include "util/Linker.h"
Linker_require("crypto/AddressCalc.c");

#include <stdint.h>
#include <stdbool.h>

/**
 * Check if an address is valid given the IPv6
 *
 * @return true if the IPv6 is a valid cjdns address.
 */

bool AddressCalc_validAddress(const uint8_t address[16]);

/**
 * Calculate a cjdns IPv6 address for a public key.
 *
 * @param addressOut put the address here.
 * @param key the 256 bit curve25519 public key.
 */
bool AddressCalc_addressForPublicKey(uint8_t addressOut[16], const uint8_t key[32]);

#endif
