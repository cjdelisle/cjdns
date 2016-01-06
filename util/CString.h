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
#ifndef CString_H
#define CString_H

#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("util/CString.c");

#include <stddef.h> // size_t

Gcc_PURE
Gcc_NONNULL(1)
unsigned long CString_strlen(const char* str);

Gcc_PURE
Gcc_NONNULL(1,2)
int CString_strcmp(const char* a, const char* b);

Gcc_PURE
Gcc_NONNULL(1,2)
int CString_strncmp(const char* a, const char *b, size_t n);

Gcc_PURE
Gcc_NONNULL(1)
char* CString_strchr(const char *a, int b);

Gcc_PURE
Gcc_NONNULL(1)
char* CString_strrchr(const char *a, int b);

Gcc_PURE
int CString_strcasecmp(const char *a, const char *b);

/** strstr(haystack, needle); */
Gcc_PURE
Gcc_NONNULL(1,2)
char* CString_strstr(const char* a, const char* b);

Gcc_NONNULL(1,2)
char* CString_strcpy(char* restrict dest, const char* restrict src);

Gcc_NONNULL(1,2)
char* CString_strncpy(char* restrict dest, const char *restrict src, size_t n);

#endif
