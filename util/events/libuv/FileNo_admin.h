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
#ifndef FileNo_admin_H
#define FileNo_admin_H

#include "admin/Admin.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/events/FileNo.h"
#include "util/Linker.h"
Linker_require("util/events/libuv/FileNo_admin.c");

struct FileNo_admin
{
    void* userData;
};

void FileNo_admin_register(struct Admin* admin,
                           struct Allocator* alloc,
                           struct EventBase* base,
                           struct Log* logger,
                           struct Except* eh);

#endif
