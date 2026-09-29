#include <errno.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static const struct gpio_dt_spec app_led = GPIO_DT_SPEC_GET(DT_ALIAS(app_led), gpios);

static int sample_fetch_custom(const struct device *dev, enum sensor_channel chan)
{
    ARG_UNUSED(dev);

    if (chan != SENSOR_CHAN_ALL && chan != SENSOR_CHAN_AMBIENT_TEMP) {
        return -ENOTSUP;
    }

    return gpio_pin_set_dt(&app_led, 1);
}

static int channel_get_my_custom(const struct device *dev,
                        enum sensor_channel chan,
                        struct sensor_value *val)
{
    ARG_UNUSED(dev);

    if (chan != SENSOR_CHAN_AMBIENT_TEMP) {
        return -ENOTSUP;
    }

    int ret = gpio_pin_set_dt(&app_led, 0);
    if (ret < 0) {
        return ret;
    }

    val->val1 = 0;
    val->val2 = 0;
    return 0;
}

static const struct sensor_driver_api custom_name_api = {
    .sample_fetch = sample_fetch_custom,
    .channel_get = channel_get_my_custom,
};

static int init(const struct device *dev)
{
    ARG_UNUSED(dev);

    LOG_INF("Initializing our driver");

    if (!gpio_is_ready_dt(&app_led)) {
        return -ENODEV;
    }

    return gpio_pin_configure_dt(&app_led, GPIO_OUTPUT_INACTIVE);
}

#define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, NULL, POST_KERNEL, 80, &custom_name_api);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST);