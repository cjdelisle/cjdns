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
#include "util/Identity.h"
#include "util/Assert.h"

struct Thing
{
    int counter;
    Identity
};

static struct Thing* increment(struct Thing* t)
{
    t->counter++;
    return t;
}

int main()
{
    struct Thing t = {
        .counter = 0
    };
    struct Thing* tp = &t;
    Identity_set(tp);
    tp = Identity_check(increment(tp));
    Assert_true(tp == &t);
    Assert_true(t.counter == 1);
    return 0;
}
