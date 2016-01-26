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
#ifndef Penalty_H
#define Penalty_H

#include "wire/SwitchHeader.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("switch/Penalty.c");

struct Penalty
{
    int unused;
};

void Penalty_apply(struct Penalty* penalty, struct SwitchHeader* switchHeader, int messageLen);
struct Penalty* Penalty_new(struct Allocator* alloc, struct EventBase* base, struct Log* log);

#endif
