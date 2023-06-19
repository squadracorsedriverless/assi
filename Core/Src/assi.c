
#include "assi.h"
#include "spi.h"
#include "ws2812_spi.h"

#define ASSI_YELLOW WS2812_COLOR(0xFF, 0x80, 0)
#define ASSI_BLUE WS2812_COLOR(0, 0, 0xFF)

static as_state_t as_state = AS_TEST;

static const uint32_t assi_color[AS_STATE_NUM] = {
    [AS_OFF] = WS2812_COLOR(0, 0, 0),
    [AS_READY] = ASSI_YELLOW,
    [AS_DRIVING] = ASSI_YELLOW,
    [AS_EMERGENCY] = ASSI_BLUE,
    [AS_FINISHED] = ASSI_BLUE};

bool state_dirty = true;

void assi_init()
{
    ws2812_spi_set_all(WS2812_COLOR(0, 0, 0));
    ws2812_spi_send(&hspi1);
}

as_state_t assi_get_state()
{
    return as_state;
}
void assi_set_state(as_state_t state)
{
    if (state != as_state)
    {
        as_state = state;
        state_dirty = true;
        ws2812_spi_set_all(assi_color[as_state]);
    }
}

void assi_sync(bool light_state)
{
    if (as_state == AS_DRIVING || as_state == AS_EMERGENCY)
    {
        state_dirty = 1;
        ws2812_spi_set_all(light_state ? assi_color[as_state] : WS2812_COLOR(0, 0, 0));
    }
    if (state_dirty)
    {
        state_dirty = 0;
        ws2812_spi_send(&hspi1);
    }
}