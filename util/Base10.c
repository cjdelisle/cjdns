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
#include "exception/Except.h"
#include "util/CString.h"

#include <stdbool.h>

Er_DEFUN(void Base10_write(struct Message* msg, int64_t num))
{
    bool negative = num < 0;
    if (negative) {
        num = -num;
    } else if (num == 0) {
        Er(Message_epush8(msg, '0'));
        Er_ret();
    }
    while (num > 0) {
        Er(Message_epush8(msg, '0' + (num % 10)));
        num /= 10;
    }
    if (negative) {
        Er(Message_epush8(msg, '-'));
    }
    Er_ret();
}

Er_DEFUN(int64_t Base10_read(struct Message* msg))
{
    int64_t out = 0;
    bool negative = false;
    uint8_t chr = Er(Message_epop8(msg));
    if (chr == '-') {
        negative = true;
        chr = Er(Message_epop8(msg));
    }
    if (chr >= '0' && chr <= '9') {
        while (chr >= '0' && chr <= '9') {
            out *= 10;
            out += chr - '0';
            if (msg->length == 0) {
                if (negative) { out = -out; }
                Er_ret(out);
            }
            chr = Er(Message_epop8(msg));
        }
        Er(Message_epush8(msg, chr));
        if (negative) { out = -out; }
        Er_ret(out);
    } else {
        Er(Message_epush8(msg, chr));
        Er_raise(msg->alloc, "No base10 characters found");
    }
}

int Base10_fromString(uint8_t* str, int64_t* numOut)
{
    int len = CString_strlen(str);
    if (len < 1) {
        return -1;
    } else if (str[0] == '-') {
        if (len < 2 || str[1] < '0' || str[1] > '9') {
            return -1;
        }
    } else if (str[0] < '0' || str[0] > '9') {
        return -1;
    }
    struct Message msg = { .length = len, .bytes = str, .capacity = len };
    *numOut = Er_assert(Base10_read(&msg));
    return 0;
}
