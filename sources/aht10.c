#include "aht10.h"

#define AHT10_ADDR 0x38

APP_AHT10_t aht10 = {0};

void APP_AHT10_Init(void)
{
    uint8_t reset_cmd = 0xBA;
    uint8_t init_cmd[3] = {0xE1, 0x08, 0x00};

    /* Reset */
    I2C_MASTER_Transmit(&I2C_MASTER_0,
                        true,
                        (AHT10_ADDR << 1),
                        &reset_cmd,
                        1,
                        true);

    for(volatile uint32_t i = 0; i < 300000; i++);

    /* Init / Calibrate */
    I2C_MASTER_Transmit(&I2C_MASTER_0,
                        true,
                        (AHT10_ADDR << 1),
                        init_cmd,
                        3,
                        true);

    for(volatile uint32_t i = 0; i < 300000; i++);
}

uint8_t APP_AHT10_Read(void)
{
    uint8_t cmd[3] = {0xAC, 0x33, 0x00};
    uint8_t data[6];
    uint8_t tries = 10;

    /* Start measurement */
    if(I2C_MASTER_Transmit(&I2C_MASTER_0,
                           true,
                           (AHT10_ADDR << 1),
                           cmd,
                           3,
                           true) != I2C_MASTER_STATUS_SUCCESS)
    {
        return 0;
    }

    do
    {
        for(volatile uint32_t i = 0; i < 250000; i++);

        if(I2C_MASTER_Receive(&I2C_MASTER_0,
                              true,
                              (AHT10_ADDR << 1),
                              data,
                              6,
                              true,
                              true) != I2C_MASTER_STATUS_SUCCESS)
        {
            return 0;
        }

        tries--;

    } while((data[0] & 0x80) && tries);

    if(tries == 0)
    {
        return 0;
    }

    uint32_t raw_hum =
        ((uint32_t)data[1] << 12) |
        ((uint32_t)data[2] << 4)  |
        ((uint32_t)data[3] >> 4);

    uint32_t raw_temp =
        ((uint32_t)(data[3] & 0x0F) << 16) |
        ((uint32_t)data[4] << 8) |
        ((uint32_t)data[5]);

    aht10.humidity =
        ((float)raw_hum * 100.0f) / 1048576.0f;

    aht10.temperature =
        ((float)raw_temp * 200.0f) / 1048576.0f - 50.0f;

    aht10.valid = 1;

    return 1;
}
