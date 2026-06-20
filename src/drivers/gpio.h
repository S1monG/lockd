// Small libgpiod wrapper to request a GPIO line and drive it high or low.
// Hardware-dependent code for the raspberry pi zero w, hard to test without the board so no unit tests.

#ifndef LOCKD_GPIO_H
#define LOCKD_GPIO_H

#include <gpiod.h>

#define GREEN_LED_BCM_PIN 23
#define RED_LED_BCM_PIN 24
#define NUM_REQUESTED_OFFSETS 2

void gpio_set(const enum gpiod_line_value *values);
void gpio_get(enum gpiod_line_value *values);
void gpio_init(void);
void gpio_close(void);

#endif // LOCKD_GPIO_H