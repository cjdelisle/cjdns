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
#ifndef AndroidWrapper_H
#define AndroidWrapper_H

#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("interface/tuntap/AndroidWrapper.c");

struct AndroidWrapper
{
    struct Iface internalIf;
    struct Iface externalIf;
};

struct AndroidWrapper* AndroidWrapper_new(struct Allocator* alloc, struct Log* log);

#endif
