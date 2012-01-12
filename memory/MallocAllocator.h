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
#ifndef MALLOC_ALLOCATOR_H
#define MALLOC_ALLOCATOR_H

#include "memory/MemAllocator.h"

/**
 * Create a new MemAllocator which is a wrapper around malloc().
 *
 * @param sizeLimit the number of bytes which are allowed to be allocated by
 *                  this allocator or any of it's children before the program
 *                  will be halted with an error.
 */
struct MemAllocator* MallocAllocator_new(size_t sizeLimit);

#endif
