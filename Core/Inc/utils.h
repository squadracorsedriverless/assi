#ifndef UTILS_H_
#define UTILS_H_
#include <inttypes.h>

#define BOARD_MASTER BOARD_LEFT

#define BOARD_LEFT BOARD_00
#define BOARD_RIGHT BOARD_01
#define BOARD_REAR BOARD_10
#define BOARD_AMI BOARD_11

typedef enum
{
    BOARD_NOT_SAMPLED = -1,
    BOARD_00 = 0b00,
    BOARD_01 = 0b01,
    BOARD_10 = 0b10,
    BOARD_11 = 0b11
} board_t;

uint8_t delay_fun(uint32_t *delay_100us_last, uint32_t delay_100us);
uint32_t tick_get_100us(void);

board_t utils_get_board_id();

#endif
