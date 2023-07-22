#ifndef CAN_COMMS_H_
#define CAN_COMMS_H_

#include "can.h"
#include "utils.h"

#define CAN_BOOTLOADER_ID (0x1F + 2 * utils_get_board_id())
#define CAN_AS_STATE_ID 0x027
#define CAN_ASSI_SYNC_ID 0x1AA

// Tx
#define CAN_AIR_BRAKE_PRESS_ID 0x006 // TODO: change

void can_msg_send(CAN_HandleTypeDef *hcan, uint16_t id, uint8_t aData[], uint8_t dlc);
#endif
