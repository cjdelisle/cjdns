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
#include <stdio.h>

<?js
    var done = this.async();
    var Tests = require("./Tests");
    Tests.get(function (tests) {
        var prototypes = [];
        var mainContent = [];
        tests.forEach(function (test) {
            var main = /^.*\/([^\/]+)\.c$/.exec(test)[1] + '_main';
            (state['cflags'+test] = state['cflags'+test] || []).push('-D', 'main='+main);
            file.links.push(test);
            mainContent.push('printf("'+test+'\\n");');
            mainContent.push(main+'(argc, argv);');
            prototypes.push('int '+main+'(int argc, char** argv);');
        });
        done(prototypes.join('\n'));
        process.nextTick(function() { mainDone(mainContent.join('\n')); });
    });
?>

int main(int argc, char** argv)
{
    <?js
        mainDone = this.async();
    ?>
}
