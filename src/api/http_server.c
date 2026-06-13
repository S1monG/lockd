#include "http_server.h"

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <string.h>

static struct MHD_Daemon *http_daemon = NULL;

int answer_to_connection (void *cls, struct MHD_Connection *connection,
                          const char *url,
                          const char *method, const char *version,
                          const char *upload_data,
                          size_t *upload_data_size, void **req_cls)
{
    const char *page  = "<html><body>Hello, browser!</body></html>";
    struct MHD_Response *response;
    int ret;

    response = MHD_create_response_from_buffer(strlen(page), (void*) page, MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

void http_server_init()
{
    http_daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
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



