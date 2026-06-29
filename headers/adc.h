#ifndef ADC_H_
#define ADC_H_

#include "DAVE.h"
#include <stdint.h>

typedef struct
{
    uint16_t value;
} ADC_t;

extern ADC_t adc;

void ADC_Update(void);
uint16_t ADC_GetValue(void);

#endif
