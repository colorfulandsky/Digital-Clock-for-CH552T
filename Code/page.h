#include "ds1302.h"
#include "OLED.h"
#include <reg51.h>
#include "key.h"



sbit Alarm_Pin = P1^0;          //




extern unsigned char blink_flag; //时间显示中，用于闪烁冒号的标识。程序执行周期的过程中，需要反复判断当前显示的时间冒号是否显示，所以引入这个变量

extern unsigned char isAlarmOn;         //具体请看page.c

extern unsigned char Alarm_Time[];      //响铃时间

extern unsigned char isShowSecond;

extern unsigned char isShowYear;

extern unsigned char Time_Fontstyle;

//extern unsigned int Envior_Temp[];




void ShowPage_Home();
void ShowPage_Menu();
void ShowPage_TimeSet();
void ShowPage_DateSet();

void ShowPage_AlarmSet();
void Show_AlarmTimeSet();
void Show_PatternSet();

void ShowPage_AlarmOn();

void Show_DataString(unsigned char form, unsigned char x, unsigned char y);

void Show_WeekString(unsigned char form, unsigned char x, unsigned char y);

void Show_LargeTimeString(unsigned char form, unsigned char FontStyle);

void Show_TimeString(unsigned char x, unsigned char y, unsigned char *TimeString, unsigned char length);

unsigned char Show_SelectBar(unsigned char item);

void AlarmToggle(unsigned char statue);
