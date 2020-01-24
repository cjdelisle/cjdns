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
#ifndef AddressCalc_H
#define AddressCalc_H

#include "util/Linker.h"
Linker_require("crypto/AddressCalc.c");

#include <stdint.h>
#include <stdbool.h>

#ifdef ADDRESS_PREFIX
#define AddressCalc_ADDRESS_PREFIX ADDRESS_PREFIX
#else
#define AddressCalc_ADDRESS_PREFIX 0xfc
#endif
#ifdef ADDRESS_PREFIX_BITS
#define AddressCalc_ADDRESS_PREFIX_BITS ADDRESS_PREFIX_BITS
#else
#define AddressCalc_ADDRESS_PREFIX_BITS 8
#endif

#if AddressCalc_ADDRESS_PREFIX_BITS > 64
#error "ADDRESS_PREFIX_BITS may not be > 64."
#endif
#if AddressCalc_ADDRESS_PREFIX_BITS <= 0
#error "ADDRESS_PREFIX_BITS may not be <= 0."
#endif
#if AddressCalc_ADDRESS_PREFIX >= (1 << AddressCalc_ADDRESS_PREFIX_BITS)
#error "ADDRESS_PREFIX may not be >= 2^ADDRESS_PREFIX_BITS."
#endif

/**
 * Check if an address is valid given the IPv6
 *
 * @return true if the IPv6 is a valid cjdns address.
 */

bool AddressCalc_validAddress(const uint8_t address[16]);

/**
 * Edits the prefix of the given address to make it a valid cjdns address.
 */

void AddressCalc_makeValidAddress(uint8_t address[16]);

/**
 * Calculate a cjdns IPv6 address for a public key.
 *
 * @param addressOut put the address here.
 * @param key the 256 bit curve25519 public key.
 * @return true if the IPv6 is a valid cjdns address.
 */
bool AddressCalc_addressForPublicKey(uint8_t addressOut[16], const uint8_t key[32]);

#endif
