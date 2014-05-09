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
#include "memory/Allocator.h"
#include "benc/String.h"
#include "util/Bits.h"

#include <stdio.h>
#include <stdarg.h>

/** @see Object.h */
String* String_new(const char* bytes, struct Allocator* allocator)
{
    return String_newBinary(bytes, CString_strlen(bytes), allocator);
}

/** @see Object.h */
String* String_newBinary(const char* bytes, unsigned long length, struct Allocator* allocator)
{
    char* copy = Allocator_malloc(allocator, length + 1);
    // Make the string null terminated so it will print nicely.
    copy[length] = '\0';
    if (bytes != NULL) {
        Bits_memcpy(copy, bytes, length);
    } else {
        Bits_memset(copy, '\0', length);
    }
    String* string = Allocator_malloc(allocator, sizeof(String));
    string->len = length;
    string->bytes = copy;
    return string;
}

String* String_vprintf(struct Allocator* allocator, const char* format, va_list args)
{
    #define String_BUFFER_SZ 1024
    char buff[String_BUFFER_SZ];
    vsnprintf(buff, String_BUFFER_SZ, format, args);
    size_t length = CString_strlen(buff);
    return String_newBinary(buff, length, allocator);
    #undef String_BUFFER_SZ
}

String* String_printf(struct Allocator* allocator, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    String* out = String_vprintf(allocator, format, args);
    va_end(args);
    return out;
}

int String_compare(const String* a, const String* b)
{
    if (a == NULL || b == NULL) {
        return (a == NULL) - (b == NULL);
    }
    size_t i;
    int d;
    for (i = 0; i < (a->len < b->len ? a->len : b->len); i++)
    {
        d = a->bytes[i] - b->bytes[i];
        if (0 != d) {
            return d;
        }
    }
    return a->len - b->len;
}

int String_equals(const String* a, const String* b)
{
    if (a == NULL || b == NULL) {
        return a == NULL && b == NULL;
    }
    return a->len == b->len && (Bits_memcmp(a->bytes, b->bytes, a->len) == 0);
}
