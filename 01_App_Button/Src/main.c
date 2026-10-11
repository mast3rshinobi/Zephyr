#include <stdio.h>
#include <zephyr/kernel.h>

#include "button.h"

static const int32_t sleep_time_ms = 500;
static const struct device *btn_1 = DEVICE_DT_GET(DT_ALIAS(my_button_1));

int main(void)
{
    int ret;
    uint8_t state_1;

    if (!device_is_ready(btn_1)) {
        printk("Error: button is not ready\r\n");
        return 0;
    }

    // Reach the driver through the device's api pointer
    const struct button_api *btn_api = (const struct button_api *)btn_1->api;

    while (1) {

        ret = btn_api->get(btn_1, &state_1);
        if (ret < 0) {
            printk("Error (%d): failed to read button pin\r\n", ret);
            continue;
        }

        printk("The State of the button is:%u\r\n", state_1);

        k_msleep(sleep_time_ms);
    }

    return 0;
}