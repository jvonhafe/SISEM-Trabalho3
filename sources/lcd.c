#include "lcd.h"

#include "aht10.h"
#include <stdio.h>
#include <stdlib.h>
#include "can_app.h"
#include "bluetooth.h"

extern I2C_MASTER_t I2C_MASTER_0;

static uint8_t lcd_page = 0;

void LCD_Send_Cmd (char cmd)
{
  char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;
	data_t[1] = data_u|0x08;
	data_t[2] = data_l|0x0C;
	data_t[3] = data_l|0x08;

	I2C_MASTER_Transmit(&I2C_MASTER_0,
						true,
						SLAVE_ADDRESS_LCD,
						(uint8_t *) data_t,
						4,
						true);

	for(volatile uint32_t i = 0; i < 20000; i++);
}

void LCD_Send_Data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;
	data_t[1] = data_u|0x09;
	data_t[2] = data_l|0x0D;
	data_t[3] = data_l|0x09;

	I2C_MASTER_Transmit(&I2C_MASTER_0,
						true,
						SLAVE_ADDRESS_LCD,
						(uint8_t *) data_t,
						4,
						true);

	for(volatile uint32_t i = 0; i < 20000; i++);
}

void LCD_Clear (void)
{
	LCD_Send_Cmd (0x01);
}

void LCD_Set_Cursor(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;

        case 1:
            col |= 0xC0;
            break;
    }

    LCD_Send_Cmd(col);
}


void LCD_Init(void)
{
    LCD_Send_Cmd(0x33);
    LCD_Send_Cmd(0x32);

    LCD_Send_Cmd(0x28);

    LCD_Send_Cmd(0x0C);

    LCD_Send_Cmd(0x01);

    LCD_Send_Cmd(0x06);

    LCD_SetPage(1);

    for(volatile uint32_t i = 0; i < 300000; i++);
}

void LCD_Send_String(char *str)
{
    while (*str)
    {
        LCD_Send_Data(*str++);

        for (volatile uint32_t i = 0; i < 5000; i++);
    }
}

void LCD_Task(void)
{
    static uint32_t counter = 0;

    counter++;

    if(counter >= 500000)
    {
        counter = 0;

        if(aht10.valid)
        {
        	switch(lcd_page)
        	{
        	    case 0:
        	        LCD_ShowTemperatureHumidity();
        	        break;

        	    case 1:
        	        LCD_ShowCAN();
        	        break;

        	    case 2:
        	        break;
        	}
        }
    }
}

void LCD_ShowTemperatureHumidity(void)
{
    char line[17];

    int temp = (int)(aht10.temperature * 100);
    int hum  = (int)(aht10.humidity * 100);

    LCD_Set_Cursor(0,0);

    LCD_Set_Cursor(0,0);
    sprintf(line, "Temp:%d.%02dC",
            temp / 100,
            abs(temp % 100));
    LCD_Send_String(line);

    LCD_Set_Cursor(1,0);
    sprintf(line, "Hum :%d.%02d%%",
            hum / 100,
            abs(hum % 100));
    LCD_Send_String(line);
}

void LCD_ShowCAN(void)
{
    char line[17];

    LCD_Set_Cursor(0,0);

    sprintf(line,"CAN:%4u",can_rx_id);
    LCD_Send_String(line);

    LCD_Set_Cursor(1,0);

    if(can_rx_flag)
    {
        LCD_Send_String("RX: OK");
    }
    else
    {
    	LCD_Set_Cursor(0,0);
    	LCD_Send_String("CAN RX");

    	LCD_Set_Cursor(1,0);
        LCD_Send_String("Waiting...");
    }
}

void LCD_SetPage(uint8_t page)
{
    lcd_page = page;
}

uint8_t LCD_GetPage(void)
{
    return lcd_page;
}

void LCD_ShowMessage(char *msg)
{
    LCD_Clear();

    LCD_Set_Cursor(0,0);
    LCD_Send_String(msg);
}
