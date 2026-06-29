#include "uart.h"
#include "adc.h"
#include "led.h"
#include "aht10.h"
#include "can_app.h"
#include "lcd.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

UART_DATA_t uart_data = {0};

void APP_UART_SendString(char *msg)
{
    UART_Transmit(&UART_0, (uint8_t*)msg, strlen(msg));
}

void APP_UART_Init(void)
{
    APP_UART_SendString(
        "Grupo 08\r\n"
        "Gustavo - 1241402\r\n"
        "Joao - 1241237\r\n");
}

void APP_UART_Task(void)
{
    if(UART_Receive(&UART_0,
                    &uart_data.rx_char,
                    1) == UART_STATUS_SUCCESS)
    {
        if(uart_data.rx_char != 0)
        {
            APP_UART_ProcessCommand(
                uart_data.rx_char);

            uart_data.rx_char = 0;
        }
    }
}

void APP_UART_ProcessCommand(uint8_t cmd)
{
	if(cmd == '+')
	{
	    LED_SetSpeed(led.desired_ms + 100);

	    snprintf(uart_data.tx_buffer,
	             sizeof(uart_data.tx_buffer),
	             "Blink = %lu ms\r\n",
	             led.desired_ms);

	    APP_UART_SendString(uart_data.tx_buffer);
	}

	else if(cmd == '-')
	{
	    if(led.desired_ms > 100)
	    {
	        LED_SetSpeed(led.desired_ms - 100);
	    }

	    snprintf(uart_data.tx_buffer,
	             sizeof(uart_data.tx_buffer),
	             "Blink = %lu ms\r\n",
	             led.desired_ms);

	    APP_UART_SendString(uart_data.tx_buffer);
	}

	else if(cmd == 'l')
	{
	    LED_ButtonTask();

	    if(led.blink_mode)
	    {
	        APP_UART_SendString("LED = blinking\r\n");
	    }
	    else
	    {
	        APP_UART_SendString("LED = ON\r\n");
	    }
	}

	else if(cmd == 'b')
	{
	    snprintf(uart_data.tx_buffer,
	             sizeof(uart_data.tx_buffer),
	             "Blink = %lu ms\r\n",
	             led.desired_ms);

	    APP_UART_SendString(uart_data.tx_buffer);
	}

    if(cmd == 'a')
    {
        snprintf(uart_data.tx_buffer,
                 sizeof(uart_data.tx_buffer),
                 "ADC = %d\r\n",
                 ADC_GetValue());

        APP_UART_SendString(
            uart_data.tx_buffer);
    }

    else if(cmd == 't')
    {
        if(APP_AHT10_Read())
        {
            int temp =
                (int)(aht10.temperature * 100);

            snprintf(
                uart_data.tx_buffer,
                sizeof(uart_data.tx_buffer),
                "Temp = %d.%02d C\r\n",
                temp/100,
                abs(temp%100));

            APP_UART_SendString(
                uart_data.tx_buffer);
        }
        else
        {
            APP_UART_SendString(
                "Erro AHT10\r\n");
        }
    }

    else if(cmd == 'h')
    {
        if(APP_AHT10_Read())
        {
            int hum =
                (int)(aht10.humidity * 100);

            snprintf(
                uart_data.tx_buffer,
                sizeof(uart_data.tx_buffer),
                "Hum = %d.%02d %%\r\n",
                hum/100,
                abs(hum%100));

            APP_UART_SendString(
                uart_data.tx_buffer);
        }
        else
        {
            APP_UART_SendString(
                "Erro AHT10\r\n");
        }
    }

    else if(cmd == 'c')
    {
        APP_AHT10_Read();

        int temp =
            (int)(aht10.temperature * 100);

        int hum =
            (int)(aht10.humidity * 100);

        uint16_t id =
            APP_CAN_GetTxID();

        snprintf(
            uart_data.tx_buffer,
            sizeof(uart_data.tx_buffer),

            "CAN ID %u / 0x%03X\r\n"
            "Temp = %d.%02d C\r\n"
            "Hum = %d.%02d %%\r\n",

            id,
            id,

            temp/100,
            abs(temp%100),

            hum/100,
            abs(hum%100)
        );

        APP_UART_SendString(
            uart_data.tx_buffer);
    }

    else if(cmd == 'n')
    {
        APP_CAN_NextTxID();

        snprintf(
            uart_data.tx_buffer,
            sizeof(uart_data.tx_buffer),

            "Novo CAN TX ID = %u / 0x%03X\r\n",

            APP_CAN_GetTxID(),
            APP_CAN_GetTxID()
        );

        APP_UART_SendString(
            uart_data.tx_buffer);
    }

    else if(cmd == 'p')
    {
        APP_CAN_PrevTxID();

        snprintf(
            uart_data.tx_buffer,
            sizeof(uart_data.tx_buffer),

            "Novo CAN TX ID = %u / 0x%03X\r\n",

            APP_CAN_GetTxID(),
            APP_CAN_GetTxID()
        );

        APP_UART_SendString(
            uart_data.tx_buffer);
    }
    else if(cmd == '1')
    {
        LCD_SetPage(0);

        APP_UART_SendString("Pagina Temperatura\r\n");
    }

    else if(cmd == '2')
    {
        LCD_SetPage(1);

        APP_UART_SendString("Pagina CAN\r\n");
    }
}
