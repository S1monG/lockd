#include "door_lock.h"
#include "drivers/gpio.h"

void door_lock_lock()
{
    const enum gpiod_line_value values[NUM_REQUESTED_OFFSETS] = {GPIOD_LINE_VALUE_INACTIVE, GPIOD_LINE_VALUE_ACTIVE};
    gpio_set(values);
}

void door_lock_unlock()
{
    const enum gpiod_line_value values[NUM_REQUESTED_OFFSETS] = {GPIOD_LINE_VALUE_ACTIVE, GPIOD_LINE_VALUE_INACTIVE};
    gpio_set(values);
}
