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
#ifndef AngelInit_H
#define AngelInit_H

#include "util/Linker.h"
Linker_require("admin/angel/AngelInit.c")

/**
 * Input:
 * {
 *   "admin": {
 *     "core": "/path/to/core/binary",
 *     "bind": "127.0.0.1:12345",
 *     "pass": "12345adminsocketpassword",
 *     "user": "setUidToThisUser"
 *   }
 * }
 * for example:
 * d5:admind4:core30:./build/admin/angel/cjdns-core4:bind15:127.0.0.1:123454:pass4:abcdee
 *
/home/user/wrk/cjdns/build/admin/angel/cjdns-core
 * "user" is optional, if set the angel will setuid() that user's uid.
 */
int AngelInit_main(int argc, char** argv);

#endif
