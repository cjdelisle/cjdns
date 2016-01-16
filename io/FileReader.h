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
#ifndef FileReader_H
#define FileReader_H

#include "io/Reader.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("io/FileReader.c");

#include <stdio.h>

/**
 * Create a new Reader which reads a file or stream.
 *
 * @param toRead the file to read from.
 * @param allocator the memory allocator to use for allocating the reader and context.
 */
struct Reader* FileReader_new(FILE* toRead,
                              struct Allocator* allocator);

#endif
