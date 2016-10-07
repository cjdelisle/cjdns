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
#ifndef RumorMill_H
#define RumorMill_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "dht/Address.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/RumorMill.c");

struct RumorMill_admin
{
    int unused;
};

/**
 * Make a RumorMill available to be queried from the RPC interface.
 */
void RouterMill_admin_exposeMill(struct RouterMill_admin* rma, struct RumorMill* rm);

struct RouterMill_admin* RouterMill_admin_register(struct Admin* admin);

#endif
