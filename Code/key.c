#include "key.h"


sbit OK_Key = P3 ^ 3;
sbit TOGGLE_Key = P3 ^ 4;
sbit CANCEL_Key = P3 ^ 5;


/// @brief 单次扫描按键（只扫描一次后正常返回）
unsigned char Key_Scan()
{
    unsigned char keycode = 0;
    if(OK_Key==0)	  //读P1.0引脚，如果引脚为低电平，则进入if
	{
		Delay_ms(10); //延时10ms消抖
		if(OK_Key==0)	 //再次判断按键是否按下，防止干扰，增强稳定
		{			
			while(OK_Key==0);//等待按键松开
            keycode=1;
		}
	}
    if(TOGGLE_Key==0)	  //读P1.0引脚，如果引脚为低电平，则进入if
	{
		Delay_ms(10); //延时10ms消抖
		if(TOGGLE_Key==0)	 //再次判断按键是否按下，防止干扰，增强稳定
		{			
			while(TOGGLE_Key==0);//等待按键松开
            keycode=2;
		}
	}
    if(CANCEL_Key==0)	  //读P1.0引脚，如果引脚为低电平，则进入if
	{
		Delay_ms(10); //延时10ms消抖
		if(CANCEL_Key==0)	 //再次判断按键是否按下，防止干扰，增强稳定
		{			
			while(CANCEL_Key==0);//等待按键松开
            keycode=1;
		}
	}
    return keycode;
}

/// @brief 持续扫描按键，（阻塞语句）如果不按下按键则始终扫描
unsigned char Key_Scan_Blocked()
{
    while(1)
    {
        if(OK_Key==0)	  //读P1.0引脚，如果引脚为低电平，则进入if
	    {
            Delay_ms(10); //延时10ms消抖
            if(OK_Key==0)	 //再次判断按键是否按下，防止干扰，增强稳定
            {			
                while(OK_Key==0);//等待按键松开
                return 1;
            }
	    }
        if(TOGGLE_Key==0)	  //读P1.0引脚，如果引脚为低电平，则进入if
	    {
            Delay_ms(10); //延时10ms消抖
            if(TOGGLE_Key==0)	 //再次判断按键是否按下，防止干扰，增强稳定
            {			
                while(TOGGLE_Key==0);//等待按键松开
                return 2;
            }
	    }
        if(CANCEL_Key==0)	  //读P1.0引脚，如果引脚为低电平，则进入if
	    {
            Delay_ms(10); //延时10ms消抖
            if(CANCEL_Key==0)	 //再次判断按键是否按下，防止干扰，增强稳定
            {			
                while(CANCEL_Key==0);//等待按键松开
                return 3;
            }
	    }
    }
    return 0;
}