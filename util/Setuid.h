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
#ifndef Setuid_H
#define Setuid_H

#include "memory/Allocator.h"
#include "exception/Except.h"
#include "util/Linker.h"
#define Setuid_IMPL "util/Setuid_" + builder.config.systemName + ".c"

void Setuid_preSetuid(struct Allocator* alloc, struct Except* eh);
void Setuid_postSetuid(struct Allocator* alloc, struct Except* eh);

<?js
    var done = this.async();
    require("fs").exists(Setuid_IMPL, function (exists) {
        var out = "";
        if (!exists) {
            out = "void Setuid_preSetuid(struct Allocator* alloc, struct Except* eh) { }\n" +
                "void Setuid_postSetuid(struct Allocator* alloc, struct Except* eh) { }";
            console.log("No setuid keepNetAdmin");
        } else {
            file.links.push(Setuid_IMPL);
            console.log("Has setuid keepNetAdmin");
        }
        done(out);
    });
?>

#endif
