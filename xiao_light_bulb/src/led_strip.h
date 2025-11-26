#pragma once

#include <stdio.h>

#define RED_R   0x0f
#define RED_G   0x00
#define RED_B   0x00

#define GREEN_R 0x00
#define GREEN_G 0x0f
#define GREEN_B 0x00

#define BLUE_R  0x00
#define BLUE_G  0x00
#define BLUE_B  0x0f

#define OFF_R   0x00
#define OFF_G   0x00
#define OFF_B   0x00

int led_strip_init(void);
int led_strip_write(size_t led_index, uint8_t r, uint8_t g, uint8_t b);
int led_strip_clear(void);