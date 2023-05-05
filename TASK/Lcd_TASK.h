#ifndef __LCD_TASK_H
#define __LCD_TASL_H
#include "MQTT_TASK.h"
void Lcd_TASK(User_Data_t *User_Data);
void Load_Drow_Dialog(void);
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color);
u16 my_abs(u16 x1,u16 x2);
void Draw_mainpage(void);
void draw_weather(u8 tempdata,u8 humidata);
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color);
void lcdshounum(User_Data_t *User_Data);
#endif










