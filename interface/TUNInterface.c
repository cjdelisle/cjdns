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

#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "interface/TUNInterface_struct.h"
#include "benc/String.h"
#include "util/Endian.h"

#include <string.h>
#include <event2/event.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#ifdef __APPLE__
    #define APPLE_UTUN_CONTROL "com.apple.net.utun_control"
    #define INET6_ETHERTYPE PF_INET6
    #define UTUN_OPT_IFNAME 2

    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/kern_control.h>
    #include <sys/sys_domain.h>
    #include <sys/kern_event.h>
    #include <sys/errno.h>
    #include <netinet/if_ether.h>
#else
    #define INET6_ETHERTYPE ETH_P_IPV6

    #include <linux/if.h>
    #include <linux/if_tun.h>
    #include <linux/if_ether.h>
#endif

// Defined extra large so large MTU can be taken advantage of later.
#define MAX_PACKET_SIZE 8192
#define PADDING_SPACE (10240 - MAX_PACKET_SIZE)

// The number of bytes at the beginning of the message which is used
// to contain the type of packet.
#define PACKET_INFO_SIZE 4



static int openTunnel(const char* interfaceName, char assignedInterfaceName[IFNAMSIZ])
{
#ifdef __APPLE__
    int tunUnit = 0; /* allocate dynamically by default */

    if (interfaceName) {
        int parsedUnit = 0;

        if (sscanf(interfaceName, "utun%i", &parsedUnit) != 1 || parsedUnit < 0) {
            fprintf(stderr, "Invalid utun device %s. "
                "Remove this configuration parameter to use automatic assignment.\n", interfaceName);
            return -1;
        }

        tunUnit = parsedUnit + 1; /* device number used is unit - 1*/
    }

    fprintf(stderr, "Initializing utun interface: %s\n", interfaceName ? interfaceName : "auto");

    int tunFileDescriptor = socket(PF_SYSTEM, SOCK_DGRAM, SYSPROTO_CONTROL);
    if (tunFileDescriptor < 0) {
        return tunFileDescriptor;
    }

    /* get the utun control id */
    struct ctl_info info;
    memset(&info, 0, sizeof(info));
    strncpy(info.ctl_name, APPLE_UTUN_CONTROL, strlen(APPLE_UTUN_CONTROL));

    if (ioctl(tunFileDescriptor, CTLIOCGINFO, &info) < 0) {
        fprintf(stderr, "Error while setting options on kernel control: %s\n", strerror(errno));
        close(tunFileDescriptor);
        return -1;
    }

    /* connect the utun device */
    struct sockaddr_ctl addr;
    addr.sc_id = info.ctl_id;

    addr.sc_len = sizeof(addr);
    addr.sc_family = AF_SYSTEM;
    addr.ss_sysaddr = AF_SYS_CONTROL;
    addr.sc_unit = tunUnit;

    if (connect(tunFileDescriptor, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "Error attempting to connect to utun device: %s\n", strerror(errno));
        close(tunFileDescriptor);

        return -1;
    }

    /* retrieve the assigned utun interface name */
    uint32_t assignedInterfaceNameLength = sizeof(assignedInterfaceName);

    if (getsockopt(tunFileDescriptor, SYSPROTO_CONTROL, UTUN_OPT_IFNAME,
                   assignedInterfaceName, &assignedInterfaceNameLength) >= 0) {
        fprintf(stderr, "Initialized utun interface: %s\n", assignedInterfaceName);
    } else {
        perror("getsockopt");
    }

#else /* __APPLE__ */

    // Linux implementation
    fprintf(stderr, "Initializing tun device: ");
    if (interfaceName) {
        fprintf(stderr, "%s", interfaceName);
    }
    fprintf(stderr, "\n");

    int tunFileDescriptor;

    struct ifreq ifRequest;
    memset(&ifRequest, 0, sizeof(struct ifreq));
    ifRequest.ifr_flags = IFF_TUN;
    if (interfaceName) {
        strncpy(ifRequest.ifr_name, interfaceName, IFNAMSIZ);
    }
    tunFileDescriptor = open("/dev/net/tun", O_RDWR);

    if (tunFileDescriptor < 0) {
        return tunFileDescriptor;
    }

    int out = ioctl(tunFileDescriptor, TUNSETIFF, &ifRequest);
    if (out < 0) {
        close(tunFileDescriptor);
        return -1;
    }
    strcpy(assignedInterfaceName, ifRequest.ifr_name);

#endif /* __APPLE__ */
    return tunFileDescriptor;
}

