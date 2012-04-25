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
#ifndef AbortHandler_H
#define AbortHandler_H

#include "exception/ExceptionHandler.h"

#include <stdio.h>

/** Internal callback, please use AbortHandler_INSTANCE instead. */
static void AbortHandler_callback(char* message, int code, struct ExceptionHandler* handler)
{
    fprintf(stderr, "Error: %s (code: %d)\n", message, code);
    abort();
}

/**
 * The exception handler.
 * Prints the message to stderr and aborts the program.
 */
static struct ExceptionHandler* AbortHandler_INSTANCE = &(struct ExceptionHandler) {
    .exception = AbortHandler_callback
};

#endif
