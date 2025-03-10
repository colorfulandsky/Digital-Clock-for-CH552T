#ifndef _ds18b20_H
#define _ds18b20_H

#include "public.h"

//管脚定义
sbit DS18B20_PORT=P3^7;	//DS18B20数据口定义

//全局变量
extern unsigned int Envior_Temp;        //全局环境温度变量
extern unsigned char Temp_flag;         //温度正负标识位，1为正温度，0为负温度


//函数声明
unsigned char ds18b20_init(void);
void ds18b20_read_temperture(void);

#endif