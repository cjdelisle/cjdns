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
struct Average
{
    uint32_t shiftedAvg;
    uint32_t shiftCount;
    uint32_t weight;
};

static inline void Average_init(struct Average* average, uint32_t factor, uint32_t weight)
{
	average->weight = Bits_log2x32(weight);
	average->shiftCount = Bits_log2x32(factor);
	average->shiftedAvg = 0;

    // must be a power of 2
    Assert_true((1 << average->shiftCount) == factor);
    Assert_true((1 << average->weight) == weight);
}

static inline void Average_accumulate(struct Average* average, uint32_t value)
{
    if (!average->shiftedAvg) {
        average->shiftedAvg = value << average->factor;
        return;
    }
    uint32_t sha = average->shiftedAvg;
    sha = ( (sha << average->weight) - sha + (value << avg->shiftCount) ) >> average->weight;
    average->shiftedAvg = sha;
}

static inline uint32_t Average_value(struct Average* average)
{
    return avg->shiftedAvg >> avg->shiftFactor;
}
