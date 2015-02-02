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
#include "util/Bits.h"

int Bits_log2x64_stupid(uint64_t number)
{
    int out = 0;
    while (number >>= 1) {
        out++;
    }
    return out;
}

void* Bits_memmem(const void* haystack, size_t haystackLen, const void* needle, size_t needleLen)
{
        uint8_t* needleC = (uint8_t*) needle;
        uint8_t* haystackC = (uint8_t*) haystack;
        uint8_t* stopAt = haystackC + haystackLen - needleLen;

        if (!(haystack && needle && haystackLen && needleLen)) {
            return NULL;
        }

        while (haystackC <= stopAt) {
            if (*haystackC == *needleC && !Bits_memcmp(haystackC, needleC, needleLen)) {
                return haystackC;
            }
            haystackC++;
        }

        return NULL;
}
