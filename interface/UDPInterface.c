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
#include "util/events/libuv/UvWrapper.h"
#include "benc/StringList.h"
#include "interface/UDPInterface.h"
#include "wire/Message.h"
#include "util/events/UDPAddrIface.h"
#include "util/GlobalConfig.h"

#define ArrayList_TYPE struct Sockaddr
#define ArrayList_NAME Sockaddr
#include "util/ArrayList.h"

struct UDPInterface_pvt
{
    struct UDPInterface pub;
    struct Log* log;
    struct Allocator* allocator;

    struct Allocator* bcastAddrAlloc;
    struct ArrayList_Sockaddr* bcastAddrs;

    struct Allocator* bcastIfaceAlloc;
    struct StringList* bcastIfaces;

    struct UDPAddrIface* commIf;
    struct UDPAddrIface* bcastIf;

    struct GlobalConfig* globalConf;

    struct Iface commSock;
    struct Iface bcastSock;
    uint16_t beaconPort_be;
    uint16_t commPort_be;

    Identity
};

static struct Sockaddr* mkBcastAddr(
    uint16_t beaconPort_be,
    uv_interface_address_t* iface,
    struct Allocator* alloc)
{
    struct sockaddr_in bcast4 = {
        .sin_family = AF_INET,
        .sin_port = beaconPort_be,
        .sin_addr = {
            .s_addr =
                (
                    iface->address.address4.sin_addr.s_addr &
                    iface->netmask.netmask4.sin_addr.s_addr
                ) | ~iface->netmask.netmask4.sin_addr.s_addr
        }
    };
    return Sockaddr_fromNative(&bcast4, sizeof(struct sockaddr_in), alloc);
}

static void updateBcastAddrs(struct UDPInterface_pvt* ctx)
{
    bool all = false;
    for (int i = 0; ctx->bcastIfaces && i < ctx->bcastIfaces->length; i++) {
        String* iface = StringList_get(ctx->bcastIfaces, i);
        if (String_equals(iface, String_CONST("all"))) { all = true; }
    }
    uv_interface_address_t* interfaces;
    int count;
    int res = uv_interface_addresses(&interfaces, &count);
    if (res) {
        Log_info(ctx->log, "uv_interface_addresses failed [%s]", uv_strerror(-res));
        return;
    }

    if (ctx->bcastAddrAlloc) { Allocator_free(ctx->bcastAddrAlloc); }
    struct Allocator* alloc = ctx->bcastAddrAlloc = Allocator_child(ctx->allocator);
    ctx->bcastAddrs = ArrayList_Sockaddr_new(alloc);

    String* tunDev = GlobalConfig_getTunName(ctx->globalConf);

    for (int i = 0; i < count; i++) {
        if (interfaces[i].is_internal) { continue; }
        if (interfaces[i].address.address4.sin_family != AF_INET) { continue; }
        if (tunDev && !CString_strncmp(interfaces[i].name, tunDev->bytes, tunDev->len)) {
            continue;
        }
        struct Sockaddr* addr = mkBcastAddr(ctx->beaconPort_be, &interfaces[i], alloc);
        if (!all) {
            String* addrStr = String_new(Sockaddr_print(addr, alloc), alloc);
            bool found = false;
            for (int j = 0; ctx->bcastIfaces && j < ctx->bcastIfaces->length; j++) {
                String* iface = StringList_get(ctx->bcastIfaces, j);
                if (String_equals(iface, addrStr)) { found = true; }
                if (String_equals(iface, String_CONST(interfaces[i].name))) { found = true; }
            }
            if (!found) { continue; }
        }
        ArrayList_Sockaddr_add(ctx->bcastAddrs, addr);
    }
    uv_free_interface_addresses(interfaces, count);
}

