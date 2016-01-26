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
#ifndef String_H
#define String_H

#include "benc/Object.h"
#include "memory/Allocator.h"
#include "util/CString.h"
#include "util/Linker.h"
Linker_require("benc/String.c");

#include <stdbool.h>
#include <stddef.h> // NULL

/**
 * Create a new bencoded string from a C null terminated string.
 * This implementation will make a copy of the string into the memory provided by the allocator.
 *
 * @param bytes the beginning of a memory location containing the string to use.
 * @param allocator a means of getting the memory to store the string object.
 * @return a bencoded string.
 */
String* String_new(const char* bytes, struct Allocator* allocator);

/**
 * Create a new bencoded constant string on the stack.
 */
#define String_CONST(x) (&(String) { .bytes = x, .len = CString_strlen(x) })

/** For use outside of functions with compile time constant strings. */
#define String_CONST_SO(x) (&(String) { .bytes = x, .len = sizeof(x) - 1 })

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
String* String_newBinary(const char* bytes, unsigned long length, struct Allocator* allocator);

#define String_clone(string, alloc) \
    ((string) ? String_newBinary(string->bytes, string->len, alloc) : NULL)

/**
 * Create a new bencoded string from a format and arguments.
 * EG: String_printf("this is on line number %d!", allocator, __LINE__);
 *
 * @param allocator a means of getting the memory to store the string object.
 * @param format standard printf formatting.
 * @params arguments to the printf() function.
 * @return a bencoded string.
 */
String* String_printf(struct Allocator* allocator, const char* format, ...);

#ifdef va_start
/**
 * Same as String_printf() except the arguments are passed as a va_list.
 * Only enabled if stdarg.h is included before String.h.
 */
String* String_vprintf(struct Allocator* allocator, const char* format, va_list args);
#endif

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
int String_equals(const String* a, const String* b);

#endif
