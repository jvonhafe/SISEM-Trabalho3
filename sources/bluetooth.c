#include "bluetooth.h"
#include "uart.h"
#include "lcd.h"

#include <string.h>

static uint8_t bt_rx;

void Bluetooth_Init(void)
{
    Bluetooth_SendString("Bluetooth OK\r\n");
}

void Bluetooth_SendString(char *msg)
{
    UART_Transmit(&UART_1,
                  (uint8_t *)msg,
                  strlen(msg));
}

void Bluetooth_Task(void)
{
    if (UART_Receive(&UART_1,
                     &bt_rx,
                     1) == UART_STATUS_SUCCESS)
    {
        if (bt_rx != 0)
        {
            Bluetooth_ProcessCommand(bt_rx);
            bt_rx = 0;
        }
    }
}

void Bluetooth_ProcessCommand(uint8_t cmd)
{
    if(cmd == '3')
    {
        LCD_SetPage(2);

        LCD_ShowMessage("Ola ISEP");

        Bluetooth_SendString("Pagina Bluetooth\r\n");

        return;
    }

    APP_UART_ProcessCommand(cmd);
}
