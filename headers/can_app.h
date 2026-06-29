#ifndef CAN_APP_H_
#define CAN_APP_H_

#include "DAVE.h"
#include <stdint.h>

#define CAN_GROUP_ID 1210U

void APP_CAN_RequestTx(void);
void APP_CAN_Task(void);
void APP_CAN_Transmit(uint16_t id, uint8_t *data, uint8_t length);

extern volatile uint16_t can_rx_id;
extern volatile uint8_t can_rx_dlc;
extern volatile uint8_t can_rx_data[8];
extern volatile uint8_t can_rx_flag;

void CAN_A_RX(void);

void APP_CAN_NextTxID(void);
void APP_CAN_PrevTxID(void);
uint16_t APP_CAN_GetTxID(void);

#endif
