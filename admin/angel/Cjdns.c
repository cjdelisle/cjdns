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
#define string_strlen // some systems implement strcmp as a macro using strlen.
#include "admin/angel/AngelInit.h"
#include "admin/angel/Core.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (isatty(STDIN_FILENO) || argc < 2) {
        // Fall through.
    } else if (!CString_strcmp("angel", argv[1])) {
        return AngelInit_main(argc, argv);
    } else if (!CString_strcmp("core", argv[1])) {
        return Core_main(argc, argv);
    }
    printf("This is internal to cjdns, it should not be started manually.\n");
    return -1;
}
