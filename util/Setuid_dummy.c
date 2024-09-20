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
#include "util/Setuid.h"
#include "memory/Allocator.h"
#include "exception/Err.h"

// This implementation does nothing

Err_DEFUN Setuid_preSetuid(struct Allocator* alloc)
{
    return NULL;
}

Err_DEFUN Setuid_postSetuid(struct Allocator* alloc)
{
    return NULL;
}
