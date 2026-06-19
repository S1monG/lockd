#include "http_server.h"

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h> // definitions for Internet address family data structures and constants (e.g., struct sockaddr_in, struct in_addr, AF_INET, INADDR_ANY)
#include <arpa/inet.h>  // functions to convert IP addresses between binary and text forms (inet_ntop, inet_pton, inet_ntoa, etc.)
#include <microhttpd.h>
#include <string.h>
#include <stdio.h>

static struct MHD_Daemon *http_daemon = NULL;

// callback to check which clients will be allowed to connect and/or to log client address
enum MHD_Result on_client_connect(void *cls,
                                const struct sockaddr *addr,
                                socklen_t addrlen)
{
    // when is address ever NULL?
    // TODO: replace with real address filtering, ex MHD_NO for everything except localhost/127.0.0.1
    if (addr == NULL) {
        return MHD_YES;
    }

    char ipstr[INET6_ADDRSTRLEN] = {0};

    if (addr->sa_family == AF_INET) {
        const struct sockaddr_in *sin = (const struct sockaddr_in *)addr;
        if (inet_ntop(AF_INET, &sin->sin_addr, ipstr, sizeof(ipstr)) != NULL) {
            printf("Client connected: %s\n", ipstr); // TODO: logger
        }
    } else if (addr->sa_family == AF_INET6) {
        const struct sockaddr_in6 *sin6 = (const struct sockaddr_in6 *)addr;
        if (inet_ntop(AF_INET6, &sin6->sin6_addr, ipstr, sizeof(ipstr)) != NULL) {
            printf("Client connected (v6): %s\n", ipstr); // TODO: logger
        }
    }

    return MHD_YES;
}

// Using inet_ntoa which only handles IPv4 (and maybe not thread-safe)
/* static int on_client_connect(void *cls,
                            const struct sockaddr *addr,
			                socklen_t addrlen)
{
    // TODO: replace with real address filtering, ex MHD_NO for everything except localhost/127.0.0.1
    if (addr == NULL) {
        return MHD_YES;
    }

    const struct sockaddr_in *sin = (const struct sockaddr_in *)addr;
    printf("Client connected: %s\n", inet_ntoa(sin->sin_addr));

    return MHD_YES;
} */

// callback handler for all URIs
enum MHD_Result answer_to_connection(void *cls, struct MHD_Connection *connection,
                                    const char *url,
                                    const char *method, const char *version,
                                    const char *upload_data,
                                    size_t *upload_data_size, void **req_cls)
{
    struct MHD_Response *response;
    int ret;

    // const char *page  = "<html><body>Hello, browser!</body></html>";
    // response = MHD_create_response_from_buffer(strlen(page), (void*) page, MHD_RESPMEM_PERSISTENT);

    response = MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
    
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    // ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
    MHD_destroy_response(response);

    return ret;
}

void http_server_init()
{
    http_daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT,
                                   &on_client_connect, NULL,
                                   &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == http_daemon) {
        // TODO LOG
    }
}

void http_server_stop()
{
    if (http_daemon != NULL) {
        // TODO LOG
        MHD_stop_daemon(http_daemon);
        http_daemon = NULL;
    }
}



