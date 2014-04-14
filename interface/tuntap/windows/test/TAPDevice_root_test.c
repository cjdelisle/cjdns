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
#ifndef win32
int main(int argc, char** argv)
{
    return 0;
}
#else
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "interface/tuntap/windows/TAPDevice.h"
#include "util/Assert.h"
#include "test/RootTest.h"

#include <stdio.h>

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct TAPDevice* dev = TAPDevice_find(NULL, NULL, alloc);
    Assert_true(dev && dev->name && dev->path);
    printf("name [%s] path [%s]\n", dev->name, dev->path);
    return 0;
}

#endif
