#ifndef LCD_H_
#define LCD_H_

#include "DAVE.h"

#define SLAVE_ADDRESS_LCD 0x4E

void LCD_Init(void);
void LCD_Task(void);
void LCD_Send_Cmd(char cmd);
void LCD_Send_Data(char data);
void LCD_Send_String(char *str);
void LCD_Set_Cursor(int row, int col);
void LCD_Clear(void);
void LCD_ShowTemperatureHumidity(void);
void LCD_ShowCAN(void);
void LCD_SetPage(uint8_t page);
void LCD_ShowMessage(char *msg);

uint8_t LCD_GetPage(void);

#endif
