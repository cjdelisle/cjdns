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
#ifndef MallocAllocator_H
#define MallocAllocator_H

#include "memory/Allocator.h"

/**
 * Create a new Allocator which is a wrapper around malloc().
 *
 * @param sizeLimit the number of bytes which are allowed to be allocated by
 *                  this allocator or any of its children before the program
 *                  will be halted with an error.
 * @param identFile the file where this allocator was created.
 * @param identLine the line where this was called from.
 */
struct Allocator* MallocAllocator_newWithIdentity(unsigned long sizeLimit,
                                                  const char* identFile,
                                                  int identLine);
#define MallocAllocator_new(sl) \
    MallocAllocator_newWithIdentity((sl), __FILE__, __LINE__)

/**
 * Get the number of bytes allocated so far by this allocator,
 * all of its parents, and all of its children.
 *
 * @param allocator this *must* be a MallocAllocator, no checking is done!
 * @return the number of bytes which have been allocated so far.
 */
unsigned long MallocAllocator_bytesAllocated(struct Allocator* allocator);

#endif
