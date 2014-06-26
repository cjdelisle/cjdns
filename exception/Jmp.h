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
#ifndef Jmp_H
#define Jmp_H

#include "exception/Except.h"
#include <setjmp.h>

/**
 * setjmp based exception handler.
 *
 * struct Jmp jmp;
 * Jmp_try(jmp) {
 *    Do_somethingDangerous(&jmp.handler);
 * } Jmp_catch {
 *    printf("failed %d %s", jmp.code, jmp.message);
 * }
 *
 * Provides an easy way to implement the most basic try/catch functionality.
 * the jmp structure must not be used for anything outside of the try block,
 * calling jmp.handler.exception outside of a try block is undefined behavior.
 */

struct Jmp {
    /** The exception handler which will trigger the entry into the catch block. */
    struct Except handler;

    /** The exception message if in the catch block, otherwise undefined. */
    char* message;

    /** Internal setjmp buffer. */
    jmp_buf buf;
};

/** Internal callback, this should not be called directly. */
Gcc_NORETURN
static void Jmp_callback(char* message, struct Except* handler)
{
    struct Jmp* jmp = (struct Jmp*) handler;
    jmp->message = message;
    longjmp(jmp->buf, 1);
}

#define Jmp_try(jmp) \
    jmp.handler.exception = Jmp_callback; \
    if (!setjmp(jmp.buf))
// CHECKFILES_IGNORE squigly bracket will be added by the caller.

#define Jmp_catch else

#endif
