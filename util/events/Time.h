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
#ifndef Time_H
#define Time_H

#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("util/events/libuv/Time.c");

#include <stdint.h>

/** Nanosecond time which has no relationship to any wall clock. */
uint64_t Time_hrtime();

uint64_t Time_currentTimeMilliseconds(struct EventBase* eventBase);

uint64_t Time_currentTimeSeconds(struct EventBase* eventBase);

#endif
