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

// This file is used to generate src/cffi.rs using bindgen

#include "benc/Object.h"
#include "benc/List.h"
#include "benc/Dict.h"
#include "interface/Iface.h"
#include "interface/test/RustIface_test.h"
#include "crypto/random/Random.h"
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "util/platform/Sockaddr.h"
#include "util/version/Version.h"
#include "wire/PFChan.h"
#include "wire/Control.h"

enum RBindings_Version {
    RBindings_Version_CurrentProtocol = Version_CURRENT_PROTOCOL,
};

// This structure is guaranteed to be present in the generated rust code
// Any types which are not transitively included in either this structure
// or in one of the functions will not be generated. This prevents generating
// a bunch of platform-specific trash like uint_fast8_t etc.
struct RBindings_Whitelist {
    Iface_t b;
    Message_t d;
    List_Item_t e;
    Dict_Entry_t ee;
    Object_t eee;
    Log_t* f;
    enum RBindings_Version g;
    Sockaddr_t h;
    PFChan_FromPathfinder_t i;
    PFChan_FromCore_t ii;
    enum Log_Level j;
    Allocator_t* k;
    struct Sockaddr_storage l;
    struct Sockaddr m;
    Random_t* n;
    Control_LlAddr_t o;
};