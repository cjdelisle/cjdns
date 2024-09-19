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
#ifndef Object_H
#define Object_H

#include <stdint.h>

// Dictionaries and lists are pointers to the head entry so that the head can change.
typedef struct Dict_Entry* Dict;
typedef struct List_Item* List;
typedef struct String_s {
    uintptr_t len;
    char* bytes;
} String;

typedef String String_t;
typedef Dict Dict_t;
typedef List List_t;

enum Object_Type {
    Object_INTEGER,
    Object_STRING,
    Object_LIST,
    Object_DICT,
    Object_UNPARSABLE
};

typedef struct {
    enum Object_Type type;
    union {
        int64_t number;
        String_t* string;
        List_t* list;
        Dict_t* dictionary;
    } as;
} Object;

typedef Object Object_t;

#endif
