#ifndef LOCKD_GPIO_H
#define LOCKD_GPIO_H

#define GREEN_LED_BCM_PIN 23
#define RED_LED_BCM_PIN 24
#define BCM_PINS { GREEN_LED_BCM_PIN, RED_LED_BCM_PIN }

void gpio_set_high(int pin);
void gpio_set_low(int pin);

#endif // LOCKD_GPIO_H