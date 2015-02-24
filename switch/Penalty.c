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

#include "switch/Penalty.h"
#include "switch/PenaltyFloat.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/log/Log.h"

/**
 * Penalty entry
 *
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 |  Packed Penalty (host order)  |  size   |  timeMilliseconds   |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#define ENTRY_COUNT 1024

struct Penalty_pvt
{
    struct Penalty pub;
    struct Timeout* maintanenceTimeout;
    struct EventBase* eventBase;
    struct Log* log;
    int lastReplaced;
    uint32_t entries[ENTRY_COUNT];
    Identity
};

#define EXPIRE_AFTER_MILLISECONDS 1024

#define PENALTY_TIME(penaltyEntry) ((penaltyEntry) << 21 >> 21)

static uint16_t handlePacket(struct Penalty_pvt* p,
                             uint32_t currentPackedPenalty,
                             int messageLength)
{
    uint32_t now = Time_currentTimeMilliseconds(p->eventBase);
    uint64_t unpackedPenalty = PenaltyFloat_unpack((uint16_t)currentPackedPenalty);
    uint32_t newPenaltyEntry =
        (PenaltyFloat_pack(unpackedPenalty) << 16) |
        (((messageLength / 128) & ((1<<5)-1)) << 11) |
        (now & ((1<<11)-1));

    // normalize currentPackedPenalty
    currentPackedPenalty = newPenaltyEntry >> 16;

    for (int i = 0; i < ENTRY_COUNT; i++) {
        // Non-branching comparison: 30% - 60% better perf for whole switching process (no crypto)
        uint32_t e = p->entries[i];
        //if (e > newPenaltyEntry) { unpackedPenalty += (e << 16 >> 11); }
        unpackedPenalty += ( ((((e - newPenaltyEntry - 1) >> 31) << 31) - 1) & (e << 16 >> 11) );
    }
    p->lastReplaced = (p->lastReplaced + 1) % ENTRY_COUNT;
    p->entries[p->lastReplaced] = newPenaltyEntry;

    uint16_t newPackedPenalty = PenaltyFloat_pack(unpackedPenalty);
    if (newPackedPenalty > currentPackedPenalty) {
        /* Per packet logging...
        Log_debug(p->log, "Adjusting penalty [%016llx] -> [%016llx]",
                  (long long)PenaltyFloat_unpack((uint16_t)currentPackedPenalty),
                  (long long)unpackedPenalty);
        */
        return newPackedPenalty;
    }
    return (uint16_t)currentPackedPenalty;
}

void Penalty_apply(struct Penalty* penalty, struct SwitchHeader* switchHeader, int messageLen)
{
    struct Penalty_pvt* p = Identity_check((struct Penalty_pvt*) penalty);
    uint16_t currentPenalty = SwitchHeader_getPenalty(switchHeader);
    SwitchHeader_setPenalty(switchHeader, handlePacket(p, currentPenalty, messageLen));
}

static void maintanence(void* vPenalty)
{
    struct Penalty_pvt* p = Identity_check((struct Penalty_pvt*) vPenalty);
    uint32_t now = Time_currentTimeMilliseconds(p->eventBase);
    uint32_t tooOld = PENALTY_TIME(now - EXPIRE_AFTER_MILLISECONDS);
    now = PENALTY_TIME(now);

    for (int i = 0; i < ENTRY_COUNT; i++) {
        if (PENALTY_TIME(p->entries[i]) < tooOld) {
            // set to max penalty.
            p->entries[i] = (PenaltyFloat_MAX << 16) | now;
        }
    }
}

struct Penalty* Penalty_new(struct Allocator* alloc, struct EventBase* base, struct Log* log)
{
    struct Penalty_pvt* p = Allocator_calloc(alloc, sizeof(struct Penalty_pvt), 1);
    p->eventBase = base;
    p->maintanenceTimeout = Timeout_setInterval(maintanence, p, 1024, base, alloc);
    p->log = log;
    Identity_set(p);
    uint32_t now = Time_currentTimeMilliseconds(p->eventBase);
    for (int i = 0; i < ENTRY_COUNT; i++) {
        p->entries[i] = (PenaltyFloat_MAX << 16) | now;
    }
    return &p->pub;
}
