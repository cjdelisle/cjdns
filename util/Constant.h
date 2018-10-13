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

Js({ file.Constant_JS = require("../util/Constant.js"); })

#define Constant_stringForHex(hex) JsOr({ return file.Constant_JS.stringForHex( hex ) }, "")

#define Constant_base2(num) JsOr({ return file.Constant_JS.base2( #num ) }, 0)

#define Constant_rand64() JsOr({ return file.Constant_JS.rand64(file); }, ((uint64_t)0ull))

#define Constant_rand32() JsOr({ return file.Constant_JS.rand32(file); }, ((uint32_t)0ull))

#define Constant_randHexString(len) \
    JsOr({ return file.Constant_JS.randHexString(#len, file); }, "")

#define Constant_log2(num) JsOr({ return file.Constant_JS.log2(num); }, 0)

#endif
