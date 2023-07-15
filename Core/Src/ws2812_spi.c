#include "ws2812_spi.h"

uint8_t ws2812_buffer[WS2812_BUF_LEN + WS2812_BUF_RESET_LEN] = {0};

uint32_t ws2812_spi_get_led(uint16_t index)
{
    return ws2812_buffer[(index * 24) + index] << 16 | ws2812_buffer[(index * 24) + index + 1] << 8 | ws2812_buffer[(index * 24) + index + 2];
}
void ws2812_spi_set_led(uint16_t index, uint32_t color)
{
    if (index >= WS2812_STRIP_LEN)
        return;

    for (uint8_t i = 0; i < 24; i++)
    {
        ws2812_buffer[(index * 24) + i] = ((color >> (24 - i)) & 1) ? 0b11111100 : 0b111000000;
    }
}
void ws2812_spi_set_all(uint32_t color)
{
    for (uint16_t i = 0; i < WS2812_STRIP_LEN; i++)
    {
        ws2812_spi_set_led(i, color);
    }
}

void ws2812_spi_send(SPI_HandleTypeDef *hspi)
{
    HAL_SPI_Transmit_DMA(hspi, ws2812_buffer, WS2812_BUF_LEN + WS2812_BUF_RESET_LEN);
}

void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi)
{
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
}