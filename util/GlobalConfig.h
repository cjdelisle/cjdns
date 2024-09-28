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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef GlobalConfig_H
#define GlobalConfig_H

#include "benc/Object.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("util/GlobalConfig.c")

/**
 * Cjdns is mostly a very well structured project, there is not a lot of
 * communication between modules. However there are a couple of things
 * which are concerns of a number of very different modules. For example
 * the name of the TUN device which you have configured is important to
 * the UDP interface because it doesn't want to broadcast messages down
 * that interface, also it's interesting to the IpTunnel because the
 * IpTunnel needs to make routes to the TUN device.
 *
 * This file is for those things which for very good reason, simply need
 * to violate the architecture of cjdns and communicate between far away
 * modules. Things which make sense to be in here might include peers'
 * IP addresses. Please don't use this to break the architecture when it
 * is not strictly needed.
 */
struct GlobalConfig {
    int unused;
};

String* GlobalConfig_getTunName(struct GlobalConfig* conf);
void GlobalConfig_setTunName(struct GlobalConfig* conf, String* name);

struct GlobalConfig* GlobalConfig_new(struct Allocator* alloc);

#endif