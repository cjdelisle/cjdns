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
#include "switch/LabelSplicer.h"
#include "util/Endian.h"

#include <stdio.h>

int splice()
{
    // 000000100
    uint64_t goHere = 1<<2;
    uint64_t goHere_be = Endian_bigEndianToHost64(goHere);

    // 000000100
    uint64_t viaHere = 1<<2;
    uint64_t viaHere_be = Endian_bigEndianToHost64(viaHere);

    // 000010000
    uint64_t expected = 1<<4;
    uint64_t expected_be = Endian_bigEndianToHost64(expected);

    uint64_t out_be = LabelSplicer_splice(goHere_be, viaHere_be);

    //uint64_t out = Endian_bigEndianToHost64(out_be);
    //printf("Splicing %lu with %lu yields %lu, expecting %lu\n", goHere, viaHere, out, expected);

    return expected_be != out_be;
}

int main()
{
    return splice();
}
