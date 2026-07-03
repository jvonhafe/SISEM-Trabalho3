#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "DAVE.h"
#include <stdint.h>

void Bluetooth_Init(void);
void Bluetooth_Task(void);
void Bluetooth_SendString(char *msg);
void Bluetooth_ProcessCommand(uint8_t cmd);

extern char bt_message[17];

#endif
