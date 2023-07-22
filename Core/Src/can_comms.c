#include "can_comms.h"
#include "assi.h"
#include "main.h"
#include "tim.h"
#include "utils.h"

#include "ws2812_spi.h"

void can_recv(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8] = {0};

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
    {
        /* Transmission request Error */
        HAL_CAN_ResetError(hcan);
        // Error_Handler();
    }

    /*Reboot Board - Received command byte from CAN*/
    if ((RxHeader.StdId == CAN_BOOTLOADER_ID) && (RxHeader.DLC == 2) && (RxData[0] == 0xFF) && (RxData[1] == 0x00))
    {
        NVIC_SystemReset();
    }
    else if ((RxHeader.StdId == CAN_AS_STATE_ID))
    {
        if (RxData[1] <= AS_TEST)
        {
            HAL_TIM_Base_Start_IT(&TIM_DSPACE_TIMEOUT); // start at first message
            __HAL_TIM_SET_COUNTER(&TIM_DSPACE_TIMEOUT, 0);

            assi_set_state(RxData[1]);
        }
    }
    else if (RxHeader.StdId == CAN_ASSI_SYNC_ID && RxHeader.DLC == 1)
    {
        if (RxData[0] <= 1)
        {
            __HAL_TIM_SET_COUNTER(&TIM_SYNC_TIMEOUT, 0); // reset timeout timer
            if (utils_get_board_id() != BOARD_MASTER)
            { // stop internal sync if not master
                HAL_TIM_Base_Stop(&TIM_SYNC);
            }

            assi_sync(RxData[0]);
        }
    }
}

void can_msg_send(CAN_HandleTypeDef *hcan, uint16_t id, uint8_t aData[], uint8_t dlc)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t tx_mailbox;

    TxHeader.StdId = id;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = dlc;
    TxHeader.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage(hcan, &TxHeader, aData, &tx_mailbox) != HAL_OK)
    {
        HAL_CAN_ResetError(hcan);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    can_recv(hcan);
}
