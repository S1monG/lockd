#include <stdio.h>
#include "common/hello.h"
#include "controller/door_controller.h"
#include "api/http_server.h"

int main(void)
{
    printf("%s\n", hello_message());
    
    // door_controller_event(VALID_ACCESS);
    // door_controller_event(TIMEOUT);

    http_server_init();
    getchar(); // processing-time friendly pause, waiting for enter key
    http_server_stop();
    
    return 0;
}
