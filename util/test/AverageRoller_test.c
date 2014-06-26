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
#include "memory/MallocAllocator.h"
#include "util/AverageRoller.h"
#include "util/AverageRoller_pvt.h"
#include "util/events/EventBase.h"

#include <stdio.h>

int main()
{
    #define ENTRY_COUNT 13
    // {seconds,entry}
    const uint32_t testData[ENTRY_COUNT][3] =
        {
        // The entry between the */and/* is the one which was just altered.
        // second  col1   col2  col3   average
            { 0,/* */0/*             */,  0},
            { 1,/*   0   */1/*       */,  0},
            { 2,/*   0     1   */2/* */,  1},
            { 3,/* */3/*   1     2   */,  2},
            { 4,/*   3   */4/*   2   */,  3},
            { 5,/*   3     4   */5/* */,  4},
            { 6,/* */6/*   4     5   */,  5},
            { 7,/*   6   */7/*   5   */,  6},
            { 8,/*   6     7   */8/* */,  7},
            { 9,/* */9/*   7     8   */,  8},
            //       9   */7/*   8        8   skipped a second
            {11,/*   9     7   */7/* */,  7},
            {12,/* */6/*   7     7   */,  6},
            //       6   */7/*   7        6   skipped a second
            {14,/*   6     7   */4/* */,  5}
        };

    const uint32_t windowSeconds = 3;

    struct Allocator* allocator = MallocAllocator_new(4096);

    struct EventBase* eventBase = EventBase_new(allocator);

    struct AverageRoller_pvt* roller =
        (struct AverageRoller_pvt*) AverageRoller_new(windowSeconds, eventBase, allocator);
    // To make life easy we will pretend it's january first 1970...
    roller->lastUpdateTime = 0;

    uint32_t ret = 0;
    for (uint32_t i = 0; i < ENTRY_COUNT; i++)
    {
        uint32_t average = AverageRoller_updateAtTime(&roller->pub, testData[i][0], testData[i][1]);

        if (average != testData[i][2]) {
            printf("For average #%d, expected %d, got %d,  float: %f,  entryCount: %d\n",
                   (int) i, (int) testData[i][2], (int) average,
                   ((float) roller->sum) / roller->entryCount,
                   (int) roller->entryCount);
            ret = 1;
        }
    }
    return ret;
}
