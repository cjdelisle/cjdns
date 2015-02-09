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
#ifndef Bridge_H
#define Bridge_H

#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "wire/ContentType.h"
#include "util/Linker.h"
Linker_require("net/Bridge.c")

struct Bridge
{
    /**
     * Special iface which handles all types <= 255, handling ranges is not generally needed,
     * just in this edge case.
     */
    struct Interface_Two cjdnsIp6If;
};

void Bridge_regIface(struct Bridge* bridge, struct Interface_Two* iface, enum ContentType type);

struct Bridge* Bridge_new(struct Allocator* alloc);

#endif