static void closeInterface(void* vcontext)
{
    struct TUNInterface* tun = (struct TUNInterface*) vcontext;
    close(tun->fileDescriptor);
    event_del(tun->incomingEvent);
    event_free(tun->incomingEvent);
}

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext)
{
    // 292 bytes of extra padding to build headers back from for better efficiency.
    uint8_t messageBuffer[MAX_PACKET_SIZE + PADDING_SPACE];

    struct Message message = {
        .bytes = messageBuffer + PADDING_SPACE,
        .padding = PADDING_SPACE,
        .length = MAX_PACKET_SIZE
    };

    ssize_t length =
        read(socket, messageBuffer + PADDING_SPACE - PACKET_INFO_SIZE, MAX_PACKET_SIZE);

    if (length < 0) {
        printf("Error reading from TUN device %d\n", (int) length);
        return;
    }
    message.length = length - PACKET_INFO_SIZE;

    struct Interface* iface = &((struct TUNInterface*) vcontext)->interface;
    if (iface->receiveMessage) {
        iface->receiveMessage(&message, iface);
    }
}

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    // The type of packet we send, older kernels need this hint otherwise they assume it's ipv4.
    Message_shift(message, PACKET_INFO_SIZE);
    const uint16_t packetInfo[2] = { 0, Endian_bigEndianToHost16(INET6_ETHERTYPE) };
    memcpy(message->bytes, packetInfo, PACKET_INFO_SIZE);

    struct TUNInterface* tun = (struct TUNInterface*) iface->senderContext;
    ssize_t ret = write(tun->fileDescriptor, message->bytes, message->length);
    if (ret < 0) {
        printf("Error writing to TUN device %d\n", errno);
    }
    // TODO: report errors
    return 0;
}

struct TUNInterface* TUNInterface_new(String* interfaceName,
                                      struct event_base* base,
                                      struct Allocator* allocator)
{
    errno = 0;
    String* ifName = String_newBinary(NULL, IFNAMSIZ, allocator);

    int tunFileDesc = openTunnel(interfaceName ? interfaceName->bytes : NULL, ifName->bytes);
    if (tunFileDesc < 0) {
        if (errno == EPERM) {
            fprintf(stderr, "You don't have permission to open tunnel. "
                            "This probably needs to be run as root.\n");
        } else {
            fprintf(stderr, "Failed to open tunnel, error: %d\n", errno);
        }
        return NULL;
    }

    evutil_make_socket_nonblocking(tunFileDesc);

    struct TUNInterface* tun = allocator->malloc(sizeof(struct TUNInterface), allocator);
    tun->incomingEvent = event_new(base, tunFileDesc, EV_READ | EV_PERSIST, handleEvent, tun);
    tun->fileDescriptor = tunFileDesc;
    tun->name = ifName;

    if (tun->incomingEvent == NULL) {
        abort();
    }

    struct Interface iface = {
        .senderContext = tun,
        .sendMessage = sendMessage,
        .allocator = allocator,
        .requiredPadding = 0,
        .maxMessageLength = MAX_PACKET_SIZE
    };

    memcpy(&tun->interface, &iface, sizeof(struct Interface));

    event_add(tun->incomingEvent, NULL);

    allocator->onFree(closeInterface, tun, allocator);

    return tun;
}

struct Interface* TUNInterface_asGeneric(struct TUNInterface* tunIf)
{
    return (tunIf) ? &tunIf->interface : NULL;
}
