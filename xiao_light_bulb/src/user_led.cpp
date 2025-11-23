#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define USER_LED_NODE DT_ALIAS(user_led)
static const struct gpio_dt_spec user_led = GPIO_DT_SPEC_GET(USER_LED_NODE, gpios);

void user_led_init()
{
	if (!gpio_is_ready_dt(&user_led)) {
		printk("Error: LED device %s is not ready\n", user_led.port->name);
		return;
	}

	int ret = gpio_pin_configure_dt(&user_led, GPIO_OUTPUT_LOW);

    if (ret < 0) {
		printk("Error: User LED Configuration failed.\n");
		return;
	}
}

void user_led_turn_on()
{
	int ret = gpio_pin_set_dt(&user_led, 1);

	if (ret < 0) {
		printk("Error: User LED Turn On failed.");
		return;
	}
}

void user_led_turn_off()
{
	int ret = gpio_pin_set_dt(&user_led, 0);

	if (ret < 0) {
		printk("Error: User LED Turn Off failed.");
		return;
	}
}