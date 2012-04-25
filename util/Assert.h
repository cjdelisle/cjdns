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
#ifndef Assert_H
#define Assert_H

/**
 * Prevent compilation if assertion is false.
 * Thanks to http://www.jaggersoft.com/pubs/CVu11_3.html
 */
#define Assert_UNIQUE_NAME Assert_MAKE_NAME(__LINE__)
#define Assert_MAKE_NAME(x) Assert_MAKE_NAME2(x)
#define Assert_MAKE_NAME2(x) Assert_testStruct_ ## x
#define Assert_assertTrue(isTrue) struct Assert_UNIQUE_NAME { unsigned int assertFailed : (isTrue); }

#endif
