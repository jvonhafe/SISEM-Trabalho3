#ifndef LED_H_
#define LED_H_

#include "DAVE.h"
#include <stdint.h>

typedef struct
{
    uint8_t blink_mode;
    uint32_t desired_ms;
    uint32_t elapsed_ms;
} LED_t;

extern LED_t led;

void LED_Init(void);
void LED_TimerTask(void);
void LED_ButtonTask(void);
void LED_SetSpeed(uint32_t ms);

#endif
