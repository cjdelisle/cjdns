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
#ifndef Defined_H
#define Defined_H

/**
 * Preprocesses to a 1 if the macro is defined, a 0 if it is not.
 * Example:
 *
 * if (Defined(linux)) {
 *     doLinuxSpecificStuff();
 * }
 */
#define Defined_Q <?js return String.fromCharCode(34); ?>
#define Defined(macro) \
    <?js return ( Defined_Q macro Defined_Q === ' ' + #macro + ' ' ) ? '0' : '1'; ?>

#endif
