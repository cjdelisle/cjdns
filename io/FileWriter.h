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
#ifndef ARRAYWRITER_H
#define ARRAYWRITER_H

#include "Writer.h"
#include <memory/MemAllocator.h>
#include <stdio.h>

/**
 * Create a new Writer which writes to a file or stream.
 *
 * @param writeTo the file/stream which should be written to.
 * @param allocator the memory allocator to use for allocating the writer and context.
 */
struct Writer* FileWriter_new(FILE* writeTo, const struct MemAllocator* allocator);

#endif
