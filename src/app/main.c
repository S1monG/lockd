#include <stdio.h>
#include "common/hello.h"
#include "drivers/gpio.h"

int main(void)
{
    printf("%s\n", hello_message());
    
    gpio_set_high(GREEN_LED_BCM_PIN);
    gpio_set_low(RED_LED_BCM_PIN);

    return 0;
}
