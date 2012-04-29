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

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

/** @see Object.h */
String* String_new(const char* bytes, const struct Allocator* allocator)
{
    return String_newBinary(bytes, strlen(bytes), allocator);
}

/** @see Object.h */
String* String_newBinary(const char* bytes, size_t length, const struct Allocator* allocator)
{
    char* copy = allocator->malloc(length + 1, allocator);
    // Make the string null terminated so it will print nicely.
    copy[length] = '\0';
    if (bytes != NULL) {
        Bits_memcpy(copy, bytes, length);
    } else {
        memset(copy, '\0', length);
    }
    String* string = allocator->malloc(sizeof(String), allocator);
    string->len = length;
    string->bytes = copy;
    return string;
}

String* String_printf(const struct Allocator* allocator, const char* format, ...)
{
    #define String_BUFFER_SZ 1024
    char buff[String_BUFFER_SZ];
    va_list args;
    va_start(args, format);
    vsnprintf(buff, String_BUFFER_SZ, format, args);
    size_t length = strlen(buff);
    return String_newBinary(buff, length, allocator);
    #undef String_BUFFER_SZ
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
        if (0 != d) return d;
    }
    return a->len - b->len;
}

bool String_equals(const String* a, const String* b)
{
    if (a == NULL || b == NULL) {
        return a == NULL && b == NULL;
    }
    return a->len == b->len && (memcmp(a->bytes, b->bytes, a->len) == 0);
}
