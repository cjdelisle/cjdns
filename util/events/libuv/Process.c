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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "rust/cjdns_sys/Rffi.h"
#include "memory/Allocator.h"
#include "util/events/Process.h"
#include "util/Bits.h"
#include "util/Identity.h"

int Process_spawn(const char* binaryPath,
                  const char* const* args,
                  struct Allocator* alloc,
                  Process_OnExitCallback callback)
{
    int i;
    for (i = 0; args[i]; i++) ;
    return Rffi_spawn(binaryPath, args, i, alloc, callback);
}

const char* Process_getPath(struct Allocator* alloc)
{
    const char* out;
    int ret = Rffi_exepath(&out, alloc);
    if (ret < 0) {
        out = 0;
    }
    return out;
}
