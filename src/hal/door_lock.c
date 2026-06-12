#include "door_lock.h"
#include "drivers/gpio.h"

void door_lock_lock()
{
    gpio_set_high(RED_LED_BCM_PIN);
    gpio_set_low(GREEN_LED_BCM_PIN);
}

void door_lock_unlock()
{
    gpio_set_high(GREEN_LED_BCM_PIN);
    gpio_set_low(RED_LED_BCM_PIN);
}
