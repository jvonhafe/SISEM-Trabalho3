#include "led.h"

LED_t led = {1, 500, 0};

void LED_Init(void)
{
    led.blink_mode = 1;
    led.desired_ms = 500;
    led.elapsed_ms = 0;
}

void LED_TimerTask(void)
{
    if (led.blink_mode)
    {
        led.elapsed_ms += 100;

        if (led.elapsed_ms >= led.desired_ms)
        {
            DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
            led.elapsed_ms = 0;
        }
    }
}

void LED_ButtonTask(void)
{
    led.blink_mode = !led.blink_mode;

    if (!led.blink_mode)
    {
        DIGITAL_IO_SetOutputHigh(&DIGITAL_IO_0);
    }
}

void LED_SetSpeed(uint32_t ms)
{
    led.desired_ms = ms;
}
