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
#ifndef Seccomp_H
#define Seccomp_H

#include "exception/Except.h"

<?js
    var main = function (async) {
        if (typeof(builder.config.HAS_SECCOMP) !== 'undefined') {
            if (builder.config.HAS_SECCOMP) {
                file.links.push("util/Seccomp.c");
                return [
                    "void Seccomp_dropPermissions(struct Except* eh);",
                    "int Seccomp_isWorking();",
                    "static inline int Seccomp_exists() { return 1; }",
                ].join('\n');
            } else {
                return [
                    "static inline void Seccomp_dropPermissions(struct Except* eh) { }",
                    "static inline int Seccomp_isWorking() { return 0; }",
                    "static inline int Seccomp_exists() { return 0; }",
                ].join('\n');
            }
        }

        console.log("Searching for SECCOMP");
        var done = async();

        var HasFunction = require("./HasFunction");

        HasFunction.check(builder, "seccomp_init", ["-lseccomp"], function (err, has) {
            builder.config.HAS_SECCOMP = (!err && has);
            if (has) {
                console.log("Successfully found SECCOMP");
                builder.config.libs.push("-lseccomp");
            } else {
                console.log("Could not find SECCOMP, skipping");
            }
            done(main());
        });
    };
    return main(this.async);
?>

#endif
