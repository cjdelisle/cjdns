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
#ifndef Constant_H
#define Constant_H

<?js file.Constant_JS = require("../util/Constant.js"); ?>

#define Constant_stringForHex(hex) <?js return file.Constant_JS.stringForHex( hex ) ?>

#define Constant_base2(num) <?js return file.Constant_JS.base2( #num ) ?>

#define Constant_rand64() <?js return file.Constant_JS.rand64(); ?>

#define Constant_rand32() <?js return file.Constant_JS.rand32(); ?>

#define Constant_randHexString(len) <?js return file.Constant_JS.randHexString(#len); ?>

#define Constant_log2(num) <?js return file.Constant_JS.log2(num); ?>

#endif
