#ifndef AHT10_H_
#define AHT10_H_

#include "DAVE.h"
#include <stdint.h>

typedef struct
{
    float temperature;
    float humidity;
    uint8_t valid;
} APP_AHT10_t;

extern APP_AHT10_t aht10;

void APP_AHT10_Init(void);
uint8_t APP_AHT10_Read(void);

#endif
