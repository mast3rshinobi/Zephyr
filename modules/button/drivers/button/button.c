// Ties to the 'compatible = "custom,button"' node in the devicetree
#define DT_DRV_COMPAT custom_button

#include <errno.h>
#include <zephyr/logging/log.h>

#include "button.h"

LOG_MODULE_REGISTER(button);

static int button_init(const struct device *dev);
static int button_state_get(const struct device *dev, uint8_t *state);

// Runs at boot
static int button_init(const struct device *dev)
{
    int ret;
    const struct button_config *cfg = (const struct button_config *)dev->config;
    const struct gpio_dt_spec *btn = &cfg->btn;

    LOG_DBG("Initializing button (instance ID: %u)\r\n", cfg->id);

    if (!gpio_is_ready_dt(btn)) {
        LOG_ERR("GPIO is not ready\r\n");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(btn, GPIO_INPUT);
    if (ret < 0) {
        LOG_ERR("Could not configure GPIO as input\r\n");
        return -ENODEV;
    }

    return 0;
}

// The one API function this driver provides
static int button_state_get(const struct device *dev, uint8_t *state)
{
    int ret;
    const struct button_config *cfg = (const struct button_config *)dev->config;
    const struct gpio_dt_spec *btn = &cfg->btn;

    ret = gpio_pin_get_dt(btn);
    if (ret < 0) {
        LOG_ERR("Error (%d): failed to read button pin\r\n", ret);
        return ret;
    } else {
        *state = ret;
    }

    return 0;
}

// Our function listed under the API's member name
static const struct button_api button_api_funcs = {
    .get = button_state_get,
};

// Expanded once per matching devicetree node
#define BUTTON_DEFINE(inst)                                                 \
                                                                            \
    static const struct button_config button_config_##inst = {              \
        .btn = GPIO_DT_SPEC_GET(                                            \
            DT_PHANDLE(DT_INST(inst, custom_button), pin), gpios),          \
        .id = inst                                                          \
    };                                                                      \
                                                                            \
    DEVICE_DT_INST_DEFINE(inst,                                             \
                          button_init,                                      \
                          NULL,                                             \
                          NULL,                                             \
                          &button_config_##inst,                            \
                          POST_KERNEL,                                      \
                          CONFIG_GPIO_INIT_PRIORITY,                        \
                          &button_api_funcs);

DT_INST_FOREACH_STATUS_OKAY(BUTTON_DEFINE)