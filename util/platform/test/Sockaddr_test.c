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
#include "memory/MallocAllocator.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/CString.h"

static void expectFailure(char* address)
{
    struct Sockaddr_storage ss;
    Assert_true(Sockaddr_parse(address, &ss));
}

static void expectConvert(char* address, char* expectedOutput)
{
    struct Sockaddr_storage ss;
    Assert_true(!Sockaddr_parse(address, &ss));
    struct Allocator* alloc = MallocAllocator_new(20000);
    char* outAddr = Sockaddr_print(&ss.addr, alloc);
    Assert_true(outAddr);
    Assert_true(CString_strlen(outAddr) == CString_strlen(expectedOutput));
    Assert_true(!CString_strcmp(outAddr, expectedOutput));
    Allocator_free(alloc);
}

static void expectSuccess(char* address)
{
    expectConvert(address, address);
}

static void parse()
{
    struct Sockaddr_storage test;
    Assert_true(Sockaddr_asNative(&test.addr) == ((uint8_t*)&test) + Sockaddr_OVERHEAD);


    expectSuccess("0.0.0.0");
    expectSuccess("111.111.111.111");
    expectSuccess("111.111.111.111:12345");

    expectFailure("111.111.111.111:99999");
    expectFailure("[111.111.111.111]");
    expectFailure("[fc00::");

    expectConvert("[fc00::]", "fc00::");
    expectSuccess("fc00::");
    expectSuccess("::");
    expectSuccess("1::1");
    expectSuccess("1::1");
    expectSuccess("[1::1]:12345");
    expectFailure("[1::1]:99999");
    expectSuccess("[ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff]:12345");
    expectFailure("[ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff]:12345");
    expectFailure("[:]:12345");
    expectFailure("[0]:12345");
    expectFailure("0");
    expectFailure("1.0.0.");
}


int main()
{
    parse();
    return 0;
}
