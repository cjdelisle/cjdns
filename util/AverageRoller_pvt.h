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
#ifndef AverageRoller_pvt_H
#define AverageRoller_pvt_H

#include "util/AverageRoller.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("util/AverageRoller.c");

/** Used to represent the sum and entry count for a given second. */
struct AverageRoller_SumAndEntryCount
{
    /** Sum of all entries. */
    uint32_t sum;

    /** Number of entries. */
    uint32_t entryCount;
};

/**
 * A structure for helping calculate a rolling average over some number of seconds.
 * Each element in "seconds" array is responsable for 1 second and it is a circular array.
 */
struct AverageRoller_pvt
{
    struct AverageRoller pub;

    /** The average will be calculated over this number of seconds. */
    const uint32_t windowSeconds;

    /** The number of seconds since the epoch when the average was last updated. */
    uint32_t lastUpdateTime;

    /** The index of the array when the roller was last updated. */
    uint32_t lastUpdateIndex;

    /** The sum of every entry in the last windowSeconds seconds. */
    uint32_t sum;

    /** The total number of entries in the last windowSeconds seconds. */
    uint32_t entryCount;

    /** A stored value equal to the sum divided by the entry count. */
    uint32_t average;

    /** Means of getting the current time. */
    struct EventBase* eventBase;

    Identity

    /**
     * An array of entries containing sum and entry count for each second
     * in the seconds prior to the last update.
     * This array pretends to contain one entry but actually contans windowSeconds entries.
     */
    struct AverageRoller_SumAndEntryCount seconds[1];
};

#endif
