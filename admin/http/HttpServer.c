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
#include <sys/types.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <assert.h>
#include <errno.h>
#include <err.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <string.h>
#include <stdbool.h>

#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "crypto/Crypto.h"
#include "dht/CJDHTConstants.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/ArrayWriter.h"
#include "io/Writer.h"
#include "io/EvBufferWriter.h"
#include "io/FileWriter.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "memory/MallocAllocator.h"

#define LIBSRVR_INDEX "text/html/index.html"
#define SECOND_SLASH_INDEX 9
#define MAX_PATH_SZ 1024
#define MAX_FILE_SZ (1<<20)
#define MAX_CONCURRENT_REQUESTS 32
#define MAX_API_REPLY_SIZE (1<<16)
#define LISTENPORT 51902
#define LISTENADDR "::1"

struct Request
{
    struct evhttp_request* request;
    uint64_t time;
};

struct Context
{
    char* workingDir;

    struct event_base* eventBase;

    struct Request requests[MAX_CONCURRENT_REQUESTS];

    uint32_t txidBaseline;

    struct Writer* stdoutWriter;

    /** Allocator which will be freed when there are no more in-flight requests. */
    struct Allocator* allocator;

    uint8_t messageBuffer[MAX_API_REPLY_SIZE];
    uint8_t* messagePtr;

    evutil_socket_t apiSocket;
    struct event* apiEvent;
};

static void fileHandler(struct evhttp_request* req, void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;
    char html[MAX_FILE_SZ];
    char path[MAX_PATH_SZ];

    const char* uri = evhttp_request_get_uri(req);

    if (uri[0] == '\0') {
        evhttp_send_error(req, HTTP_NOTFOUND, "no URI");
        return;
    } else if (uri[0] == '/') {
        uri++;
    }
    const char* slash = strchr(uri, '/');
    if (slash && slash[0] != '\0') {
        slash = strchr(slash + 1, '/');
    }
    if (!slash) {
        uri = LIBSRVR_INDEX;
        slash = uri + SECOND_SLASH_INDEX;
    }

    if (MAX_PATH_SZ <= strlen(context->workingDir) + strlen(uri) + 1) {
        evhttp_send_error(req, 501, "URI too big.");
        return;
    }

    strcpy(path, context->workingDir);
    strcat(path, uri);

    // This seems to be handled but leaving for paranoia's sake.
    if (strstr(path, "..")) {
        evhttp_send_error(req, 501, "consecutive dots.");
        return;
    }

    struct stat st;
    if (stat(path, &st) == -1) {
        evhttp_send_error(req, HTTP_NOTFOUND, "not found");
        return;
    } else {
        if (S_ISDIR(st.st_mode)) {
            strcpy(path, context->workingDir);
            strcat(path, LIBSRVR_INDEX);
            if (stat(path, &st) == -1) {
                evhttp_send_error(req, HTTP_NOTFOUND, "not found");
                return;
            }
        }
    }

    int fileSize = st.st_size;
    if (fileSize >= MAX_FILE_SZ) {
        evhttp_send_error(req, 501, "file too big");
        return;
    }

    if (fileSize != 0) {
        FILE *fp = fopen(path, "r");
        int read = fread(html, 1, fileSize, fp);
        fclose(fp);
        if (read != fileSize) {
            evhttp_send_error(req, 501, "failed reading file");
            return;
        }
    }

    struct evkeyvalq* headers = evhttp_request_get_output_headers(req);

    int offset = slash - uri;
    memcpy(path, uri, offset);
    char* charset = "; charset=UTF-8";
    memcpy(path + offset, charset, strlen(charset) + 1);
    evhttp_add_header(headers, "Content-Type", path);

    struct evbuffer* buff;
    assert(buff = evbuffer_new());
    evbuffer_add(buff, html, fileSize);
    evhttp_send_reply(req, HTTP_OK, "OK", buff);
    evbuffer_free(buff);
}