static Iface_DEFUN sendPacket(struct Message* m, struct Iface* iface)
{
    struct UDPInterface_pvt* ctx =
        Identity_containerOf(iface, struct UDPInterface_pvt, pub.generic.iface);

    Assert_true(m->length > Sockaddr_OVERHEAD);
    struct Sockaddr* sa = (struct Sockaddr*) m->bytes;
    Assert_true(m->length > sa->addrLen);

    // Regular traffic
    if (!(sa->flags & Sockaddr_flags_BCAST)) { return Iface_next(&ctx->commSock, m); }

    updateBcastAddrs(ctx);

    // bcast
    struct UDPInterface_BroadcastHeader hdr = {
        .fffffffc_be = Endian_hostToBigEndian32(0xfffffffc),
        .version = UDPInterface_CURRENT_VERSION,
        .zero = 0,
        .commPort_be = ctx->commPort_be
    };
    Message_shift(m, -sa->addrLen, NULL);
    Message_push(m, &hdr, UDPInterface_BroadcastHeader_SIZE, NULL);

    for (int i = 0; i < ctx->bcastAddrs->length; i++) {
        struct Allocator* tmpAlloc = Allocator_child(ctx->allocator);
        struct Message* mm = Message_clone(m, tmpAlloc);
        struct Sockaddr* addr = ArrayList_Sockaddr_get(ctx->bcastAddrs, i);
        Message_push(mm, addr, addr->addrLen, NULL);
        Iface_send(&ctx->bcastSock, mm);
        Allocator_free(tmpAlloc);
    }

    return NULL;
}

static Iface_DEFUN fromCommSock(struct Message* m, struct Iface* iface)
{
    struct UDPInterface_pvt* ctx =
        Identity_containerOf(iface, struct UDPInterface_pvt, commSock);
    return Iface_next(&ctx->pub.generic.iface, m);
}

static Iface_DEFUN fromBcastSock(struct Message* m, struct Iface* iface)
{
    struct UDPInterface_pvt* ctx =
        Identity_containerOf(iface, struct UDPInterface_pvt, bcastSock);

    if (m->length < UDPInterface_BroadcastHeader_SIZE + Sockaddr_OVERHEAD) {
        Log_debug(ctx->log, "DROP runt bcast");
        return NULL;
    }

    struct Sockaddr_storage ss;
    Message_pop(m, &ss, Sockaddr_OVERHEAD, NULL);
    if (m->length < UDPInterface_BroadcastHeader_SIZE + ss.addr.addrLen - Sockaddr_OVERHEAD) {
        Log_debug(ctx->log, "DROP runt bcast");
        return NULL;
    }
    Message_pop(m, &ss.nativeAddr, ss.addr.addrLen - Sockaddr_OVERHEAD, NULL);

    struct UDPInterface_BroadcastHeader hdr;
    Message_pop(m, &hdr, UDPInterface_BroadcastHeader_SIZE, NULL);

    if (hdr.fffffffc_be != Endian_hostToBigEndian32(0xfffffffc)) {
        Log_debug(ctx->log, "DROP bcast bad magic, expected 0xfffffffc got [%08x]",
            Endian_bigEndianToHost32(hdr.fffffffc_be));
        return NULL;
    }

    if (hdr.version != UDPInterface_CURRENT_VERSION) {
        Log_debug(ctx->log, "DROP bcast bad version [%u]", hdr.version);
        return NULL;
    }

    if (hdr.zero) {
        Log_debug(ctx->log, "DROP bcast malformed (zero not zero)");
        return NULL;
    }

    uint16_t commPort = Endian_bigEndianToHost16(hdr.commPort_be);

    // Fake that it came from the communication port
    Sockaddr_setPort(&ss.addr, commPort);
    ss.addr.flags |= Sockaddr_flags_BCAST;

    Message_push(m, &ss.addr, ss.addr.addrLen, NULL);

    return Iface_next(&ctx->pub.generic.iface, m);
}

struct UDPInterface* UDPInterface_new(struct EventBase* eventBase,
                                      struct Sockaddr* bindAddr,
                                      uint16_t beaconPort,
                                      struct Allocator* alloc,
                                      struct Except* exHandler,
                                      struct Log* logger,
                                      struct GlobalConfig* globalConf)
{
    if (beaconPort && Sockaddr_getFamily(bindAddr) != Sockaddr_AF_INET) {
        Except_throw(exHandler, "UDP broadcast only supported by ipv4.");
    }
    if (beaconPort && Sockaddr_getPort(bindAddr) == beaconPort) {
        Except_throw(exHandler, "UDP broadcast port must be different from communication port.");
    }

    struct UDPAddrIface* uai = UDPAddrIface_new(eventBase, bindAddr, alloc, exHandler, logger);

