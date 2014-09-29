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

/**
 * Penalty entry
 *
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 |R|U|          Packed Penalty (host order)      |     cycle     |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * The penalty entry contains a cycle which is based on the periodic cleanup
 * cycles that ensure that a penalty entry will not last much longer than one second.
 *
 * R is a rollover flag to use subtraction in order to determine which value is larger.
 * U is unused :)
 */

#define ENTRY_COUNT 1024

struct Penalty_pvt
{
    struct Penalty pub;
    struct Timeout* maintanenceTimeout;
    uint8_t currentCycle;
    int lastReplaced;
    uint32_t entries[ENTRY_COUNT];
    Identity
};

uint32_t Penalty_handlePacket(struct Penalty* penalty, uint32_t currentPackedPenalty)
{
    struct Penalty_pvt* p = Identity_check((struct Penalty_pvt*) penalty);
    uint64_t unpackedPenalty = PenaltyFloat_unpack(currentPackedPenalty);
    currentPackedPenalty <<= 8;
    for (int i = 0; i < ENTRY_COUNT; i++) {
        // greater-than-or-equal-to without invoking a branch
        // roll it over and check for the highbit
        unpackedPenalty += ((currentPackedPenalty - p->entries[i] - 1) >> 31);
    }
    p->lastReplaced = ((p->lastReplaced + 1) % ENTRY_COUNT;
    p->entries[p->lastReplaced] = currentPackedPenalty | p->currentCycle;
    return PenaltyFloat_pack(unpackedPenalty);
}

void maintanence(void* vPenalty)
{
    struct Penalty_pvt* p = Identity_check((struct Penalty_pvt*) vPenalty);
    p->currentCycle++;
    for (int i = 0; i < ENTRY_COUNT; i++) {
        uint8_t ecycle = p->entries[i] & 0xff;
        if (++ecycle != p->currentCycle &&
            ++ecycle != p->currentCycle &&
            ++ecycle != p->currentCycle)
        {
            p->entries[i] = 0x3fffff00 | p->currentCycle;
        }
    }
}

struct Penalty* Penalty_new(struct Allocator* alloc, struct EventBase* base)
{
    struct Penalty_pvt* p = Allocator_calloc(alloc, sizeof(struct Penalty_pvt), 1);
    p->maintanenceTimeout = Timeout_setInterval(maintanence, p, 250, base, alloc);
}
