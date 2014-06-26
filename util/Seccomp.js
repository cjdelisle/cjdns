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

var TEST_PROGRAM = [
    "#include <sys/resource.h>",
    "#include <sys/prctl.h>",
    "#include <linux/filter.h>",
    "#include <linux/seccomp.h>",
    "#include <linux/audit.h>",
    "#include <sys/syscall.h>",
    "int main() {",
    "    return __NR_read",
    "        | PR_SET_NO_NEW_PRIVS | PR_SET_SECCOMP | AUDIT_ARCH_X86_64",
    "        | BPF_K | SECCOMP_MODE_FILTER;",
    "}"
].join('\n');

var pushLinks = function (builder) {
    builder.Seccomp_QUEUE.forEach(function (file) {
        if (builder.Seccomp_EXISTS) {
            file.links.push("util/Seccomp.c");
        } else {
            file.links.push("util/Seccomp_dummy.c");
        }
    });
    builder.Seccomp_QUEUE = undefined;
};

var detect = module.exports.detect = function (async, file, builder)
{
    if (typeof(builder.Seccomp_QUEUE) !== 'undefined') { builder.Seccomp_QUEUE.push(file); return; }
    builder.Seccomp_QUEUE = [ file ];
    if (typeof(builder.Seccomp_EXISTS) !== 'undefined') {
        pushLinks(builder);
        return;
    }

    console.log("Searching for SECCOMP");

    var hasSeccomp = false;
    if (builder.config.systemName !== 'linux') {
        console.log("SECCOMP is only available on linux");
    } else if (process.env['Seccomp_NO']) {
        console.log("SECCOMP disabled");
    } else {
        var done = async();
        var CanCompile = require('../node_build/CanCompile');
        var cflags = [ builder.config.cflags, '-x', 'c' ];
        CanCompile.check(builder, TEST_PROGRAM, cflags, function (err, can) {
            builder.Seccomp_EXISTS = !!can;
            if (!can) {
                console.log("Failed to get SECCOMP, compile failure: [" + err + "]");
            }
            pushLinks(builder);
            done();
        });
        return;
    }
    builder.Seccomp_EXISTS = hasSeccomp;
    pushLinks(builder);
};
