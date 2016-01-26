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
#ifndef SystemRandomSeed_H
#define SystemRandomSeed_H

#include "crypto/random/seed/RandomSeed.h"
#include "crypto/random/seed/SystemRandomSeed.h"
#include "util/Linker.h"
Linker_require("crypto/random/seed/SystemRandomSeed.c");

/**
 * Create a new randomseed with the default system providers as well as optionally more providers
 * additionalProviders is a null terminalted list.
 */
struct RandomSeed* SystemRandomSeed_new(RandomSeed_Provider* additionalProviders,
                                        int additionalProviderCount,
                                        struct Log* logger,
                                        struct Allocator* alloc);

#endif
