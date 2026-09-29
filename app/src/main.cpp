#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
    struct sensor_value val;

    if (!device_is_ready(driver)) {
        LOG_ERR("Sensor driver is not ready");
        return 0;
    }

    while (1) {
        int ret = sensor_sample_fetch(driver);
        if (ret < 0) {
            LOG_ERR("Sensor sample fetch failed: %d", ret);
            return 0;
        }
        LOG_INF("sensor_sample_fetch: LED ON");

        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

        ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        if (ret < 0) {
            LOG_ERR("Sensor channel get failed: %d", ret);
            return 0;
        }
        LOG_INF("sensor_channel_get: LED OFF");

        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

    return 0;
}
