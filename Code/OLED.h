
#ifndef __OLED_H__
#define __OLED_H__

#include "I2C.h"

#ifndef uint
#define uint unsigned int
#endif
#ifndef uchar
#define uchar unsigned char
#endif

/******************************************/
#define OLED_Addr 0x78 //OLED器件地址(bit0=1读，bit0=0写)
#define isCmd 0x00     //表示写入的是指令
#define isData 0x40    //表示写入的是数据
#define OLED_Width 128 //OLED宽度128像素
#define OLED_High 64   //OLED高度64像素

/******************************************/

void Delay_ms(unsigned int n);

void OLED_Write_Byte(uchar WData, uchar cmdORdata);
void OLED_Fill(uchar Fill_Data);
void OLED_Set_Pos(uchar x, uchar y);
void OLED_Init();
void OLED_DisplayChar(uchar x, uchar y, uchar DisplayChar);
void OLED_ShowLargeChar(unsigned char x, unsigned char y, unsigned char no, unsigned char FontStyle);
void OLED_DisplayStr(uchar x, uchar y, uchar *DisplayStr);
void OLED_DisplayCN(uchar x, uchar y, uchar Num);


#endif
