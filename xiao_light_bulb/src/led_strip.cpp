#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(led_strip, LOG_LEVEL_INF);

#define STRIP_NODE		DT_ALIAS(led_strip)

#if DT_NODE_HAS_PROP(DT_ALIAS(led_strip), chain_length)
#define STRIP_NUM_PIXELS	DT_PROP(DT_ALIAS(led_strip), chain_length)
#else
#error Unable to determine length of LED strip
#endif

#define DELAY_TIME K_MSEC(CONFIG_SAMPLE_LED_UPDATE_DELAY)

static struct led_rgb pixels[STRIP_NUM_PIXELS];

static const struct device *const strip = DEVICE_DT_GET(STRIP_NODE);

int led_strip_init(void)
{
    if (device_is_ready(strip)) {
        LOG_INF("Found LED strip device %s", strip->name);
    } else {
        LOG_ERR("LED strip device %s is not ready", strip->name);
        return -1;
    }

    // nothing to do here

    return 0;
}

int led_strip_write(size_t led_index, uint8_t r, uint8_t g, uint8_t b)
{
    if (led_index >= STRIP_NUM_PIXELS) {
        LOG_ERR("LED index out of range");
        return -1;
    }
    pixels[led_index].r = r;
    pixels[led_index].g = g;
    pixels[led_index].b = b;

    int rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
    if (rc) {
        LOG_ERR("Failed to update LED strip: %d", rc);
        return rc;
    }

    return 0;
}

int led_strip_clear(void)
{
	for (size_t i = 0; i < STRIP_NUM_PIXELS; i++) {
		pixels[i].r = 0;
		pixels[i].g = 0;
		pixels[i].b = 0;
	}

	int rc = led_strip_update_rgb(strip, pixels, STRIP_NUM_PIXELS);
	if (rc) {
		LOG_ERR("Failed to update LED strip: %d", rc);
        return rc;
	}

    return 0;
}