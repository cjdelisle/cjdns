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

#undef memcpy
#ifndef string_memcpy
    #define memcpy string_internal_memcpy
#endif
#undef memmove
#ifndef string_memmove
    #define memmove string_internal_memmove
#endif
#undef memccpy
#ifndef string_memccpy
    #define memccpy string_internal_memccpy
#endif
#undef memset
#ifndef string_memset
    #define memset string_internal_memset
#endif
#undef memcmp
#ifndef string_memcmp
    #define memcmp string_internal_memcmp
#endif
#undef memchr
#ifndef string_memchr
    #define memchr string_internal_memchr
#endif
#undef strcpy
#ifndef string_strcpy
    #define strcpy string_internal_strcpy
#endif
#undef strncpy
#ifndef string_strncpy
    #define strncpy string_internal_strncpy
#endif
#undef strcat
#ifndef string_strcat
    #define strcat string_internal_strcat
#endif
#undef strncat
#ifndef string_strncat
    #define strncat string_internal_strncat
#endif
#undef strcmp
#ifndef string_strcmp
    #define strcmp string_internal_strcmp
#endif
#undef strncmp
#ifndef string_strncmp
    #define strncmp string_internal_strncmp
#endif
#undef strcoll
#ifndef string_strcoll
    #define strcoll string_internal_strcoll
#endif
#undef strxfrm
#ifndef string_strxfrm
    #define strxfrm string_internal_strxfrm
#endif
#undef strcoll_l
#ifndef string_strcoll_l
    #define strcoll_l string_internal_strcoll_l
#endif
#undef strxfrm_l
#ifndef string_strxfrm_l
    #define strxfrm_l string_internal_strxfrm_l
#endif
#undef strdup
#ifndef string_strdup
    #define strdup string_internal_strdup
#endif
#undef strndup
#ifndef string_strndup
    #define strndup string_internal_strndup
#endif
#undef strchr
#ifndef string_strchr
    #define strchr string_internal_strchr
#endif
#undef strrchr
#ifndef string_strrchr
    #define strrchr string_internal_strrchr
#endif
#undef strcspn
#ifndef string_strcspn
    #define strcspn string_internal_strcspn
#endif
#undef strspn
#ifndef string_strspn
    #define strspn string_internal_strspn
#endif
#undef strpbrk
#ifndef string_strpbrk
    #define strpbrk string_internal_strpbrk
#endif
#undef strstr
#ifndef string_strstr
    #define strstr string_internal_strstr
#endif
#undef strtok
#ifndef string_strtok
    #define strtok string_internal_strtok
#endif
#undef strtok_r
#ifndef string_strtok_r
    #define strtok_r string_internal_strtok_r
#endif
#undef strlen
#ifndef string_strlen
    #define strlen string_internal_strlen
#endif
#undef strnlen
#ifndef string_strnlen
    #define strnlen string_internal_strnlen
#endif
#undef strerror
#ifndef string_strerror
    #define strerror string_internal_strerror
#endif
#undef strerror_r
#ifndef string_strerror_r
    #define strerror_r string_internal_strerror_r
#endif
#undef strerror_l
#ifndef string_strerror_l
    #define strerror_l string_internal_strerror_l
#endif
#undef bcopy
#ifndef string_bcopy
    #define bcopy string_internal_bcopy
#endif
#undef bzero
#ifndef string_bzero
    #define bzero string_internal_bzero
#endif
#undef bcmp
#ifndef string_bcmp
    #define bcmp string_internal_bcmp
#endif
#undef index
#ifndef string_index
    #define index string_internal_index
#endif
#undef rindex
#ifndef string_rindex
    #define rindex string_internal_rindex
#endif
#undef ffs
#ifndef string_ffs
    #define ffs string_internal_ffs
#endif
#undef strcasecmp
#ifndef string_strcasecmp
    #define strcasecmp string_internal_strcasecmp
#endif
#undef strncasecmp
#ifndef string_strncasecmp
    #define strncasecmp string_internal_strncasecmp
#endif
#undef strsep
#ifndef string_strsep
    #define strsep string_internal_strsep
#endif
#undef strsignal
#ifndef string_strsignal
    #define strsignal string_internal_strsignal
#endif
#undef stpcpy
#ifndef string_stpcpy
    #define stpcpy string_internal_stpcpy
#endif
#undef stpncpy
#ifndef string_stpncpy
    #define stpncpy string_internal_stpncpy
#endif

#include <string.h>

#ifndef string_memcpy
    #undef memcpy
#endif
#ifndef string_memmove
    #undef memmove
#endif
#ifndef string_memccpy
    #undef memccpy
#endif
#ifndef string_memset
    #undef memset
#endif
#ifndef string_memcmp
    #undef memcmp
#endif
#ifndef string_memchr
    #undef memchr
#endif
#ifndef string_strcpy
    #undef strcpy
#endif
#ifndef string_strncpy
    #undef strncpy
#endif
#ifndef string_strcat
    #undef strcat
#endif
#ifndef string_strncat
    #undef strncat
#endif
#ifndef string_strcmp
    #undef strcmp
#endif
#ifndef string_strncmp
    #undef strncmp
#endif
#ifndef string_strcoll
    #undef strcoll
#endif
#ifndef string_strxfrm
    #undef strxfrm
#endif
#ifndef string_strcoll_l
    #undef strcoll_l
#endif
#ifndef string_strxfrm_l
    #undef strxfrm_l
#endif
#ifndef string_strdup
    #undef strdup
#endif
#ifndef string_strndup
    #undef strndup
#endif
#ifndef string_strchr
    #undef strchr
#endif
#ifndef string_strrchr
    #undef strrchr
#endif
#ifndef string_strcspn
    #undef strcspn
#endif
#ifndef string_strspn
    #undef strspn
#endif
#ifndef string_strpbrk
    #undef strpbrk
#endif
#ifndef string_strstr
    #undef strstr
#endif
#ifndef string_strtok
    #undef strtok
#endif
#ifndef string_strtok_r
    #undef strtok_r
#endif
#ifndef string_strlen
    #undef strlen
#endif
#ifndef string_strnlen
    #undef strnlen
#endif
#ifndef string_strerror
    #undef strerror
#endif
#ifndef string_strerror_r
    #undef strerror_r
#endif
#ifndef string_strerror_l
    #undef strerror_l
#endif
#ifndef string_bcopy
    #undef bcopy
#endif
#ifndef string_bzero
    #undef bzero
#endif
#ifndef string_bcmp
    #undef bcmp
#endif
#ifndef string_index
    #undef index
#endif
#ifndef string_rindex
    #undef rindex
#endif
#ifndef string_ffs
    #undef ffs
#endif
#ifndef string_strcasecmp
    #undef strcasecmp
#endif
#ifndef string_strncasecmp
    #undef strncasecmp
#endif
#ifndef string_strsep
    #undef strsep
#endif
#ifndef string_strsignal
    #undef strsignal
#endif
#ifndef string_stpcpy
    #undef stpcpy
#endif
#ifndef string_stpncpy
    #undef stpncpy
#endif

#endif
