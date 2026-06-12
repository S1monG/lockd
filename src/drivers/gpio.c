// Small libgpiod wrapper to request a GPIO line and drive it high or low.

#include "gpio.h"

#include <gpiod.h>
#include <errno.h>
#include <stdio.h>

#define GPIO_CHIP_NAME "gpiochip0" // Default first GPIO chip on most Pi boards. TODO configure
#define GPIO_MAX_PIN 64

static struct gpiod_chip *gpio_chip = NULL;                    // cache the opened GPIO device
static struct gpiod_line *gpio_lines[GPIO_MAX_PIN] = { NULL }; // cache requested lines

static struct gpiod_line *get_gpio_line(int pin)
{
    if (pin < 0 || pin >= GPIO_MAX_PIN) { // Validate pin number
        fprintf(stderr, "gpio: invalid BCM pin %d\n", pin);
        return NULL;
    }

    if (gpio_lines[pin]) { // Return cached line if already requested
        return gpio_lines[pin];
    }

    if (!gpio_chip) { // Open GPIO chip if not already done
        gpio_chip = gpiod_chip_open_by_name(GPIO_CHIP_NAME);
        if (!gpio_chip) {
            perror("gpiod_chip_open_by_name");
            return NULL;
        }
    }

    struct gpiod_line *line = gpiod_chip_get_line(gpio_chip, pin); // Get line for the specified pin
    if (!line) {
        fprintf(stderr, "gpio: failed to get line for BCM pin %d\n", pin);
        return NULL;
    }

    if (gpiod_line_request_output(line, "lockd", 0) < 0) { // Request line as output, initially low
        perror("gpiod_line_request_output");
        return NULL;
    }

    gpio_lines[pin] = line; // cache and return the line
    return line;
}

void gpio_set_high(int pin)
{
    struct gpiod_line *line = get_gpio_line(pin);
    if (!line) {
        return;
    }

    if (gpiod_line_set_value(line, 1) < 0) {
        perror("gpiod_line_set_value");
    }
}

void gpio_set_low(int pin)
{
    struct gpiod_line *line = get_gpio_line(pin);
    if (!line) {
        return;
    }

    if (gpiod_line_set_value(line, 0) < 0) {
        perror("gpiod_line_set_value");
    }
}

