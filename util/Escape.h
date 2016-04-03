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
#ifndef Escape_H
#define Escape_H

#include "memory/Allocator.h"
#include "util/Assert.h"

static inline char* Escape_getEscaped(uint8_t* buff, int length, struct Allocator* alloc)
{
    const char* hexSymbols = "0123456789abcdef";
    int finalSize = 1;
    for (int i = 0; i < length; i++) {
        if (buff[i] > 31 && buff[i] < 127) {
            finalSize++;
        } else {
            finalSize += 4;
        }
    }
    char* output = Allocator_malloc(alloc, finalSize);
    int j = 0;
    for (int i = 0; i < length; i++) {
        if (buff[i] > 31 && buff[i] < 127) {
            output[j++] = buff[i];
        } else {
            output[j++] = '\\';
            output[j++] = 'x';
            output[j++] = hexSymbols[buff[i] >> 4];
            output[j++] = hexSymbols[buff[i] & 0x0f];
        }
    }
    output[j++] = '\0';
    Assert_true(j == finalSize);
    return output;
}

#endif
