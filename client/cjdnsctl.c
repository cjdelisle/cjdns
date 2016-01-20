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
#include "benc/serialization/standard/BencMessageReader.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/events/EventBase.h"
#include "util/CString.h"

#include <stdio.h>

struct Context
{
    struct Allocator* alloc;
    struct EventBase* base;
    int argc;
    char** argv;
};

static void usage()
{
    printf("Usage: cjdnsctl [ OPTIONS ] OBJECT { COMMAND | help }\n");
    printf("where  OBJECT := { iptun }\n");
    printf("       OPTIONS := { -f { cjdns-binary-config-file }\n");
}

static void iptunUsage()
{
    printf("Usage: cjdnsctl iptun add KEY APA [ APA ] [ comment COMMENT ]\n");
    printf("       cjdnsctl iptun rm KEY\n");
    printf("       cjdnsctl iptun ls\n");
    printf("       cjdnsctl iptun help\n");
    printf("where  KEY := base-32 encoded string ending in .k\n");
    printf("       APA := ADDRESS[ [ /PREFIX ] /ALLOCATION ]\n");
    printf("       PREFIX := 0-32 for IPv4, 0-128 for IPv6 (announced to client)\n");
    printf("       ALLOCATION := 0-32 for IPv4, 0-129 for IPv6 (issued to client)\n");
    printf("       COMMENT := any string\n");
}

static void iptunList(struct Context* ctx)
{
}

static void iptunRemove(struct Context* ctx, char* key)
{
}

static void iptunAdd(struct Context* ctx, int beginArgs)
{
}

static void iptun(struct Context* ctx)
{
    int cmd = -1;
    for (int i = 0; i < ctx->argc; i++) {
        if (!CString_strcmp("iptun", ctx->argv[i])) {
            cmd = i + 1;
            break;
        }
    }
    if (cmd >= ctx->argc) {
        iptunUsage();
    } else if (!CString_strcmp("add", ctx->argv[cmd])) {
        iptunAdd(ctx, cmd+1);
    } else if (!CString_strcmp("rm", ctx->argv[cmd])) {
        if (cmd+2 != ctx->argc) {
            printf("ERROR `cjdnsctl iptun rm` takes 1 argument\n");
        } else {
            iptunRemove(ctx, ctx->argv[cmd+1]);
        }
    } else if (!CString_strcmp("ls", ctx->argv[cmd])) {
        if (cmd+1 != ctx->argc) {
            printf("ERROR `cjdnsctl iptun ls` takes no arguments\n");
        } else {
            iptunList(ctx);
        }
    }
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    ctx->alloc = alloc;
    ctx->argc = argc;
    ctx->argv = argv;
    //struct EventBase* base = ctx->base = EventBase_new(alloc);

    if (argc < 2) {
        // fallthrough
        usage();
    } else if (!CString_strcmp("iptun", argv[1])) {
        iptun(ctx);
    }
}
