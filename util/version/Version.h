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
#ifndef Version_H
#define Version_H

#include "util/Linker.h"
Linker_require("util/version/Version.c")

#include <stdint.h>
#include <stdbool.h>

/**
 * The current protocol version.
 */
#define Version_CURRENT_PROTOCOL 22
#define Version_21_COMPAT
#define Version_20_COMPAT

#define Version_MINIMUM_COMPATIBLE 20
#define Version_DEFAULT_ASSUMPTION 20

// NOTE: The version table has been moved to Version.c

static inline bool Version_compatibleWithCurrent(uint32_t version) {
    return version > Version_MINIMUM_COMPATIBLE;
}

/**
 * Check the compatibility matrix and return whether two versions are compatible.
 * If a version is not listed on the table, the highest version on the table is
 * substituted for it but if the return value is yes, it is changed to maybe.
 *
 * @param version1 the first version
 * @param version2 the second version
 */
bool Version_isCompatible(uint32_t one, uint32_t two);

#endif
