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
#ifndef UniqueName_H
#define UniqueName_H

#define UniqueName_COUNTER x
#define UniqueName_MAKE UniqueName_MAKE_NAME(UniqueName_COUNTER, __LINE__)
#define UniqueName_MAKE_NAME(x, y) UniqueName_MAKE_NAME2(x, y)
#define UniqueName_MAKE_NAME2(x, y) UniqueName_generated_ ## x ## y
#define UniqueName_GLUE(x, y) x ## y

#define UniqueName_get() UniqueName_MAKE

#else /* #ifdef UniqueName_H */
    // This is needed every time the file is pulled in to prevent name collisions.
    #define UniqueName_COUNTER2 UniqueName_COUNTER
    #undef UniqueName_COUNTER
    #define UniqueName_COUNTER UniqueName_GLUE(UniqueName_COUNTER2, x)
#endif
