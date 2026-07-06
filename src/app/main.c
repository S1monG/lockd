#include <stdio.h>
#include <unistd.h>

#include "drivers/gpio.h"
#include "utils/logger.h"
#include "controller/door_controller.h"
#include "api/http_server.h"

int main(void)
{
    LOG_INFO("hello number: %d", 42);
    LOG_ERROR("");
    LOG_WARN("this is a warning");

    return 0;

    gpio_init();
    door_controller_init();
    http_server_init();

    getchar(); // processing-time friendly pause, waiting for enter key

    http_server_stop();
    door_controller_stop();
    gpio_close();

    return 0;
}