static void apiHandler(struct evhttp_request* req, void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;
    const char* uri = evhttp_decode_uri(evhttp_request_get_uri(req));

    #define KEY "/?q="
    const char* q = strstr(uri, KEY);
    if (!q) {
        evhttp_send_error(req, 501, "URI must contain " KEY);
        return;
    }

    const char* content = q + strlen(KEY);

    if (strlen(content) > MAX_API_REPLY_SIZE - 8) {
        evhttp_send_error(req, 501, "Message too long.");
        return;
    }

    memcpy((char*)context->messageBuffer + 8, content, strlen(content));

    struct timeval now;
    event_base_gettimeofday_cached(context->eventBase, &now);
    bool freeAlloc = true;
    for (int i = 0; i < MAX_CONCURRENT_REQUESTS; i++) {
        if (context->requests[i].request) {
            if (context->requests[i].time + 10 < (uint64_t) now.tv_sec) {
                evhttp_request_free(context->requests[i].request);
                context->requests[i].request = NULL;
            } else {
                freeAlloc = false;
            }
        }
    }
    if (freeAlloc) {
        context->allocator->free(context->allocator);
    }

    for (int i = 0; i < MAX_CONCURRENT_REQUESTS; i++) {
        struct Request* oldreq = &context->requests[i];
        if (!oldreq->request) {
            context->requests[i].request = req;
            context->requests[i].time = now.tv_sec;
            uint32_t txNum = context->txidBaseline + i;
            memcpy(context->messageBuffer, "0123", 4);
            memcpy(context->messageBuffer + 4, &txNum, 4);
            write(context->apiSocket, context->messageBuffer, 8 + strlen(content));
            fwrite(context->messageBuffer, 8 + strlen(content), 1, stdout);
            printf("\n");
            return;
        }
    }

    evhttp_send_error(req, 501, "not enough concurrent request slots.");
}

static void handleApiEvent(evutil_socket_t socket, short eventType, void* vcontext)
{
    eventType = eventType;
    struct Context* context = (struct Context*) vcontext;
    errno = 0;
    ssize_t length = read(socket, context->messageBuffer, MAX_API_REPLY_SIZE);

    if (length < 8 || length == MAX_API_REPLY_SIZE) {
        if (length < 8) {
            if (errno == EAGAIN) {
                return;
            }
            perror("broken pipe");
            event_free(context->apiEvent);
            return;
        }
        perror("overlong message");
        return;
    }

    if (memcmp(context->messageBuffer, "4567", 4)) {
        // unrecognized message.
        return;
    }

    uint32_t txNum;
    memcpy(&txNum, context->messageBuffer + 4, 4);

    txNum -= context->txidBaseline;
    if (txNum >= MAX_CONCURRENT_REQUESTS || !context->requests[txNum].request) {
        fprintf(stderr, "txid out of bounds.\n");
        return;
    }
    struct evhttp_request* req = context->requests[txNum].request;
    context->requests[txNum].request = NULL;

    struct evbuffer* buff;
    assert(buff = evbuffer_new());
    struct Writer* w = EvBufferWriter_new(buff, context->allocator);
    w->write(context->messageBuffer + 8, length - 8, w);

    evhttp_send_reply(req, HTTP_OK, "OK", buff);
    evbuffer_free(buff);
}

static void setupApi(char* ipAndPort, struct Context* context)
{
    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    if (evutil_parse_sockaddr_port(ipAndPort, (struct sockaddr*) &addr, &addrLen)) {
        perror("failed to parse api server ip:port");
        exit(1);
    }

    evutil_socket_t sockfd = socket(addr.ss_family, SOCK_STREAM, 0);

    if (connect(sockfd, (struct sockaddr*) &addr, addrLen) < 0) {
        perror("error connecting");
        exit(1);
    }

    context->apiEvent =
        event_new(context->eventBase, sockfd, EV_READ | EV_PERSIST, handleApiEvent, context);
    event_add(context->apiEvent, NULL);

    context->apiSocket = sockfd;

    return;
}

int main(int argc, char **argv)
{
    uint16_t listenPort = LISTENPORT;
    char* listenAddr = LISTENADDR;
    assert(argc > 0);
    if (argc < 3) {
        printf("Usage: %s address.of.api.server:port /full/path/to/directory/ [listen.address.without.brackets [listen.port]]\n", argv[0]);
        return 0;
    }
    if (argc > 3) {
        listenAddr = argv[3];
        if (argc > 4) { listenPort = atoi(argv[4]); }
    }
    Crypto_init();

    struct Context context;
    memset(&context, 0, sizeof(struct Context));
    context.workingDir = argv[2];
    randombytes((uint8_t*) &context.txidBaseline, 4);

    char alloc[1<<20];
    context.allocator = BufferAllocator_new(alloc, 1<<20);

    context.eventBase = event_base_new();

    setupApi(argv[1], &context);

    struct evhttp* httpd = evhttp_new(context.eventBase);
    evhttp_bind_socket(httpd, listenAddr, listenPort);
    evhttp_set_cb(httpd, "/api/", apiHandler, &context);
    evhttp_set_gencb(httpd, fileHandler, &context);

    event_base_dispatch(context.eventBase);

    // Not reached in this code as it is now.
    //evhttp_free(httpd);

    return 0;
}
