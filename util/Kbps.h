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
#ifndef Kbps_H
#define Kbps_H

#include "util/Constant.h"

// Must be multiples of 2
#define Kbps_WINDOW_SIZE 8

// Must be a multiple of 1024 (1 second)
#define Kbps_TIMESPAN    1024

#define Kbps_WINDOW_SH Constant_log2(Kbps_WINDOW_SIZE)
#define Kbps_TIMESPAN_SH Constant_log2(Kbps_TIMESPAN)

struct Kbps
{
    /** bookkeeping */
    uint32_t window[Kbps_WINDOW_SIZE];

    /** Millisecond time of last message. */
    uint32_t lastTime;

    /** KiloBytes per timeslot. */
    uint32_t currentBpt;
};

#define Kbps_accumulate_NO_PACKET 0xffffffff
static inline uint32_t Kbps_accumulate(struct Kbps* ctx, uint32_t now, uint32_t packetSize)
{
    // now >> 10-3 == now / 1024 * 8 (eighth second Kbps_WINDOW_SIZE)
    uint32_t xnow = now;
    if ((xnow - ctx->lastTime) >> 31) { xnow = ctx->lastTime; }
    int slot = (xnow >> (Kbps_TIMESPAN_SH - Kbps_WINDOW_SH) ) % Kbps_WINDOW_SIZE;
    int lslot = ( ctx->lastTime >> (Kbps_TIMESPAN_SH - Kbps_WINDOW_SH) ) % Kbps_WINDOW_SIZE;
    if (xnow - ctx->lastTime > Kbps_TIMESPAN ||
        ((lslot == slot) && (xnow - ctx->lastTime > (Kbps_TIMESPAN >> 1))))
    {
        for (int i = 0; i < Kbps_WINDOW_SIZE; i++) {
            ctx->currentBpt -= ctx->window[i];
            ctx->window[i] = 0;
        }
        Assert_true(!ctx->currentBpt);
    } else if (lslot != slot) {
        for (int i = lslot + 1; ; i++) {
            i %= Kbps_WINDOW_SIZE;
            ctx->currentBpt -= ctx->window[i];
            ctx->window[i] = 0;
            if (i == slot) { break; }
        }
    }
    // Make sure it didn't go under zero and roll over
    Assert_true(!(ctx->currentBpt >> 31));
    if (packetSize != Kbps_accumulate_NO_PACKET) {
        ctx->window[slot] += packetSize;
        ctx->currentBpt += packetSize;
    }
    ctx->lastTime = now;

    // /= 2 ** Kbps_TIMESPAN_SH --> bytes per timespan to bytes per millisecond
    // *= 2 ** 10               --> bytes per millisecond to bytes per second
    // *= 2 ** 3                --> bytes per second to bits per second
    // /= 2 ** 10               --> bits per second to kbits per second
    return (
        ctx->currentBpt <?js
            var x = (Number(Kbps_TIMESPAN_SH) - 10 - 3 + 10);
            return ( ((x) < 0) ? "<<" : ">>" ) + " " + Math.abs(x);
        ?>
    );
}

#endif
