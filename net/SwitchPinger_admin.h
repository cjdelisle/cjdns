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
#ifndef SwitchPinger_admin_H
#define SwitchPinger_admin_H

#include "admin/Admin.h"
#include "net/SwitchPinger.h"
#include "util/Linker.h"
Linker_require("net/SwitchPinger_admin.c")

void SwitchPinger_admin_register(struct SwitchPinger* sp,
                                 struct Admin* admin,
                                 struct Allocator* alloc);

#endif
