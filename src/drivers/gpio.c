#include "gpio.h"

#include <gpiod.h>
#include <errno.h>
#include <stdio.h>

#define GPIO_CHIP_PATH "/dev/gpiochip0" // Default GPIO chip path on most Pi boards. TODO configure
#define GPIO_MAX_LINES 54               // Pi zero W has 54 lines/pins

// A pin is the physical contact (BCM 23, BCM 24). A line is the libgpiod software object you get after requesting that pin. 
// You can request a line as input or output, and then set its value if it's an output. 
// I cache the requested lines so I don't have to request them again every time I want to toggle them.
static struct gpiod_line_request *line_request = NULL;

static struct gpiod_line_request *request_output_lines(const unsigned int *offsets, 
                                                    enum gpiod_line_value value, 
                                                    const char *consumer)
{
    struct gpiod_request_config     *req_cfg = NULL;
    struct gpiod_line_request       *request = NULL;
    struct gpiod_line_settings      *settings;
    struct gpiod_line_config        *line_cfg;
    struct gpiod_chip               *chip;

    // open gpio chip/device
    chip = gpiod_chip_open(GPIO_CHIP_PATH);
    if (!chip) {
        perror("gpiod_chip_open"); // TODO perror or fprintf?
        return NULL;
    }

    settings = gpiod_line_settings_new();
    if (!settings) {
        perror("gpiod_line_settings_new");
        goto close_chip;
    }

    // configure value and output direction
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, value); // TODO: check what the default is

    line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        perror("gpiod_line_config_new");
        goto free_settings;
    }

    if (gpiod_line_config_add_line_settings(line_cfg, offsets, NUM_REQUESTED_OFFSETS, settings)) {
        perror("gpiod_line_config_add_line_settings");
        goto free_line_config;
    }

    req_cfg = gpiod_request_config_new();
    if (!req_cfg) {
        perror("gpiod_request_config_new");
        goto free_line_config;
    }
    gpiod_request_config_set_consumer(req_cfg, consumer); // TODO: is this needed?
    
    // request a set of lines for exclusive usage.
    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
    gpiod_request_config_free(req_cfg);

free_line_config:
    gpiod_line_config_free(line_cfg);

free_settings:
    gpiod_line_settings_free(settings);

close_chip:
    gpiod_chip_close(chip);

    return request;
}

void gpio_init(void)
{
    if (line_request) {
        return;
    }

    unsigned int offsets[NUM_REQUESTED_OFFSETS] = {GREEN_LED_BCM_PIN, RED_LED_BCM_PIN};
    enum gpiod_line_value value = GPIOD_LINE_VALUE_INACTIVE; // start lines low by default

    line_request = request_output_lines(offsets, value, "toggle-gpiod");
    if (!line_request) {
        fprintf(stderr, "gpio: failed to initialize line request\n");
    }
}

void gpio_close(void)
{
    if (line_request) {
        gpiod_line_request_release(line_request);
        line_request = NULL;
    }
}

// Set the values of all lines associated with a request.
// values must be sized to contain the number of lines in the request (2)
// The values must be freed by the caller.
void gpio_set(const enum gpiod_line_value *values)
{
    // TODO: add checks for number of values = number of lines with gpiod_line_request_get_num_requested_lines
    // and check for line_request != NULL
    gpiod_line_request_set_values(line_request, values);
}

// Get the values of all requested lines.
// values must be sized to contain the number of lines in the request (2)
// The values must be freed by the caller.
void gpio_get(enum gpiod_line_value *values)
{
    // TODO: add checks for number of values = number of lines with gpiod_line_request_get_num_requested_lines
    // and check for line_request != NULL
    gpiod_line_request_get_values(line_request, values);
}

