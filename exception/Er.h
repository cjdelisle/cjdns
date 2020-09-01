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
#ifndef Er_H
#define Er_H

#include "memory/Allocator.h"
#include "util/Gcc.h"

#include "./util/Linker.h"
Linker_require("exception/Er.c")

struct Er_Ret
{
    const char* message;
};

<?js
	request("Er_JS", "./exception/Er.js");
	if (!file.Er_JS.defun) file.Er_JS = file.Er_JS.create();
?>

#define Er_DEFUN(...) \
    Gcc_USE_RET <?js return file.Er_JS.defun(Js_Q __VA_ARGS__ Js_Q) ?>

Gcc_PRINTF(4, 5)
Gcc_USE_RET
struct Er_Ret* Er__raise(char* file, int line, struct Allocator* alloc, char* format, ...);
#define Er_raise(...) \
    do { \
        struct Er_Ret* Er_ret = Er__raise(Gcc_SHORT_FILE, Gcc_LINE, __VA_ARGS__); \
        return Er_ret; \
    } while (0)
    // CHECKFILES_IGNORE missing ;

#define Er(expr) <?js return file.Er_JS.er(Js_Q expr Js_Q, Gcc_SHORT_FILE, Gcc_LINE); ?>

#define Er_assert(expr) \
    <?js return file.Er_JS.assert(Js_Q expr Js_Q, Gcc_SHORT_FILE, Gcc_LINE); ?>

#define Er_check(ret, expr) \
    <?js return file.Er_JS.check(#ret, Js_Q expr Js_Q, Gcc_SHORT_FILE, Gcc_LINE); ?>

#define Er_ret(val) <?js return file.Er_JS.ret(Js_Q val Js_Q); ?>

static inline struct Er_Ret* Er_unwind(const char* file, int line, struct Er_Ret* ret)
{
    return ret;
}

void Er__assertFail(struct Er_Ret* er);

#endif