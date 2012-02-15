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
#ifdef _WIN32   
    // Windows
	#include "Windows.h"
#elif defined __linux__
    // Linux
	#include <linux/if.h>
	#include <linux/if_tun.h>
	#include <sys/ioctl.h>
#elif defined TARGET_OS_X
    // Mac  
#else
#endif
 
#include <string.h>
#include <event2/event.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "benc/Object.h"

// Defined extra large so large MTU can be taken advantage of later.
#define MAX_PACKET_SIZE 8192
#define PADDING_SPACE (10240 - MAX_PACKET_SIZE)

struct Tunnel
{
    struct event* incomingEvent;
    int fileDescriptor;
    struct Interface interface;
};

static int openTunnel(const char* interfaceName)
{
    fprintf(stderr, "Initializing tun device: ");
    if (interfaceName) {
        fprintf(stderr, "%s", interfaceName);
    }
    fprintf(stderr, "\n");
	
	#ifdef _WIN32
		// Windows
		HKEY hRegAdapters;
		char devGuid[64];
		LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}", 0,
			KEY_READ, &hRegAdapters);
		for (DWORD Index=0; ; Index++) {
			char subKeyName[255];
			DWORD cName = 255;
			LONG res = RegEnumKeyEx(hRegAdapters, Index, SubKeyName, &cName, 
				NULL, NULL, NULL, NULL);
			if (res != ERROR_SUCCESS) {
				tunFileDescriptor = -1;
				break;
			}
			// do something with Name
			char keyComponentId[64];
			DWORD valueBufferSize = 64;
			LONG res2 = RegGetValue(hSubKey, subKeyName, "ComponentId",
				RRF_RT_REG_SZ, NULL, keyComponentId, &valueBufferSize);
			if (res2 != ERROR_SUCCESS) {
				tunFileDescriptor = -1;
				break;
			}
			if (keyComponentId != null && keyComponentId == "tap0801"){
				devGuid = regAdapter.GetValue("NetCfgInstanceId").ToString();
				LONG res3 = RegGetValue(hSubKey, subKeyName, "NetCfgInstanceId",
					RRF_RT_REG_SZ, NULL, devGuid, &valueBufferSize);
				if(res3 != ERROR_SUCCESS){
					tunFileDescriptor = -1;
					break;
				}
			}
		}
		int* ptr = CreateFile("\\\\.\\Global\\"+devGuid+".tap",FileAccess.ReadWrite,
			FileShare.ReadWrite,0,FileMode.Open,FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED | FILE_FLAG_DELETE_ON_CLOSE, NULL);
		int len;
		int pstatus = 1;
		DeviceIoControl (ptr, TAP_CONTROL_CODE (6, METHOD_BUFFERED) /* TAP_IOCTL_SET_MEDIA_STATUS */, &pstatus, 4,
				&pstatus, 4, out len, IntPtr.Zero);
		int[2] ptun; 
		ptun[0] = 0x0100030a;
		ptun[1] = 0x0000030a; 
		putun[2] = unchecked((int)0x00ffffff); 
		DeviceIoControl(&ptr, TAP_CONTROL_CODE (10, METHOD_BUFFERED) /* TAP_IOCTL_CONFIG_TUN */, &ptun, 12,
			&ptun, 12, out len, NULL);
		
		RegCloseKey(hRegAdapters);
		return ptr;
	
	#elif defined __linux__ 
		// Linux
		int tunFileDescriptor;
		struct ifreq ifRequest;
		memset(&ifRequest, 0, sizeof(struct ifreq));
		ifRequest.ifr_flags = IFF_TUN | IFF_NO_PI;
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

		return tunFileDescriptor;
	#elif defined TARGET_OS_X
		// Mac
	#else
	#endif
}

static void closeInterface(void* vcontext)
{	
	#ifdef _WIN32
		// Windows
		CloseHandle(vcontext);
	#elif defined __linux__ 
		// Linux
		struct Tunnel* tun = (struct Tunnel*) vcontext;
		close(tun->fileDescriptor);
		event_del(tun->incomingEvent);
		event_free(tun->incomingEvent);
	#elif defined TARGET_OS_X
		// Mac
	#else
	#endif
}

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext)
{
	eventType = eventType;

    // 292 bytes of extra padding to build headers back from for better efficiency.
    uint8_t messageBuffer[MAX_PACKET_SIZE + PADDING_SPACE];

    struct Message message = {
        .bytes = messageBuffer + PADDING_SPACE,
        .padding = PADDING_SPACE,
        .length = MAX_PACKET_SIZE
    };
	
	#ifdef _WIN32   
		// Windows
		OVERLAPPED overlapped;
		// TODO
		// ssize_t length = FileRead(socket, )
		if (length < 0) {
			printf("Error reading from TUN device %d\n", (int) length);
			return;
		}
		message.length = length;

		struct Interface* iface = &((struct Tunnel*) vcontext)->interface;
		if (iface->receiveMessage) {
			iface->receiveMessage(&message, iface);
		}
	#elif defined __linux__
		// Linux
		ssize_t length = read(socket, messageBuffer + PADDING_SPACE, MAX_PACKET_SIZE);

		if (length < 0) {
			printf("Error reading from TUN device %d\n", (int) length);
			return;
		}
		message.length = length;

		struct Interface* iface = &((struct Tunnel*) vcontext)->interface;
		if (iface->receiveMessage) {
			iface->receiveMessage(&message, iface);
		}
	#elif defined TARGET_OS_X
		// Mac  
	#else
	#endif
    
}

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
	#ifdef _WIN32
		// Windows
		struct Tunnel* tun = (struct Tunnel*) iface->senderContext;
		OVERLAPPED overlapped;
		WriteFile(tun->fileDescriptor, message->bytes, message->length, overlapped);
	#elif defined __linux__
		// Linux
		struct Tunnel* tun = (struct Tunnel*) iface->senderContext;
		ssize_t ret = write(tun->fileDescriptor, message->bytes, message->length);
		if (ret < 0) {
			printf("Error writing to TUN device %d\n", errno);
		}
		// TODO: report errors
		return 0;
	#elif defined TARGET_OS_X
		// Mac  
	#else
	#endif
    
}

struct Interface* TunInterface_new(String* interfaceName,
                                   struct event_base* base,
                                   struct Allocator* allocator)
{
	#ifdef _WIN32   
		// Windows
	#elif defined __linux__
		// Linux
		errno = 0;
		int tunFileDesc = openTunnel(interfaceName ? interfaceName->bytes : NULL);
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

		struct Tunnel* tun = allocator->malloc(sizeof(struct Tunnel), allocator);
		tun->incomingEvent = event_new(base, tunFileDesc, EV_READ | EV_PERSIST, handleEvent, tun);
		tun->fileDescriptor = tunFileDesc;

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

		return &tun->interface;
	#elif defined TARGET_OS_X
		// Mac  
	#else
	#endif
    
}