    uint16_t commPort = Sockaddr_getPort(uai->generic.addr);

    struct UDPInterface_pvt* context = Allocator_calloc(alloc, sizeof(struct UDPInterface_pvt), 1);
    Identity_set(context);
    context->log = logger;
    context->allocator = alloc;
    context->beaconPort_be = Endian_hostToBigEndian16(beaconPort);
    context->commPort_be = Endian_hostToBigEndian16(commPort);
    context->pub.generic.addr = uai->generic.addr;
    context->pub.generic.alloc = alloc;
    context->pub.generic.iface.send = sendPacket;
    context->commSock.send = fromCommSock;
    context->bcastSock.send = fromBcastSock;
    context->commIf = uai;
    context->globalConf = globalConf;
    Iface_plumb(&uai->generic.iface, &context->commSock);

    if (beaconPort) {
        struct Sockaddr* bcastAddr = Sockaddr_clone(bindAddr, alloc);
        Sockaddr_setPort(bcastAddr, beaconPort);
        struct UDPAddrIface* bcast =
            UDPAddrIface_new(eventBase, bcastAddr, alloc, exHandler, logger);
        UDPAddrIface_setBroadcast(bcast, 1);
        Iface_plumb(&bcast->generic.iface, &context->bcastSock);
        context->bcastIf = bcast;
    }

    return &context->pub;
}

List* UDPInterface_listDevices(struct Allocator* alloc, struct Except* eh)
{
    List* out = List_new(alloc);
    uv_interface_address_t* interfaces;
    int count;
    int res = uv_interface_addresses(&interfaces, &count);
    if (res) { Except_throw(eh, "uv_interface_addresses failed [%s]", uv_strerror(-res)); }

    for (int i = 0; i < count; i++) {
        if (interfaces[i].is_internal) { continue; }
        if (interfaces[i].address.address4.sin_family != AF_INET) { continue; }
        List_addString(out, String_new(interfaces[i].name, alloc), alloc);
    }
    uv_free_interface_addresses(interfaces, count);
    return out;
}

void UDPInterface_setBroadcastDevices(struct UDPInterface* udpif, List* devices)
{
    struct UDPInterface_pvt* ctx = Identity_check((struct UDPInterface_pvt*) udpif);
    if (ctx->bcastIfaceAlloc) { Allocator_free(ctx->bcastIfaceAlloc); }
    struct Allocator* alloc = ctx->bcastIfaceAlloc = Allocator_child(ctx->allocator);
    struct StringList* bcastIfaces = ctx->bcastIfaces = StringList_new(alloc);
    int len = List_size(devices);
    for (uint32_t i = 0; i < (unsigned) len; i++) {
        String* dev = List_getString(devices, i);
        StringList_add(bcastIfaces, String_clone(dev, alloc));
    }
}

List* UDPInterface_getBroadcastDevices(struct UDPInterface* udpif, struct Allocator* alloc)
{
    struct UDPInterface_pvt* ctx = Identity_check((struct UDPInterface_pvt*) udpif);
    List* out = List_new(alloc);
    for (int i = 0; ctx->bcastIfaces && i < ctx->bcastIfaces->length; i++) {
        List_addString(out, StringList_get(ctx->bcastIfaces, i), alloc);
    }
    return out;
}

List* UDPInterface_getBroadcastAddrs(struct UDPInterface* udpif, struct Allocator* alloc)
{
    struct UDPInterface_pvt* ctx = Identity_check((struct UDPInterface_pvt*) udpif);
    updateBcastAddrs(ctx);
    List* out = List_new(alloc);
    for (int i = 0; i < ctx->bcastAddrs->length; i++) {
        char* addr = Sockaddr_print(ArrayList_Sockaddr_get(ctx->bcastAddrs, i), alloc);
        List_addStringC(out, addr, alloc);
    }
    return out;
}

int UDPInterface_setDSCP(struct UDPInterface* udpif, uint8_t dscp)
{
    struct UDPInterface_pvt* ctx = Identity_check((struct UDPInterface_pvt*) udpif);
    int res = UDPAddrIface_setDSCP(ctx->commIf, dscp);
    if (res) { return res; }
    if (ctx->bcastIf) { return UDPAddrIface_setDSCP(ctx->bcastIf, dscp); }
    return 0;
}