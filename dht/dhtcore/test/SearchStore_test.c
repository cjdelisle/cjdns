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
#include "memory/BufferAllocator.h"

// The code which we are testing.
#define IS_TESTING
#include "dht/dhtcore/SearchStore.c"

#include <stdio.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    char buffer[1024];
    struct Allocator* allocator = BufferAllocator_new(buffer, 1024);
    struct SearchNodeIndex index = {
        rand() % SearchStore_MAX_SEARCHES, rand() % SearchStore_SEARCH_NODES
    };
    String* str = tidForSearchNodeIndex(&index, allocator);
    struct SearchNodeIndex index2 = searchNodeIndexForTid(str);
    if (index.search - index2.search + index.node - index2.node) {
        printf("inputs were %d and %d\noutputs were %d and %d\nintermediet was %s\nlength was: %d",
               index.search, index.node, index2.search, index2.node, str->bytes, (int) str->len);
        return -1;
    }
    return 0;
}
