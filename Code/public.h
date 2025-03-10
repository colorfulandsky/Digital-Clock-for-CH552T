#ifndef _public_H
#define _public_H

#include "reg51.h"

typedef unsigned int u16;	//对系统默认数据类型进行重定义
typedef unsigned char u8;
typedef unsigned long u32;

void delay_10us(u16 ten_us);
void delay_ms(u16 ms);

#endif
