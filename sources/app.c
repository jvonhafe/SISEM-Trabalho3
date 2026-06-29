#include "app.h"
#include "uart.h"
#include "adc.h"
#include "led.h"
#include "aht10.h"
#include "can_app.h"
#include "lcd.h"

void APP_Init(void)
{
    LED_Init();

    APP_UART_Init();

    APP_AHT10_Init();

    LCD_Init();
}

void APP_Run(void)
{
    ADC_Update();

    APP_UART_Task();

    LCD_Task();

    APP_CAN_Task();
}
