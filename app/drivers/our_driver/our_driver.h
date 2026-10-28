#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <stdbool.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_set_led_state(const struct device *dev, bool enabled);

#ifdef __cplusplus
}
#endif

#endif