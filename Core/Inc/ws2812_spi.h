
#ifndef WS2812_SPI_H_
#define WS2812_SPI_H_

#include "stm32l4xx.h"
#include <stdint.h>

#define WS2812_STRIP_LEN 10
#define WS2812_BUF_LEN (WS2812_STRIP_LEN * 24)
#define WS2812_BUF_RESET_LEN 0 // making this higher introduces glitches

#define WS2812_COLOR(r, g, b) (((uint32_t)(g)) << 16 | ((uint32_t)(r)) << 8 | (b))
#define WS2812_RED(color) (((uint32_t)color << 8) & 0xFF)
#define WS2812_GREEN(color) (((uint32_t)color << 16) & 0xFF)
#define WS2812_BLUE(color) (((uint32_t)color) & 0xFF)

extern uint8_t ws2812_buffer[WS2812_BUF_LEN + WS2812_BUF_RESET_LEN];
extern uint8_t ws2812_send;

uint32_t ws2812_spi_get_led(uint16_t index);
void ws2812_spi_set_led(uint16_t index, uint32_t color);
void ws2812_spi_set_all(uint32_t color);
void ws2812_spi_send(SPI_HandleTypeDef *hspi);

#endif