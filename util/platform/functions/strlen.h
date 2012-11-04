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
#ifndef string_H
#define string_H

#define memcpy strlen_memcpy
#define memmove strlen_memmove
#define memccpy strlen_memccpy
#define memset strlen_memset
#define memcmp strlen_memcmp
#define memchr strlen_memchr
#define strcpy strlen_strcpy
#define strncpy strlen_strncpy
#define strcat strlen_strcat
#define strncat strlen_strncat
#define strcmp strlen_strcmp
#define strncmp strlen_strncmp
#define strcoll strlen_strcoll
#define strxfrm strlen_strxfrm
#define strcoll_l strlen_strcoll_l
#define strxfrm_l strlen_strxfrm_l
#define strdup strlen_strdup
#define strndup strlen_strndup
#define strchr strlen_strchr
#define strrchr strlen_strrchr
#define strcspn strlen_strcspn
#define strspn strlen_strspn
#define strpbrk strlen_strpbrk
#define strstr strlen_strstr
#define strtok strlen_strtok
#define strtok_r strlen_strtok_r
//#define strlen strlen_strlen
#define strnlen strlen_strnlen
#define strerror strlen_strerror
#define strerror_r strlen_strerror_r
#define strerror_l strlen_strerror_l
#define bcopy strlen_bcopy
#define bzero strlen_bzero
#define bcmp strlen_bcmp
#define index strlen_index
#define rindex strlen_rindex
#define ffs strlen_ffs
#define strcasecmp strlen_strcasecmp
#define strncasecmp strlen_strncasecmp
#define strsep strlen_strsep
#define strsignal strlen_strsignal
#define stpcpy strlen_stpcpy
#define stpncpy strlen_stpncpy

#include <string.h>

#undef memcpy
#undef memmove
#undef memccpy
#undef memset
#undef memcmp
#undef memchr
#undef strcpy
#undef strncpy
#undef strcat
#undef strncat
#undef strcmp
#undef strncmp
#undef strcoll
#undef strxfrm
#undef strcoll_l
#undef strxfrm_l
#undef strdup
#undef strndup
#undef strchr
#undef strrchr
#undef strcspn
#undef strspn
#undef strpbrk
#undef strstr
#undef strtok
#undef strtok_r
//#undef strlen
#undef strnlen
#undef strerror
#undef strerror_r
#undef strerror_l
#undef bcopy
#undef bzero
#undef bcmp
#undef index
#undef rindex
#undef ffs
#undef strcasecmp
#undef strncasecmp
#undef strsep
#undef strsignal
#undef stpcpy
#undef stpncpy

#endif
