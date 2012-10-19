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
#ifndef AdminTestFramework_H
#define AdminTestFramework_H

#include "admin/Admin.h"
#include "admin/AdminClient.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"

#include <event2/event.h>

struct AdminTestFramework
{
    struct Admin* admin;
    struct AdminClient* client;
    struct Allocator* alloc;
    struct event_base* eventBase;
    struct Log* logger;
};

struct AdminTestFramework* AdminTestFramework_setUp();

#endif
