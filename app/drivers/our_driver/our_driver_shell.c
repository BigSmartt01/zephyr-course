#include <errno.h>
#include <string.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include "our_driver.h"

static const struct device *const sensor = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (!device_is_ready(sensor)) {
        shell_error(sh, "Sensor device %s is not ready", sensor->name);
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(sensor);
    if (ret < 0) {
        shell_error(sh, "Sensor sample fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Sensor sample fetched");
    return 0;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (!device_is_ready(sensor)) {
        shell_error(sh, "Sensor device %s is not ready", sensor->name);
        return -ENODEV;
    }

    struct sensor_value value;
    int ret = sensor_channel_get(sensor, SENSOR_CHAN_AMBIENT_TEMP, &value);
    if (ret < 0) {
        shell_error(sh, "Sensor channel get failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Ambient temperature: %d.%06d", value.val1, value.val2);
    return 0;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    shell_print(sh, "Device: %s", sensor->name);
    shell_print(sh, "Ready: %s", device_is_ready(sensor) ? "yes" : "no");
    return 0;
}

static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
    if (argc < 2) {
        shell_error(sh, "Missing LED state; expected 0 (off) or 1 (on)");
        return -EINVAL;
    }

    bool enabled;
    if (strcmp(argv[1], "0") == 0) {
        enabled = false;
    } else if (strcmp(argv[1], "1") == 0) {
        enabled = true;
    } else {
        shell_error(sh, "Invalid LED state '%s'; expected 0 or 1", argv[1]);
        return -EINVAL;
    }

    if (!device_is_ready(sensor)) {
        shell_error(sh, "Sensor device %s is not ready", sensor->name);
        return -ENODEV;
    }

    int ret = our_driver_set_led_state(sensor, enabled);
    if (ret < 0) {
        shell_error(sh, "Failed to set LED state: %d", ret);
        return ret;
    }

    shell_print(sh, "LED %s", enabled ? "on" : "off");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmd,
    SHELL_CMD(fetch, NULL, "Fetch a sensor sample", cmd_sensor_fetch),
    SHELL_CMD(read, NULL, "Read the ambient temperature channel", cmd_sensor_read),
    SHELL_CMD(info, NULL, "Show sensor device information", cmd_sensor_info),
    SHELL_CMD_ARG(set, NULL, "Set LED state (0=off, 1=on)", cmd_sensor_set, 1, 1),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_subcmd, "Sensor commands", NULL);