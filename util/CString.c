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
#include "util/CString.h"

#include <string.h>

#if defined(Illumos) || defined(__sun)
    #define _XPG4_2
#endif
#include <strings.h>

unsigned long CString_strlen(const char* str)
{
    return strlen(str);
}

int CString_strcmp(const char* a, const char* b)
{
    return strcmp(a,b);
}

int CString_strncmp(const char* a, const char *b, size_t n)
{
    return strncmp(a, b, n);
}

char* CString_strchr(const char* a, int b)
{
    return strchr(a,b);
}

char* CString_strrchr(const char* a, int b)
{
    return strrchr(a,b);
}

int CString_strcasecmp(const char *a, const char *b)
{
    return strcasecmp(a,b);
}

char* CString_strstr(const char* haystack, const char* needle)
{
    return strstr(haystack,needle);
}

char* CString_strcpy(char* restrict dest, const char* restrict src)
{
    return strcpy(dest, src);
}

char* CString_strncpy(char* restrict dest, const char *restrict src, size_t n)
{
    return strncpy(dest, src, n);
}
