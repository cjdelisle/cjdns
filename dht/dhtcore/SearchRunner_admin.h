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
#ifndef SearchRunner_admin_H
#define SearchRunner_admin_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "admin/Admin.h"
#include "dht/dhtcore/SearchRunner.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/SearchRunner_admin.c");

void SearchRunner_admin_register(struct SearchRunner* runner,
                                 struct Admin* admin,
                                 struct Allocator* alloc);

#endif
