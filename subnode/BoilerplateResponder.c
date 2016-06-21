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
#include "subnode/BoilerplateResponder.h"
#include "util/Identity.h"

struct BoilerplateResponder_pvt
{
    struct BoilerplateResponder pub;
    struct EncodingScheme* myScheme;
    String* mySchemeStr;
    Identity
};

void BoilerplateResponder_addBoilerplate(struct BoilerplateResponder* br,
                                         Dict* msgDict,
                                         struct Address* src,
                                         struct Allocator* alloc)
{
    struct BoilerplateResponder_pvt* brp = Identity_check((struct BoilerplateResponder_pvt*) br);
    Dict_putStringC(msgDict, "es", brp->mySchemeStr, alloc);
    int encIdx = EncodingScheme_getFormNum(brp->myScheme, src->path);
    Assert_true(encIdx != EncodingScheme_getFormNum_INVALID);
    Dict_putIntC(msgDict, "ei", encIdx, alloc);
}

struct BoilerplateResponder* BoilerplateResponder_new(struct EncodingScheme* myScheme,
                                                      struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct BoilerplateResponder_pvt* brp =
        Allocator_calloc(alloc, sizeof(struct BoilerplateResponder_pvt), 1);
    Identity_set(brp);
    brp->myScheme = myScheme;
    brp->mySchemeStr = EncodingScheme_serialize(myScheme, alloc);
    return &brp->pub;
}
