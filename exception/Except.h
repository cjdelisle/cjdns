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
#ifndef Except_H
#define Except_H

#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("exception/Except.c");

#define Except_BUFFER_SZ 1024

struct Except;
struct Except
{
    struct Except* next;

    void (* exception)(char* message, struct Except* handler);

    char message[Except_BUFFER_SZ];
};

Gcc_NORETURN
Gcc_PRINTF(4, 5)
void Except__throw(char* file, int line, struct Except* eh, char* format, ...);
#define Except_throw(...) Except__throw(Gcc_SHORT_FILE, Gcc_LINE, __VA_ARGS__)


#endif
