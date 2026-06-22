#include <stdio.h>
#include <unistd.h>

#include "drivers/gpio.h"
#include "common/hello.h"
#include "controller/door_controller.h"
#include "api/http_server.h"

int main(void)
{
    printf("%s\n", hello_message());
    gpio_init();
    door_controller_init();
    http_server_init();

    getchar(); // processing-time friendly pause, waiting for enter key

    http_server_stop();
    door_controller_stop();
    gpio_close();

    return 0;
}
