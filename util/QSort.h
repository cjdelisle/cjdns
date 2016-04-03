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
#ifndef QSort_H
#define QSort_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static inline void QSort(void* base, size_t members, size_t size,
                         int (*compar) (const void *, const void *))
{
    size_t i, j;
    size_t gap = members;
    uintptr_t p1, p2;
    bool worked;

    if (!members) {
        return;
    }

    do {
        gap = (gap * 10) / 13;
        if (gap == 9 || gap == 10) {
            gap = 11;
        } else if (gap < 1) {
            gap = 1;
        }
        worked = false;

        for (i = 0, p1 = (uintptr_t) base; i < members - gap; i++, p1 += size) {
            j = i + gap;
            p2 = (uintptr_t) base + j * size;
            if (compar((void *) p1, (void *) p2) > 0) {
                int k = size;
                uint8_t tmp;
                uint8_t* tp1;
                uint8_t* tp2;

                tp1 = (uint8_t*) p1;
                tp2 = (uint8_t*) p2;
                do {
                    tmp = *tp1;
                    *tp1++ = *tp2;
                    *tp2++ = tmp;
                } while (--k);
                worked = true;
            }
        }
    } while (gap > 1 || worked);
}

#endif
