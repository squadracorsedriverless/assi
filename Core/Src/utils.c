#include "utils.h"
#include "main.h"
uint32_t tick_100us = 0;

/**
 * @brief Returns true if a given time has passed
 *
 * @param delay_100us_last Starting time
 * @param delay_100us Desired period
 * @return true Period has elapsed
 * @return false Period has not elapsed
 */
uint8_t delay_fun(uint32_t *delay_100us_last, uint32_t delay_100us)
{
    uint32_t current_time = tick_get_100us();

    if (current_time > *delay_100us_last && (current_time - *delay_100us_last) >= delay_100us)
    {
        *delay_100us_last = current_time;
        return 1;
    }
    else if (current_time < *delay_100us_last && (UINT32_MAX - current_time - *delay_100us_last) >= delay_100us)
    {
        *delay_100us_last = current_time;
        return 1;
    }
    /*In case of timer overflow, the delay is computed correctly*/

    return 0;
}

/*Return the value of the counter that is incremented every 100us*/
uint32_t tick_get_100us(void)
{
    return tick_100us;
}

board_t utils_get_board_id()
{
    static board_t board = BOARD_NOT_SAMPLED;

    if (board == BOARD_NOT_SAMPLED)
    {
        board = HAL_GPIO_ReadPin(BOARD_ID_1_GPIO_Port, BOARD_ID_1_Pin) | (HAL_GPIO_ReadPin(BOARD_ID_2_GPIO_Port, BOARD_ID_2_Pin) << 1);
    }

    return board;
}
