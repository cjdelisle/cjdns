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
#include "util/Setuid.h"
#include "memory/Allocator.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <linux/capability.h>
#include <sys/prctl.h>

#ifndef CAP_TO_MASK
#define CAP_TO_MASK(X) (1 << ((X) & 31))
#endif

#ifndef _LINUX_CAPABILITY_VERSION_3
#define _LINUX_CAPABILITY_VERSION_3 0x20080522
#endif

#define PERMITTED_MASK CAP_TO_MASK(CAP_NET_ADMIN)

static inline int capSet(cap_user_header_t hdr, cap_user_data_t data)
{
    int capset(cap_user_header_t hdr, cap_user_data_t data);
    return capset(hdr, data);
}

static inline int capGet(cap_user_header_t hdr, cap_user_data_t data)
{
    int capget(cap_user_header_t hdr, cap_user_data_t data);
    return capget(hdr, data);
}

Err_DEFUN Setuid_preSetuid(struct Allocator* alloc)
{
    cap_user_header_t hdr = Allocator_calloc(alloc, sizeof(*hdr), 1);
    cap_user_data_t data = Allocator_calloc(alloc, sizeof(*data), 2);

    hdr->version = _LINUX_CAPABILITY_VERSION_3;
    hdr->pid = 0;
    if (capGet(hdr, data)) {
        Err_raise(alloc, "Error getting capabilities: [errno:%d (%s)]", errno, strerror(errno));
    }

    data->permitted &= PERMITTED_MASK | CAP_TO_MASK(CAP_SETUID) | CAP_TO_MASK(CAP_SETGID);
    data->effective = data->permitted;
    data->inheritable = 0;
    if (capSet(hdr, data)) {
        Err_raise(alloc, "Error setting capabilities: [errno:%d (%s)]", errno, strerror(errno));
    }

    if (prctl(PR_SET_KEEPCAPS, 1)) {
        Err_raise(alloc, "Error keeping capabilities: [errno:%d (%s)]", errno, strerror(errno));
    }
    return NULL;
}

Err_DEFUN Setuid_postSetuid(struct Allocator* alloc)
{
    cap_user_header_t hdr = Allocator_calloc(alloc, sizeof(*hdr), 1);
    cap_user_data_t data = Allocator_calloc(alloc, sizeof(*data), 2);

    hdr->version = _LINUX_CAPABILITY_VERSION_3;
    hdr->pid = 0;
    if (capGet(hdr, data)) {
        Err_raise(alloc, "Error getting capabilities (post-setuid): [errno:%d (%s)]",
                     errno, strerror(errno));
    }

    data->permitted &= PERMITTED_MASK;
    data->effective = data->permitted;
    data->inheritable = 0;
    if (capSet(hdr, data)) {
        Err_raise(alloc, "Error setting capabilities (post-setuid): [errno:%d (%s)]",
                     errno, strerror(errno));
    }

    if (prctl(PR_SET_KEEPCAPS, 0)) {
        Err_raise(alloc, "Error un-keeping capabilities (post-setuid): [errno:%d (%s)]",
                     errno, strerror(errno));
    }
    return NULL;
}
