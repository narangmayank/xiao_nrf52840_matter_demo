#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "led_strip.h"

LOG_MODULE_REGISTER(user_led, LOG_LEVEL_INF);

void user_led_init()
{
	int ret = led_strip_init();
	if (ret < 0) {
		LOG_ERR("LED strip initialization failed.");
		return;
	}
}

void user_led_turn_on()
{
	// Set first LED to green
	int ret = led_strip_write(0, GREEN_R, GREEN_G, GREEN_B);

	if (ret < 0) {
		LOG_ERR("User LED Turn On failed.");
		return;
	}
}

void user_led_turn_off()
{
	int ret = led_strip_write(0, OFF_R, OFF_G, OFF_B);

	if (ret < 0) {
		LOG_ERR("User LED Turn Off failed.");
		return;
	}
}