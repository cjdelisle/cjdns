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
#ifndef ConverterV15_H
#define ConverterV15_H

#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "net/SessionManager.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("net/ConverterV15.c")

/**
 * Convert between v15 version packets which contained full IPv6 headers and v16+ packets with
 * cjdns DataHeader.
 */
struct ConverterV15
{
    /** talks v15 */
    struct Iface sessionManagerIf;

    /** talks v16 */
    struct Iface upperDistributorIf;
};

struct ConverterV15* ConverterV15_new(struct Allocator* alloc,
                                      struct Log* log,
                                      struct SessionManager* sm,
                                      uint8_t myIp6[16]);

#endif
