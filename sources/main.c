#include "DAVE.h"
#include "app.h"
#include "led.h"
#include "can_app.h"

void TIMER_0_InterruptHandler(void)
{
    static uint8_t can_counter = 0;

    LED_TimerTask();

    can_counter++;

    if(can_counter >= 5)
    {
        can_counter = 0;
        APP_CAN_RequestTx();
    }
}

void PIN_INTERRUPT_0_InterruptHandler(void)
{
    LED_ButtonTask();
}

int main(void)
{
    if (DAVE_Init() != DAVE_STATUS_SUCCESS)
        while(1);

    APP_Init();

    while(1)
    {
        APP_Run();
    }
}
