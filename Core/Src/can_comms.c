#include "can_comms.h"
#include "assi.h"
#include "main.h"
#include "utils.h"

#include "ws2812_spi.h"

void can_recv(CAN_HandleTypeDef *hcan, uint8_t fifo)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8] = {0};

    if (HAL_CAN_GetRxMessage(hcan, fifo, &RxHeader, RxData) != HAL_OK)
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
            assi_set_state(RxData[1]);
        }
    }
    else if (RxHeader.StdId == CAN_ASSI_SYNC_ID && RxHeader.DLC == 1)
    {
        assi_sync(RxData[0]);
    }
}

void can_msg_send(CAN_HandleTypeDef *hcan, uint16_t id, uint8_t aData[], uint8_t dlc, uint32_t TimeOut)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t tx_mailbox;
    static uint32_t can_counter_100us = 0;
    can_counter_100us = tick_get_100us();

    TxHeader.StdId = id;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = dlc;
    TxHeader.TransmitGlobalTime = DISABLE;

    while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) < 1)
    {
        if (delay_fun(&can_counter_100us, TimeOut))
        {
            // Error_Handler();
            // HAL_CAN_ResetError(hcan);
            HAL_CAN_AbortTxRequest(hcan, tx_mailbox);
        }
    }

    if (HAL_CAN_AddTxMessage(hcan, &TxHeader, aData, &tx_mailbox) != HAL_OK)
    {
        /* Transmission request Error */
        // Error_Handler();
        // HAL_CAN_ResetError(hcan);
        HAL_CAN_AbortTxRequest(hcan, tx_mailbox);
    }
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    can_recv(hcan, CAN_RX_FIFO1);
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    can_recv(hcan, CAN_RX_FIFO0);
}
