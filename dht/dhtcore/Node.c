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

bool Node_isOneHopLink(struct Node_Link* link)
{
    struct EncodingScheme* ps = link->parent->encodingScheme;
    int num = EncodingScheme_getFormNum(ps, link->cannonicalLabel);
    Assert_always(num > -1 && num < ps->count);
    return EncodingScheme_formSize(&ps->forms[num]) == Bits_log2x64(link->cannonicalLabel);
}
