#ifndef UART_H_
#define UART_H_

#include "DAVE.h"
#include <stdint.h>

typedef struct
{
    uint8_t rx_char;
    char tx_buffer[128];
} UART_DATA_t;

extern UART_DATA_t uart_data;

void APP_UART_Init(void);
void APP_UART_Task(void);
void APP_UART_ProcessCommand(uint8_t cmd);
void APP_UART_SendString(char *msg);

#endif
