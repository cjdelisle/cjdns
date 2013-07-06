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

#define UniqueName_MAKE UniqueName_MAKE_NAME(UniqueName_COUNTER, __LINE__)
#define UniqueName_MAKE_NAME(x, y) UniqueName_MAKE_NAME2(x, y)
#define UniqueName_MAKE_NAME2(x, y) UniqueName_generated_ ## x ## _ ## y
#define UniqueName_GLUE(x, y) UniqueName_GLUE2(x, y)
#define UniqueName_GLUE2(x, y) x ## y
#define UniqueName_GLUE8(a,b,c,d,e,f,g,h) UniqueName_GLUE8B(a,b,c,d,e,f,g,h)
#define UniqueName_GLUE8B(a,b,c,d,e,f,g,h) a ## b ## c ## d ## e ## f ## g ## h

#define UniqueName_get() UniqueName_MAKE

#define UniqueName_COUNTER_0 0
#define UniqueName_COUNTER_1 0
#define UniqueName_COUNTER_2 0
#define UniqueName_COUNTER_3 0
#define UniqueName_COUNTER_4 0
#define UniqueName_COUNTER_5 0
#define UniqueName_COUNTER_6 0
#define UniqueName_COUNTER_7 0

#endif

#define UniqueName_CARRY
#ifdef UniqueName_CARRY
    #if (UniqueName_COUNTER_0 + 1) & 1
        #undef UniqueName_COUNTER_0
        #define UniqueName_COUNTER_0 1
        #undef UniqueName_CARRY
    #else
        #undef UniqueName_COUNTER_0
        #define UniqueName_COUNTER_0 0
    #endif
#endif
#ifdef UniqueName_CARRY
    #if (UniqueName_COUNTER_1 + 1) & 1
        #undef UniqueName_COUNTER_1
        #define UniqueName_COUNTER_1 1
        #undef UniqueName_CARRY
    #else
        #undef UniqueName_COUNTER_1
        #define UniqueName_COUNTER_1 0
    #endif
#endif
#ifdef UniqueName_CARRY
    #if (UniqueName_COUNTER_2 + 1) & 1
        #undef UniqueName_COUNTER_2
        #define UniqueName_COUNTER_2 1
        #undef UniqueName_CARRY
    #else
        #undef UniqueName_COUNTER_2
        #define UniqueName_COUNTER_2 0
    #endif
#endif
#ifdef UniqueName_CARRY
    #if (UniqueName_COUNTER_3 + 1) & 1
        #undef UniqueName_COUNTER_3
        #define UniqueName_COUNTER_3 1
        #undef UniqueName_CARRY
    #else
        #undef UniqueName_COUNTER_3
        #define UniqueName_COUNTER_3 0
    #endif
#endif
#ifdef UniqueName_CARRY
    #if (UniqueName_COUNTER_4 + 1) & 1
        #undef UniqueName_COUNTER_4
        #define UniqueName_COUNTER_4 1
        #undef UniqueName_CARRY
    #else
        #undef UniqueName_COUNTER_4
        #define UniqueName_COUNTER_4 0
    #endif
#endif
#ifdef UniqueName_CARRY
    #if (UniqueName_COUNTER_5 + 1) & 1
        #undef UniqueName_COUNTER_5
        #define UniqueName_COUNTER_5 1
        #undef UniqueName_CARRY
    #else
        #undef UniqueName_COUNTER_5
        #define UniqueName_COUNTER_5 0
    #endif
#endif
#ifdef UniqueName_CARRY
    #if (UniqueName_COUNTER_6 + 1) & 1
        #undef UniqueName_COUNTER_6
        #define UniqueName_COUNTER_6 1
        #undef UniqueName_CARRY
    #else
        #undef UniqueName_COUNTER_6
        #define UniqueName_COUNTER_6 0
    #endif
#endif
#ifdef UniqueName_CARRY
    #if (UniqueName_COUNTER_7 + 1) & 1
        #undef UniqueName_COUNTER_7
        #define UniqueName_COUNTER_7 1
        #undef UniqueName_CARRY
    #else
        #undef UniqueName_COUNTER_7
        #define UniqueName_COUNTER_7 0
    #endif
#endif

#undef UniqueName_COUNTER
#define UniqueName_COUNTER \
    UniqueName_GLUE8(UniqueName_COUNTER_0, UniqueName_COUNTER_1, \
                     UniqueName_COUNTER_2, UniqueName_COUNTER_3, \
                     UniqueName_COUNTER_4, UniqueName_COUNTER_5, \
                     UniqueName_COUNTER_6, UniqueName_COUNTER_7)


struct UniqueName_get()
{
    char uniqueName_target;
};
