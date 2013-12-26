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
#ifndef CompileTimeRandom_H
#define CompileTimeRandom_H

#define CompileTimeRandom_uint64() ((uint64_t) <?js return \
    '0x' + (Math.random().toString(16) + Math.random().toString(16)).replace(/0\./g, '') + 'ull' \
?>)

#define CompileTimeRandom_uint32() ((uint32_t) <?js return \
    '0x' + Math.random().toString(16).replace(/0\./g, '') + 'ul'                                 \
?>)

#define CompileTimeRandom_hexString(len) <?js \
    var hex = '';                                                                                \
    while (hex.length < len) { hex += Math.random().toString(16).substring(2); }                 \
    return '"' + hex.substring(0,len) + '"';                                                     \
?>

#endif
