#ifndef ZEPHYR_DRIVERS_BUTTON_H_
#define ZEPHYR_DRIVERS_BUTTON_H_

#include <zephyr/drivers/gpio.h>

// Our own API, since there is no built-in button subsystem
struct button_api {
    int (*get)(const struct device *dev, uint8_t *state);
};

// Per-instance configuration, filled from the devicetree
struct button_config {
    struct gpio_dt_spec btn;
    uint32_t id;
};

#endif /* ZEPHYR_DRIVERS_BUTTON_H_ */