// Small libgpiod wrapper to request a GPIO line and drive it high or low.
// Hardware-dependent code for the raspberry pi zero w, hard to test without the board so no unit tests.
// No initialization needed, the first time I set a pin high or low it will request the line and cache it for future use.

#ifndef LOCKD_GPIO_H
#define LOCKD_GPIO_H

#define GREEN_LED_BCM_PIN 23
#define RED_LED_BCM_PIN 24

void gpio_set_high(int pin);
void gpio_set_low(int pin);

#endif // LOCKD_GPIO_H