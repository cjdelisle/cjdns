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
#include "memory/MallocAllocator.h"

#include <stdlib.h>

static void* provideMemory(void* vNULL,
                           struct Allocator_Allocation* original,
                           unsigned long size,
                           struct Allocator* group)
{
    if (original == NULL) {
        if (size == 0) {
            return NULL;
        }
        return malloc(size);
    }

    if (size == 0) {
        free(original);
        return NULL;
    }

    return realloc(original, size);
}

struct Allocator* MallocAllocator__new(unsigned long sizeLimit, const char* file, int line)
{
    return Allocator_new(sizeLimit, provideMemory, NULL, file, line);
}
