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
#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("memory/MallocAllocator.c");

/**
 * Create a new Allocator which is a wrapper around malloc().
 *
 * @param sizeLimit the number of bytes which are allowed to be allocated by
 *                  this allocator or any of its children before the program
 *                  will be halted with an error.
 */
struct Allocator* MallocAllocator__new(unsigned long sizeLimit, const char* file, int line);
#define MallocAllocator_new(sl) MallocAllocator__new((sl),Gcc_SHORT_FILE,Gcc_LINE)

#endif
