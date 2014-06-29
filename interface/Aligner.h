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
#ifndef Aligner_H
#define Aligner_H
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("interface/Aligner.c")

struct Aligner
{
    struct Interface generic;
};

struct Aligner* Aligner_new(struct Interface* external,
                            struct Allocator* alloc,
                            uint32_t alignmentBytes);

#endif
