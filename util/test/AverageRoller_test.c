#include "util/AverageRoller.c"

#include <stdio.h>

#include "memory/BufferAllocator.h"

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

    uint8_t buffer[4096];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 4096);

    struct AverageRoller* roller =
        (struct AverageRoller*) AverageRoller_new(windowSeconds, allocator);
    // To make life easy we will pretend it's january first 1970...
    roller->lastUpdateTime = 0;

    uint32_t ret = 0;
    for (uint32_t i = 0; i < ENTRY_COUNT; i++)
    {
        uint32_t average = update(roller, testData[i][0], testData[i][1]);

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
