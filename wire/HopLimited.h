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
#ifndef HopLimited_H
#define HopLimited_H

#include "wire/Headers.h"
#include "util/Assert.h"

struct HopLimited
{
    uint8_t hops;
    uint8_t nextHeader;
    uint16_t reserved;
};
#define HopLimited_SIZE 4
Assert_compileTime(sizeof(struct HopLimited) == HopLimited_SIZE);

#endif
