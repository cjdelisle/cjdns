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
#ifndef RootTest_H
#define RootTest_H

#include "util/CString.h"
#include <stdio.h>

#define RootTest_toStr(x) RootTest_toStr2(x)
#define RootTest_toStr2(x) #x
<?js file.RootTest_mainFunc = RootTest_toStr(main) ?>

#define RootTest_main <?js return 'RootTest_'+file.RootTest_mainFunc; ?>

int RootTest_main(int argc, char** argv);
int main(int argc, char** argv)
{
    int runIt = 0;
    int j = 0;
    for (int i = 0; i < argc; i++) {
        argv[j] = argv[i];
        if (!CString_strcmp("+roottest", argv[i])) {
            j--;
            runIt = 1;
        }
        j++;
    }
    if (runIt) {
        return RootTest_main(j,argv);
    } else {
        printf("Root test %s disabled, use [%s +roottest] to include it\n", RootTest_toStr(main),
               (argc > 0) ? argv[0] : "");
    }
    return 0;
}

#ifdef main
    #undef main
#endif

#define main RootTest_main  // CHECKFILES_IGNORE
int main(int argc, char** argv);

#endif
