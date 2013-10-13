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

#define Except_BUFFER_SZ 1024

struct Except;
struct Except
{
    struct Except* next;

    void (* exception)(char* message, int code, struct Except* handler);

    char message[Except_BUFFER_SZ];
};

Gcc_NORETURN
Gcc_PRINTF(3, 4)
void Except_raise(struct Except* eh, int code, char* format, ...);

Gcc_NORETURN
Gcc_PRINTF(3, 4)
void Except__throw(char* file, int line, char* format, ...);
#define Except_throw(...) Except__throw(__FILE__, __LINE__, __VA_ARGS__)

/** Set the default handler, make sure to null it after you are finished! */
void Except_setDefaultHandler(struct Except* eh);

#endif
