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
#ifndef Constant_H
#define Constant_H

#include "util/Js.h"

Js({ this.Constant_JS = require("../util/Constant.js"); })

#define Constant_stringForHex(hex) Js_or({ return this.Constant_JS.stringForHex( hex ) }, "")

#define Constant_base2(num) Js_or({ return this.Constant_JS.base2( #num ); }, 0xffffffff)

#define Constant_rand64() \
    Js_or({ return this.Constant_JS.rand64(this, js.currentFile); }, 0x12345678abcdefull)

#define Constant_rand32() \
    Js_or({ return this.Constant_JS.rand32(this, js.currentFile); }, 0x12345678u)

#define Constant_randHexString(len) \
    Js_or({ return this.Constant_JS.randHexString(#len, this, js.currentFile); }, "")

#define Constant_log2(num) Js_or({ return this.Constant_JS.log2(num); }, 0)

#endif