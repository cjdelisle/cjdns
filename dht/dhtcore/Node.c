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

#include "dht/dhtcore/Node.h"
#include "switch/EncodingScheme.h"
#include "util/Assert.h"
#include "util/Bits.h"

bool Node_isAncestorOf(struct Node_Two* ancestor, struct Node_Two* child)
{
    struct Node_Two* nn = child;
    for (;;) {
        if (nn == ancestor) { return true; }
        struct Node_Link* next = Node_getBestParent(nn);
        if (!next || next->parent == nn) { return false; }
        nn = next->parent;
    }
}

void Node_setParentCostAndPath(struct Node_Two* node,
                               struct Node_Link* bestParent,
                               uint64_t cost,
                               uint64_t path)
{
    if (bestParent) {
        Assert_true(bestParent->child == node);
        Assert_true(cost != UINT64_MAX);
        Assert_true(path != UINT64_MAX);
        if (bestParent->parent == node) {
            Assert_true(cost == 0);
            Assert_true(path == 1);
        } else {
            Assert_true(!Node_isAncestorOf(node, bestParent->parent));
        }
    } else {
        Assert_true(cost == UINT64_MAX);
        Assert_true(path == UINT64_MAX);
    }
    node->bestParent_pvt = bestParent;
    node->cost_pvt = cost;
    node->address.path = path;
}

bool Node_isOneHopLink(struct Node_Link* link)
{
    struct EncodingScheme* ps = link->parent->encodingScheme;
    int num = EncodingScheme_getFormNum(ps, link->cannonicalLabel);
    Assert_true(num > -1 && num < ps->count);
    return EncodingScheme_formSize(&ps->forms[num]) == Bits_log2x64(link->cannonicalLabel);
}
