#include "http_server.h"
#include "controller/door_controller.h"

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h> // definitions for Internet address family data structures and constants (e.g., struct sockaddr_in, struct in_addr, AF_INET, INADDR_ANY)
#include <arpa/inet.h>  // functions to convert IP addresses between binary and text forms (inet_ntop, inet_pton, inet_ntoa, etc.)
#include <microhttpd.h>
#include <string.h>
#include <stdio.h>

#define JSON_CONTENT_TYPE "application/json"
#define EXPECTED_TOKEN "securly-secured-secret-token"

static struct MHD_Daemon *http_daemon = NULL;

int is_authorized(struct MHD_Connection *connection)
{
    const char *auth = MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Authorization");
    return auth != NULL && strcmp(auth, EXPECTED_TOKEN) == 0;
}

// callback to check which clients will be allowed to connect and/or to log client address
enum MHD_Result on_client_connect(void *cls,
                                const struct sockaddr *addr,
                                socklen_t addrlen)
{
    (void)cls;
    (void)addrlen;

    // addr should generally not be NULL
    if (addr == NULL) {
        return MHD_NO;
    }

    char ipstr[INET6_ADDRSTRLEN] = {0};

    if (addr->sa_family == AF_INET) {
        const struct sockaddr_in *sin = (const struct sockaddr_in *)addr;
        if (inet_ntop(AF_INET, &sin->sin_addr, ipstr, sizeof(ipstr)) != NULL) {
            printf("Client connected: %s\n", ipstr); // TODO: logger
            return MHD_YES;
        }
        return MHD_NO; // Failed to convert address to string, why?
    }
    
    if (addr->sa_family == AF_INET6) {
        // TODO: Allow IPv6?
        // TODO: log
        return MHD_NO;
    }

    return MHD_NO; // Fallback
}

// callback handler for all URIs
enum MHD_Result answer_to_connection(void *cls, struct MHD_Connection *connection,
                                    const char *url,
                                    const char *method, const char *version,
                                    const char *upload_data,
                                    size_t *upload_data_size, void **req_cls)
{
    (void)cls;
    (void)version;
    (void)upload_data;
    (void)upload_data_size;
    (void)req_cls;

    struct MHD_Response *response;
    int ret;

    if (!is_authorized(connection)) {
        const char *json_response  = "{ \"status\": \"UNAUTHORIZED\" }";
        response = MHD_create_response_from_buffer(strlen(json_response), (void*) json_response, MHD_RESPMEM_PERSISTENT);
        MHD_add_response_header(response, "Content-Type", JSON_CONTENT_TYPE);
        ret = MHD_queue_response(connection, MHD_HTTP_UNAUTHORIZED, response);
        MHD_destroy_response(response);
        return ret;
    }

    if (strcmp(method, "GET") == 0) {
        const char *json_response = door_controller_state() == LOCKED ? "{ \"status\": \"LOCKED\" }" : "{ \"status\": \"UNLOCKED\" }";
        response = MHD_create_response_from_buffer(strlen(json_response), (void*) json_response, MHD_RESPMEM_PERSISTENT);
    } else if (strcmp(method, "POST") == 0) {
        if (strcmp(url, "/unlock") == 0) {
            // TODO: check validity of request. For example passkey or something like that.
            door_controller_event(VALID_ACCESS);
            const char *json_response  = "{ \"status\": \"UNLOCKED\" }";
            response = MHD_create_response_from_buffer(strlen(json_response), (void*) json_response, MHD_RESPMEM_PERSISTENT);
        } else if (strcmp(url, "/lock") == 0) {
            door_controller_event(LOCK_REQUEST);
            const char *json_response  = "{ \"status\": \"LOCKED\" }";
            response = MHD_create_response_from_buffer(strlen(json_response), (void*) json_response, MHD_RESPMEM_PERSISTENT);
        } else {
            const char *json_response  = "{ \"status\": \"INVALID_REQUEST\" }";
            response = MHD_create_response_from_buffer(strlen(json_response), (void*) json_response, MHD_RESPMEM_PERSISTENT);
        }
    } else {
        return MHD_NO; // Method not allowed, TODO: return something more informative, ex 405 Method Not Allowed
    }
    
    MHD_add_response_header(response, "Content-Type", JSON_CONTENT_TYPE);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
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



