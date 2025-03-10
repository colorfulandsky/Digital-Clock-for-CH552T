#include "ds1302.h"
#include <reg51.h>
#include "OLED.h"
#include "page.h"
#include "key.h"

void IsAlarmTime();

sbit LED = P3^ 0;


//主函数
void main()
{
    unsigned char keycode;

    LED = 0;

    OLED_Init();
    DS1302_Init();
    while (1)
    {
        if(blink_flag==1) blink_flag=0;        //闪烁标识位，放在循环内执行，屏幕上的闪烁元素就需要根据此实现
        else blink_flag=1;

        ShowPage_Home();
        keycode=Key_Scan();
        if(keycode!=0)
        {
            ShowPage_Menu();
        }
        IsAlarmTime();
    }
}


/// @brief 是否到了响铃时刻
void IsAlarmTime()
{
    unsigned char i;
    unsigned char isSame = 1; // 标识位，如果两个时间相等则为1，否则为0，默认为1

    if (isAlarmOn == 1)     //只有闹铃开启的时候，才判断是否响
    {
        // 先判断是否到了开灯时间
        for (i = 0; i <= 3; i++)
        {
            if (DS1302_Time[i] != Alarm_Time[i]) // 一旦两个字符串有任意一位不相等的，则标志位isSame=0
            {
                isSame = 0; // 没必要继续判断了，跳出循环
                break;
            }
        }
        if (isSame == 1 ) // 如果经过上面的判断，标志位一直是1，此时说明响铃时间已经到了。需要注意的一点是，只有秒数为00的那一秒响铃，否则手动结束以后会一直响
        {
            if(DS1302_Time[4]==0 && DS1302_Time[5]==0)
            {
                blink_flag=0;       //考虑到响铃时刻的冒号闪烁难以实现，所以响铃时刻不会闪烁，但要确保进入程序前闪烁标识符为0，否则将不会显示冒号
                ShowPage_AlarmOn();
            }
        }
    }
}


