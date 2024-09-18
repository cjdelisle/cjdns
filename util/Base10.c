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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "util/Base10.h"
#include "wire/Message.h"
#include "exception/Er.h"
#include "util/CString.h"
#include "rust/cjdns_sys/Rffi.h"

#include <stdbool.h>

Err_DEFUN Base10_write(Message_t* msg, int64_t num)
{
    bool negative = num < 0;
    if (negative) {
        num = -num;
    } else if (num == 0) {
        Err(Message_epush8(msg, '0'));
        return NULL;
    }
    while (num > 0) {
        Err(Message_epush8(msg, '0' + (num % 10)));
        num /= 10;
    }
    if (negative) {
        Err(Message_epush8(msg, '-'));
    }
    return NULL;
}

Er_DEFUN(int64_t Base10_read(Message_t* msg))
{
    int64_t numOut = 0;
    uint32_t bytes = 0;
    int out = Rffi_parseBase10(
        Message_bytes(msg), Message_getLength(msg), &numOut, &bytes);
    if (out != 0) {
        Er_raise(Message_getAlloc(msg), "Error reading base10: %d", out);
    }
    Er(Er_fromErr(Message_eshift(msg, -bytes)));
    Er_ret(numOut);
}

int Base10_fromString(uint8_t* str, int64_t* numOut)
{
    int len = CString_strlen((char*)str);
    if (len < 1) {
        return -1;
    }
    uint32_t bytes = 0;
    return Rffi_parseBase10(str, len, numOut, &bytes);
}
