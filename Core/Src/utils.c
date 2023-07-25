#include "utils.h"
#include "main.h"

/**
 * @brief Returns true if a given time has passed
 *
 * @param delay_ms_last Starting time
 * @param delay_ms Desired period
 * @return true Period has elapsed
 * @return false Period has not elapsed
 */
uint8_t delay_fun(uint32_t *delay_ms_last, uint32_t delay_ms)
{
    uint32_t current_time = HAL_GetTick();

    if (current_time > *delay_ms_last && (current_time - *delay_ms_last) >= delay_ms)
    {
        *delay_ms_last = current_time;
        return 1;
    }
    else if (current_time < *delay_ms_last && (UINT32_MAX - current_time - *delay_ms_last) >= delay_ms)
    {
        *delay_ms_last = current_time;
        return 1;
    }
    /*In case of timer overflow, the delay is computed correctly*/

    return 0;
}

board_t utils_get_board_id()
{
    static board_t board = BOARD_NOT_SAMPLED;

    if (board == BOARD_NOT_SAMPLED)
    {
        board = HAL_GPIO_ReadPin(BOARD_ID_0_GPIO_Port, BOARD_ID_0_Pin) | (HAL_GPIO_ReadPin(BOARD_ID_1_GPIO_Port, BOARD_ID_1_Pin) << 1);
    }

    return board;
}
