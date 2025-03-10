#include <reg51.h>
#include <stdio.h>
#include <string.h>
 
extern unsigned char DS1302_Time[];//声明设置时间的数组
extern unsigned char DS1302_Date[];
extern unsigned char DS1302_Week;


void DS1302_Init();//声明初始化函数
void DS1302_WriteByte(unsigned char command,Data);//声明单字节写入函数
unsigned char DS1302_ReadByte(unsigned char command);//声明单字节读出函数
void DS1302_SetTime();//声明设置内部时间函数
void DS1302_SetDate();
void DS1302_SetWeek();
void DS1302_ReadTime();//声明读取内部时间函数