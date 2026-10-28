#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include "our_driver.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

    if (!device_is_ready(driver)) {
        LOG_ERR("Sensor driver is not ready");
        return 0;
    }

    int ret = our_driver_set_led_state(driver, false);
    if (ret < 0) {
        LOG_ERR("Failed to set LED state: %d", ret);
    }

    return 0;
}
