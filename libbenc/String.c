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
#include <string.h>

#include "memory/MemAllocator.h"
#include "libbenc/benc.h"

/** @see benc.h */
String* benc_newString(const char* bytes, const struct MemAllocator* allocator)
{
    return benc_newBinaryString(bytes, strlen(bytes), allocator);
}

/** @see benc.h */
String* benc_newBinaryString(const char* bytes, size_t length, const struct MemAllocator* allocator)
{
    char* copy = allocator->malloc(length + 1, allocator);
    // Make the string null terminated so it will print nicely.
    copy[length] = '\0';
    if (bytes != NULL) {
        memcpy(copy, bytes, length);
    } else {
        memset(copy, '\0', length);
    }
    benc_bstr_t* string = allocator->malloc(sizeof(benc_bstr_t), allocator);
    string->len = length;
    string->bytes = copy;
    return string;
}

int benc_bstr_compare(const String* a, const String* b)
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

int32_t benc_stringEquals(const String* a, const String* b)
{
    if (a == NULL || b == NULL) {
        return a == NULL && b == NULL;
    }
    return a->len == b->len && (memcmp(a->bytes, b->bytes, a->len) == 0);
}
