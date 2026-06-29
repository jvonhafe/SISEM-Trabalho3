#include "adc.h"

ADC_t adc = {0};

void ADC_Update(void)
{
    adc.value = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Channel_A_handle);
}

uint16_t ADC_GetValue(void)
{
    return adc.value;
}
