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

#include "interface/tuntap/win32/TAPInterface.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "memory/CanaryAllocator.h"
#include "memory/MallocAllocator.h"
#include "util/events/EventBase.h"
#include "interface/Interface.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"

int main()
{
    struct Allocator* alloc = CanaryAllocator_new(MallocAllocator_new(1<<20), NULL);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct EventBase* base = EventBase_new(alloc);

    /*struct Interface* iface = */TAPInterface_new(NULL, NULL, logger, base, alloc);
    Allocator_free(alloc);
}
