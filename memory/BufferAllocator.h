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
#ifndef BufferAllocator_H
#define BufferAllocator_H

#include "Allocator.h"

/**
 * Create a new Allocator which allocates from to a user supplied buffer.
 * This allocator will reset the pointer to the beginning of the buffer when
 * free() is called on it, it is up to the user to free their buffer.
 * allocator->child(allocator) always returns NULL.
 *
 * @param buffer an array to write to.
 * @param length the size of the array. If more is written than this length,
 *               further allocations will fail and return NULL.
 */
struct Allocator* BufferAllocator_new(void* buffer, size_t length);

/**
 * @param bufferAllocator the buffer allocator to set this on.
 * @param electronicThumb in an out-of-memory event, this function will be called just
 *                        before the process is aborted.
 * @param towel a context which electronicThumb will be called with.
 */
void BufferAllocator_onOOM(const struct Allocator* bufferAllocator,
                           void (* electronicThumb)(void* towel),
                           void* towel);

#endif
