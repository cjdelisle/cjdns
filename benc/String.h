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
#ifndef String_H
#define String_H

#include "memory/Allocator.h"

#include <stdbool.h>

typedef struct {
    size_t len;
    char* bytes;
} String;

/**
 * Create a new bencoded string from a C null terminated string.
 * This implementation will make a copy of the string into the memory provided by the allocator.
 *
 * @param bytes the beginning of a memory location containing the string to use.
 * @param allocator a means of getting the memory to store the string object.
 * @return a bencoded string.
 */
String* String_new(const char* bytes, const struct Allocator* allocator);

/**
 * Create a new bencoded constant string on the stack.
 */
#define String_CONST(x) (&(String) { .bytes = x, .len = strlen(x) })

#define String_OBJ(x) (&(Object) { .type = Object_STRING, .as.string = x })

/**
 * Create a new bencoded string from a set of bytes.
 * This implementation will make a copy of the string into the memory provided by the allocator.
 *
 * @param bytes the beginning of a memory location containing thre string to use.
                if NULL then this will make a new string of null characters.
 * @param length the number of bytes to use from the location.
 * @param allocator a means of getting the memory to store the string object.
 * @return a bencoded string.
 */
String* String_newBinary(const char* bytes, size_t length, const struct Allocator* allocator);

/**
 * Compare 2 bencoded strings.
 * If the first differing character is numerically smaller for input a then
 * a negative number is returned, if the first differing character is numerically
 * smaller for input b then a positive number. If all characters in a and b are
 * the same then the difference in length (a->len - b->len) is returned.
 * If a is NULL and b is not NULL then a negative is returned, if b is NULL and a
 * not NULL then a positive is returned, if both NULL then 0.
 *
 * @param a the first string to compare.
 * @param b the second string to compare.
 * @return the output from comparison, 0 if and only if they are equal.
 */
int String_compare(const String* a, const String* b);

/**
 * Will return true if and only if the String_compare() would return 0.
 *
 * @param a the first string to compare.
 * @param b the second string to compare.
 * @return !(String_compare(a, b))
 */
bool String_equals(const String* a, const String* b);

#endif
