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
#ifndef Core_admin_H
#define Core_admin_H

#include "net/Ducttape.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "admin/Admin.h"

#include "util/events/EventBase.h"
#include <stdint.h>

void Core_admin_register(uint8_t ipAddr[16],
                         struct Ducttape* dt,
                         struct Log* logger,
                         struct Allocator* alloc,
                         struct Admin* admin,
                         struct EventBase* eventBase);

#endif
