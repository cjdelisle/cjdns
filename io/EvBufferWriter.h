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
#ifndef EV_BUFFER_WRITER_H
#define EV_BUFFER_WRITER_H

#include "Writer.h"
#include "memory/Allocator.h"
#include <event2/buffer.h>

/**
 * Create a new Writer which writes to a libevent buffer.
 *
 * @param buffer the libevent buffer to write to.
 * @param allocator the memory allocator to use for allocating the writer and context.
 */
struct Writer* EvBufferWriter_new(struct evbuffer* buffer, const struct Allocator* allocator);

#endif
