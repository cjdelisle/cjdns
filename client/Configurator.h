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
#ifndef Configurator_H
#define Configurator_H

#include "benc/String.h"
#include "benc/Dict.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/platform/Sockaddr.h"
#include "util/Linker.h"
Linker_require("client/Configurator.c");

#include <stdint.h>

void Configurator_config(Dict* config,
                         struct Sockaddr* addr,
                         String* adminPassword,
                         struct EventBase* eventBase,
                         struct Log* logger,
                         struct Allocator* alloc);

#endif
