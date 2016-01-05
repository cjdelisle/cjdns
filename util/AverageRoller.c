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
#include "util/AverageRoller.h"
#include "util/AverageRoller_pvt.h"
#include "util/Bits.h"
#include "util/events/Time.h"


/** @see AverageRoller.h */
struct AverageRoller* AverageRoller_new(const uint32_t windowSeconds,
                                        struct EventBase* eventBase,
                                        struct Allocator* allocator)
{
    size_t size = sizeof(struct AverageRoller_pvt)
        + (sizeof(struct AverageRoller_SumAndEntryCount) * (windowSeconds - 1));
    struct AverageRoller_pvt* roller = Allocator_calloc(allocator, size, 1);

    Bits_memcpy(roller, (&(struct AverageRoller_pvt) {
        .windowSeconds = windowSeconds,
        .eventBase = eventBase,
        .lastUpdateTime = (uint32_t) Time_currentTimeSeconds(eventBase)
    }), sizeof(struct AverageRoller_pvt));

    Identity_set(roller);
    return &roller->pub;
}

/** @see AverageRoller.h */
uint32_t AverageRoller_getAverage(struct AverageRoller* averageRoller)
{
    struct AverageRoller_pvt* roller = Identity_check((struct AverageRoller_pvt*) averageRoller);
    return roller->average;
}

/**
 * Update the roller with a new entry.
 *
 * @param averageRoller the roller to update.
 * @param now the number of seconds since the epoch.
 * @param newEntry the a new number to be factored into the average.
 * @return the average over the last windowSeconds seconds.
 */
uint32_t AverageRoller_updateAtTime(struct AverageRoller* averageRoller,
                                    const uint64_t now,
                                    const uint32_t newEntry)
{
    struct AverageRoller_pvt* roller = Identity_check((struct AverageRoller_pvt*) averageRoller);
    uint32_t index =
        (now - roller->lastUpdateTime + roller->lastUpdateIndex) % roller->windowSeconds;

    if (((uint32_t) now) > roller->lastUpdateTime) {
        roller->sum -= roller->seconds[index].sum;
        roller->entryCount -= roller->seconds[index].entryCount;
        roller->seconds[index].sum = newEntry;
        roller->seconds[index].entryCount = 1;
    } else {
        roller->seconds[index].sum += newEntry;
        roller->seconds[index].entryCount++;
    }
    roller->sum += newEntry;
    roller->entryCount++;
    roller->average = roller->sum / roller->entryCount;
    roller->lastUpdateTime = now;
    roller->lastUpdateIndex = index;

    return roller->average;
}

/** @see AverageRoller.h */
uint32_t AverageRoller_update(struct AverageRoller* averageRoller, const uint32_t newEntry)
{
    struct AverageRoller_pvt* roller = Identity_check((struct AverageRoller_pvt*) averageRoller);
    return AverageRoller_updateAtTime(averageRoller,
                                      Time_currentTimeSeconds(roller->eventBase),
                                      newEntry);
}
