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
#ifndef CanaryAllocator_H
#define CanaryAllocator_H

#include "crypto/random/Random.h"
#include "memory/Allocator.h"

/**
 * Wrap an existing allocator with one which places canaries on either side of the allocations.
 * Canaries are checked when you call CanaryAllocator_check(), allocator->realloc(), or
 * allocator->free().
 * If either of the canary numbers has been changed (by a buffer overflow) then the program is
 * aborted. All children of a CanaryAllocator are also CanaryAllocators
 *
 * @param allocator allocator to wrap.
 * @param rand a source of canary numbers.
 * @return a new CanaryAllocator
 */
struct Allocator* CanaryAllocator_new(struct Allocator* alloc, struct Random* rand);

/**
 * Abort if any of the canaries have been written over.
 *
 * @param allocator the canary allocator.
 */
void CanaryAllocator_check(struct Allocator* allocator);

#endif
