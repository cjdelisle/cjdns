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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "util/GlobalConfig.h"
#include "util/Identity.h"

struct GlobalConfig_pvt {
    struct GlobalConfig pub;
    String* tunName;
    struct Allocator* alloc;
    Identity
};

String* GlobalConfig_getTunName(struct GlobalConfig* conf)
{
    struct GlobalConfig_pvt* ctx = Identity_check((struct GlobalConfig_pvt*) conf);
    return ctx->tunName;
}

void GlobalConfig_setTunName(struct GlobalConfig* conf, String* name)
{
    struct GlobalConfig_pvt* ctx = Identity_check((struct GlobalConfig_pvt*) conf);
    // This is a memory leak but hopefully this function doesn't get called too much...
    ctx->tunName = String_clone(name, ctx->alloc);
}

struct GlobalConfig* GlobalConfig_new(struct Allocator* alloc)
{
    struct GlobalConfig_pvt* gcp = Allocator_calloc(alloc, sizeof(struct GlobalConfig_pvt), 1);
    Identity_set(gcp);
    gcp->alloc = alloc;
    return &gcp->pub;
}