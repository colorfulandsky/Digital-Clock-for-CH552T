#include "page.h"

unsigned char blink_flag = 0; // 闪烁标识，用于主页的时间字符串的闪烁。

unsigned char isAlarmOn = 1; // 标识当前闹钟是否打开，打开为1，关闭为2

unsigned char Alarm_Time[] = {0, 6, 3, 0}; // 设置的闹钟响铃时间，6点30为上电默认时间

unsigned char isShowSecond = 0; // 是否显示秒数

unsigned char isShowYear = 0; // 是否显示年份

unsigned char Time_Fontstyle = 1; // 时间的字体风格

/// @brief 显示主页
void ShowPage_Home()
{
    DS1302_ReadTime(); // 读取时间

    if (isAlarmOn == 1)
    {
        OLED_DisplayCN(0, 0, 23); // 根据当前闹钟开关状态，判断是否需要显示闹钟标志符号
        Show_TimeString(16, 0, Alarm_Time, 4);
    }
    // else OLED_DisplayCN(0,0,19);

    Show_LargeTimeString(isShowSecond, Time_Fontstyle); // 显示主页的大号时间（参数有是否显示秒，字体风格）
    Show_DataString(isShowYear, 0, 6);                  // 显示主页的日期（参数有是否显示年份）
    Show_WeekString(0, 80, 6);                          // 显示星期几
}

/// @brief 显示菜单页面
void ShowPage_Menu()
{
    unsigned char option;

    OLED_Fill(0);

    OLED_DisplayCN(16, 0, 10); // 设置时间
    OLED_DisplayCN(32, 0, 11);
    OLED_DisplayCN(48, 0, 12);
    OLED_DisplayCN(64, 0, 13);

    OLED_DisplayCN(16, 2, 10); // 设置日期星期
    OLED_DisplayCN(32, 2, 11);
    OLED_DisplayCN(48, 2, 14);
    OLED_DisplayCN(64, 2, 15);
    OLED_DisplayCN(80, 2, 0);
    OLED_DisplayCN(96, 2, 1);

    OLED_DisplayCN(16, 4, 10); // 设置闹钟
    OLED_DisplayCN(32, 4, 11);
    OLED_DisplayCN(48, 4, 16);
    OLED_DisplayCN(64, 4, 17);

    OLED_DisplayCN(16, 6, 24); // 样式设置
    OLED_DisplayCN(32, 6, 25);
    OLED_DisplayCN(48, 6, 10);
    OLED_DisplayCN(64, 6, 11);

    option = Show_SelectBar(4); // 此处阻塞

    switch (option)
    {
    case 1:
        ShowPage_TimeSet();
        break;
    case 2:
        ShowPage_DateSet();
        break;
    case 3:
        ShowPage_AlarmSet();
        break;
    case 4:
        Show_PatternSet();
        break;
    default:
        break;
    }
    OLED_Fill(0);
}

/// @brief 显示调整时间的界面
void ShowPage_TimeSet()
{
    unsigned char i = 0; // 用于标识当前正调整的位

    OLED_Fill(0);
    OLED_DisplayCN(32, 0, 10); // 设置时间
    OLED_DisplayCN(48, 0, 11);
    OLED_DisplayCN(64, 0, 12);
    OLED_DisplayCN(80, 0, 13);
    while (i <= 3)
    {
        Show_TimeString(44, 2, DS1302_Time, 4);
        OLED_DisplayStr(44, 4, "        ");
        if (i > 1)
            OLED_DisplayChar(44 + i * 8 + 8, 4, 127); // 显示小箭头指示调整的位,此处if作用是，如果调整分钟数【i》1】情况，需要跳过一个冒号。
        else
            OLED_DisplayChar(44 + i * 8, 4, 127);

        switch (Key_Scan_Blocked())
        {
        case 1: // ok键，当前位+1
            DS1302_Time[i]++;
            if (DS1302_Time[i] > 9)
                DS1302_Time[i] = 0; // 任意一位都不允许大于9
            break;
        case 2: // 切位键，调整下一位
            i++;
            break;
        case 3: // 取消键，退出调整程序（不保存）
            return;
        }

        if (DS1302_Time[0] * 10 + DS1302_Time[1] > 23 | DS1302_Time[0] * 10 + DS1302_Time[1] < 0) // 判断当前时间的合法性
        {
            DS1302_Time[0] = 0;
            DS1302_Time[1] = 0;
        }
        if (DS1302_Time[2] * 10 + DS1302_Time[3] > 59 | DS1302_Time[2] * 10 + DS1302_Time[3] < 0)
        {
            DS1302_Time[2] = 0;
            DS1302_Time[3] = 0;
        }
    }
    DS1302_SetTime();
}

/// @brief 显示调整日期的界面
void ShowPage_DateSet()
{
    unsigned char i = 2; // 用于标识当前正调整的位,由于开头的20xx是固定的，所以直接从第二位开始调整

    OLED_Fill(0);
    OLED_DisplayCN(16, 0, 10); // 设置日期星期
    OLED_DisplayCN(32, 0, 11);
    OLED_DisplayCN(48, 0, 14);
    OLED_DisplayCN(64, 0, 15);
    OLED_DisplayCN(80, 0, 0);
    OLED_DisplayCN(96, 0, 1);

    while (i <= 8)
    {
        Show_DataString(1, 1, 2);  // 屏幕先列出当前的日期
        Show_WeekString(1, 96, 2); // 再列出当前星期
        OLED_DisplayStr(0, 4, "                ");

        // 判断指示调整位的小箭头应该显示在哪里
        if (i == 4 | i == 5)
            OLED_DisplayChar(0 + i * 8 + 8, 4, 127); // 显示小箭头指示调整的位,此处if作用是，如果调整月份【i》3】情况，需要跳过一个冒号。
        else if (i == 6 | i == 7)
            OLED_DisplayChar(0 + i * 8 + 16, 4, 127); // 显示小箭头指示调整的位,此处if作用是，如果调整日期【i》5】情况，需要跳过2个冒号。
        else if (i == 8)
            OLED_DisplayChar(119, 4, 127); // i=8时，调整的是星期，箭头显示到星期处
        else
            OLED_DisplayChar(0 + i * 8, 4, 127);

        switch (Key_Scan_Blocked()) // 扫描按键，调整日期中
        {
        case 1:        // ok键，当前位+1
            if (i < 8) // 处于调整日期的状态下
            {
                DS1302_Date[i]++;
                if (DS1302_Date[i] > 9)
                    DS1302_Date[i] = 0; // 任意一位都不允许大于9
            }
            else // 调整的是星期
            {
                DS1302_Week++;
                if (DS1302_Week == 8)
                    DS1302_Week = 1;
            }
            break;
        case 2: // 切位键，调整下一位
            i++;
            break;
        case 3: // 取消键，退出调整程序（不保存）
            return;
        }

        if (DS1302_Date[2] * 10 + DS1302_Date[3] > 99) // 判断当前日期的合法性
        {
            DS1302_Date[2] = 0;
            DS1302_Date[3] = 0;
        }
        if (DS1302_Date[4] * 10 + DS1302_Date[5] > 12 | DS1302_Date[4] * 10 + DS1302_Date[5] < 1)
        {
            DS1302_Date[4] = 0;
            DS1302_Date[5] = 1;
        }
        if (DS1302_Date[6] * 10 + DS1302_Date[7] > 31 | DS1302_Date[6] * 10 + DS1302_Date[7] < 1)
        {
            DS1302_Date[6] = 0;
            DS1302_Date[7] = 1;
        }
    }
    DS1302_SetDate();
    DS1302_SetWeek();
}

/// @brief 显示闹钟设置
void ShowPage_AlarmSet()
{
    unsigned char option;

    OLED_Fill(0);

    OLED_DisplayCN(16, 0, 16); // 闹钟时间
    OLED_DisplayCN(32, 0, 17);
    OLED_DisplayCN(48, 0, 12);
    OLED_DisplayCN(64, 0, 13);
    Show_TimeString(80, 0, Alarm_Time, 4);

    OLED_DisplayCN(16, 2, 16); // 闹钟开（或闹钟关，根据isalarmon的值来确定显示什么）
    OLED_DisplayCN(32, 2, 17);
    if (isAlarmOn == 0)
        OLED_DisplayCN(48, 2, 20);
    else
        OLED_DisplayCN(48, 2, 21);

    option = Show_SelectBar(2);
    switch (option)
    {
    case 1:
        Show_AlarmTimeSet();
        break;
    case 2:
        if (isAlarmOn == 0)
            isAlarmOn = 1;
        else
            isAlarmOn = 0;
        break;
    default:
        break;
    }
}

/// @brief 显示调整闹钟时间的子页面
void Show_AlarmTimeSet()
{
    unsigned char i = 0; // 用于标识当前正调整的位

    OLED_Fill(0);
    OLED_DisplayCN(32, 0, 16); // 闹钟时间
    OLED_DisplayCN(48, 0, 17);
    OLED_DisplayCN(64, 0, 12);
    OLED_DisplayCN(80, 0, 13);
    while (i <= 3)
    {
        Show_TimeString(44, 2, Alarm_Time, 4);
        OLED_DisplayStr(44, 4, "        ");
        if (i > 1)
            OLED_DisplayChar(44 + i * 8 + 8, 4, 127); // 显示小箭头指示调整的位,此处if作用是，如果调整分钟数【i》1】情况，需要跳过一个冒号。
        else
            OLED_DisplayChar(44 + i * 8, 4, 127);

        switch (Key_Scan_Blocked())
        {
        case 1: // ok键，当前位+1
            Alarm_Time[i]++;
            if (Alarm_Time[i] > 9)
                Alarm_Time[i] = 0; // 任意一位都不允许大于9
            break;
        case 2: // 切位键，调整下一位
            i++;
            break;
        case 3: // 取消键，退出调整程序（不保存）
            return;
        }

        if (Alarm_Time[0] * 10 + Alarm_Time[1] > 23) // 判断当前时间的合法性
        {
            Alarm_Time[0] = 0;
            Alarm_Time[1] = 0;
        }
        if (Alarm_Time[2] * 10 + Alarm_Time[3] > 59)
        {
            Alarm_Time[2] = 0;
            Alarm_Time[3] = 0;
        }
    }
}

/// @brief 显示样式切换的页面
void Show_PatternSet()
{
    unsigned char option;

    OLED_Fill(0);

    OLED_DisplayCN(16, 0, 12); // 时间样式设置
    OLED_DisplayCN(32, 0, 13);
    OLED_DisplayCN(48, 0, 24);
    OLED_DisplayCN(64, 0, 25);
    OLED_DisplayCN(80, 0, 10);
    OLED_DisplayCN(96, 0, 11);

    OLED_DisplayCN(16, 2, 14); // 日期样式设置
    OLED_DisplayCN(32, 2, 15);
    OLED_DisplayCN(48, 2, 24);
    OLED_DisplayCN(64, 2, 25);
    OLED_DisplayCN(80, 2, 10);
    OLED_DisplayCN(96, 2, 11);

    OLED_DisplayCN(16, 4, 12); // 时钟风格
    OLED_DisplayCN(32, 4, 17);
    OLED_DisplayCN(48, 4, 26);
    OLED_DisplayCN(64, 4, 27);

    // OLED_DisplayCN(16, 6, 29); //显示亮度调整
    // OLED_DisplayCN(32, 6, 30);
    // OLED_DisplayCN(48, 6, 43);
    // OLED_DisplayCN(64, 6, 44);
    // OLED_DisplayCN(80, 6, 45);
    // OLED_DisplayCN(96, 6, 46);
    OLED_DisplayStr(16, 6, "About");

    option = Show_SelectBar(4); // 此处阻塞

    switch (option)
    {
    case 1:
        OLED_Fill(0);
        OLED_DisplayCN(16, 0, 29); // 显示秒数
        OLED_DisplayCN(32, 0, 30);
        OLED_DisplayCN(48, 0, 31);
        OLED_DisplayCN(64, 0, 32);

        OLED_DisplayCN(16, 2, 28);
        OLED_DisplayCN(32, 2, 29); // 不显示秒数
        OLED_DisplayCN(48, 2, 30);
        OLED_DisplayCN(64, 2, 31);
        OLED_DisplayCN(80, 2, 32);

        option = Show_SelectBar(2); // 此处阻塞
        if (option == 1)
            isShowSecond = 1;
        else if (option == 2)
            isShowSecond = 0;

        break;
    case 2:
        OLED_Fill(0);
        OLED_DisplayCN(16, 0, 29); // 显示年份
        OLED_DisplayCN(32, 0, 30);
        OLED_DisplayCN(48, 0, 33);
        OLED_DisplayCN(64, 0, 34);

        OLED_DisplayCN(16, 2, 28);
        OLED_DisplayCN(32, 2, 29); // 不显示年份
        OLED_DisplayCN(48, 2, 30);
        OLED_DisplayCN(64, 2, 33);
        OLED_DisplayCN(80, 2, 34);

        option = Show_SelectBar(2); // 此处阻塞
        if (option == 1)
            isShowYear = 1;
        else if (option == 2)
            isShowYear = 0;

        break;
    case 3:
        OLED_Fill(0);
        OLED_DisplayCN(16, 0, 37); // 粗体
        OLED_DisplayCN(32, 0, 38);

        // OLED_DisplayCN(16, 2, 39);
        // OLED_DisplayCN(32, 2, 40); // 衬线体
        // OLED_DisplayCN(48, 2, 38);

        OLED_DisplayCN(16, 2, 41);
        OLED_DisplayCN(32, 2, 42); // 数码体
        OLED_DisplayCN(48, 2, 38);

        option = Show_SelectBar(2); // 此处阻塞
        if (option == 1)
            Time_Fontstyle = 0;
        // else if(option==2)  Time_Fontstyle=1;
        else if (option == 2)
            Time_Fontstyle = 1;

        break;
    case 4:
        OLED_Fill(0);

        OLED_DisplayStr(0, 0, "Use 2* AA BAT");
        OLED_DisplayStr(0, 2, "Or: Use Type-C");
        OLED_DisplayStr(0, 4, "DC Power");
        OLED_DisplayStr(0, 6, "Press Any Key");

        Key_Scan_Blocked();

        OLED_Fill(0);

        OLED_DisplayStr(0, 0, "Version: 1.0");
        OLED_DisplayStr(0, 2, "Compile Date:");
        OLED_DisplayStr(0, 4, "2025.1.8");
        OLED_DisplayStr(0, 6, "Press Any Key");

        Key_Scan_Blocked();

        OLED_Fill(0);

        break;
    default:
        break;
    }
    OLED_Fill(0);
}

// /// @brief 显示调整亮度界面
// void ShowPage_BrightnessChange()
// {
//     unsigned char brightness;
//     unsigned char option;

//     OLED_Fill(0);
//     // OLED_DisplayCN(16, 0, 29); //显示亮度调整
//     // OLED_DisplayCN(32, 0, 30);
//     // OLED_DisplayCN(48, 0, 43);
//     // OLED_DisplayCN(64, 0, 44);
//     // OLED_DisplayCN(80, 0, 45);
//     // OLED_DisplayCN(96, 0, 46);

//     OLED_DisplayStr(16,0,"100%");
//     OLED_DisplayStr(16,2,"75%");
//     OLED_DisplayStr(16,4,"50%");
//     OLED_DisplayStr(16,6,"10%");

//     option=Show_SelectBar(4);
//     switch (option)
//     {
//         case 1:
//             brightness=0xff;
//             break;
//         case 2:
//             brightness=0xb2;
//             break;
//         case 3:
//             brightness=0x7f;
//             break;
//         case 4:
//             brightness=0x01;
//             break;
//     }
//         OLED_Write_Byte(0x81, isCmd); //设置对比度
//         OLED_Write_Byte(brightness, isCmd); //对比度,数值越大屏幕越亮，0x00-0xff
// }

/// @brief 显示闹钟正响时刻的界面
void ShowPage_AlarmOn()
{
    unsigned int n = 180;       //循环变量
    unsigned int k = 0xffff;    //循环计数变量

    OLED_Fill(0);

    OLED_DisplayCN(0, 6, 19);
    OLED_DisplayCN(16, 6, 48); // 按任意键关闭
    OLED_DisplayCN(32, 6, 49);
    OLED_DisplayCN(48, 6, 50);
    OLED_DisplayCN(64, 6, 51);
    OLED_DisplayCN(80, 6, 21);
    OLED_DisplayCN(96, 6, 52);
    OLED_DisplayCN(112, 6, 19);

    AlarmToggle(1); // 打开闹钟

    while (n--) // 循环等待按键按下
    {

        while (k--)
        {
            DS1302_ReadTime();
            Show_LargeTimeString(isShowSecond,Time_Fontstyle);  //闹钟响时仍然保持显示当前时间

            OLED_DisplayCN(0, 0, 23);    //显示闹钟标识
            Show_TimeString(16, 0, Alarm_Time, 4);  //显示设定的闹钟时间

            if (Key_Scan() != 0) // 如果有按键按下则立即退出，否则持续循环大约3分钟后关闭
            {
                AlarmToggle(0);
                OLED_Fill(0);
                return;
            }
        }
    }
    AlarmToggle(0);
    OLED_Fill(0);
}

/// @brief 在屏幕指定位置显示日期字符串
/// @param form 显示的形式，1为显示年月日，0为只显示月日
/// @param x 0-127
/// @param y 0-7
void Show_DataString(unsigned char form, unsigned char x, unsigned char y)
{
    if (form == 1)
    {
        OLED_DisplayChar(x, y, '2');                        // 显示年份千位
        OLED_DisplayChar(x + 8, y, '0');                    // 百位
        OLED_DisplayChar(x + 16, y, DS1302_Date[2] + 0x30); // 十位
        OLED_DisplayChar(x + 24, y, DS1302_Date[3] + 0x30); // 个位
        OLED_DisplayChar(x + 32, y, '/');                   // 分隔符
        OLED_DisplayChar(x + 40, y, DS1302_Date[4] + 0x30); // 月份十位
        OLED_DisplayChar(x + 48, y, DS1302_Date[5] + 0x30); // 月份个位
        OLED_DisplayChar(x + 56, y, '/');                   // 分隔符
        OLED_DisplayChar(x + 64, y, DS1302_Date[6] + 0x30);
        OLED_DisplayChar(x + 72, y, DS1302_Date[7] + 0x30);
    }
    else if (form == 0)
    {
        OLED_DisplayChar(x, y, DS1302_Date[4] + 0x30);
        OLED_DisplayChar(x + 8, y, DS1302_Date[5] + 0x30);
        OLED_DisplayChar(x + 16, y, '-');
        OLED_DisplayChar(x + 24, y, DS1302_Date[6] + 0x30);
        OLED_DisplayChar(x + 32, y, DS1302_Date[7] + 0x30);
    }
}

/// @brief 指定位置显示星期字符串
/// @param form 0：显示 星期几     1：显示 周几
/// @param x 0-127
/// @param y 0-7
void Show_WeekString(unsigned char form, unsigned char x, unsigned char y)
{
    if (form == 0)
    {
        OLED_DisplayCN(x, y, 0); // 显示汉字：星期
        OLED_DisplayCN(x + 16, y, 1);
        switch (DS1302_Week) // 根据读取到的星期值，来确定显示哪个汉字（一二三四五六日）
        {
        case 1:
            OLED_DisplayCN(x + 32, y, 2);
            break;
        case 2:
            OLED_DisplayCN(x + 32, y, 3);
            break;
        case 3:
            OLED_DisplayCN(x + 32, y, 4);
            break;
        case 4:
            OLED_DisplayCN(x + 32, y, 5);
            break;
        case 5:
            OLED_DisplayCN(x + 32, y, 6);
            break;
        case 6:
            OLED_DisplayCN(x + 32, y, 7);
            break;
        case 7:
            OLED_DisplayCN(x + 32, y, 8);
            break;
        }
    }
    else
    {
        OLED_DisplayCN(x, y, 9);
        switch (DS1302_Week)
        {
        case 1:
            OLED_DisplayCN(x + 16, y, 2);
            break;
        case 2:
            OLED_DisplayCN(x + 16, y, 3);
            break;
        case 3:
            OLED_DisplayCN(x + 16, y, 4);
            break;
        case 4:
            OLED_DisplayCN(x + 16, y, 5);
            break;
        case 5:
            OLED_DisplayCN(x + 16, y, 6);
            break;
        case 6:
            OLED_DisplayCN(x + 16, y, 7);
            break;
        case 7:
            OLED_DisplayCN(x + 16, y, 8);
            break;
        }
    }
}

/// @brief 在oled屏幕正中心显示大号时间
/// @param form 1:显示时分秒 0：只显示时分
/// @param FontStyle 字体风格，0：黑体   2：数码时钟体    1：衬线体
void Show_LargeTimeString(unsigned char form, unsigned char FontStyle)
{
    if (form == 1)
    {
        OLED_ShowLargeChar(0, 2, DS1302_Time[0], FontStyle);
        OLED_ShowLargeChar(16, 2, DS1302_Time[1], FontStyle);
        OLED_ShowLargeChar(32, 2, 10, FontStyle);
        OLED_ShowLargeChar(48, 2, DS1302_Time[2], FontStyle);
        OLED_ShowLargeChar(64, 2, DS1302_Time[3], FontStyle);
        OLED_ShowLargeChar(80, 2, 10, FontStyle);
        OLED_ShowLargeChar(96, 2, DS1302_Time[4], FontStyle);
        OLED_ShowLargeChar(112, 2, DS1302_Time[5], FontStyle);
    }
    else if (form == 0)
    {
        OLED_ShowLargeChar(24, 2, DS1302_Time[0], FontStyle);
        OLED_ShowLargeChar(40, 2, DS1302_Time[1], FontStyle);

        if (blink_flag == 1) // 如果该标识符为1，说明需要显示冒号
        {
            //blink_flag = 0;                           // 同时标识符清零
            OLED_ShowLargeChar(56, 2, 11, FontStyle); // 显示冒号
        }
        else // 如果不为一（为0时）
        {
            //blink_flag = 1;                           // 先标识符置一
            OLED_ShowLargeChar(56, 2, 10, FontStyle); // 再清除冒号显示
        }

        OLED_ShowLargeChar(72, 2, DS1302_Time[2], FontStyle);
        OLED_ShowLargeChar(88, 2, DS1302_Time[3], FontStyle);
    }
}

/// @brief oled指定位置显示时间的字符串，字体为8*16
/// @param x 0-127
/// @param y 0-7
/// @param TimeString 需要显示的时间字符串
/// @param length 需要显示的长度，如果为4则显示时分、为6显示时分秒
void Show_TimeString(unsigned char x, unsigned char y, unsigned char *TimeString, unsigned char length)
{
    OLED_DisplayChar(x, y, TimeString[0] + 0x30);
    OLED_DisplayChar(x + 8, y, TimeString[1] + 0x30);
    OLED_DisplayChar(x + 16, y, ':');
    OLED_DisplayChar(x + 24, y, TimeString[2] + 0x30);
    OLED_DisplayChar(x + 32, y, TimeString[3] + 0x30);
    if (length == 6)
    {
        OLED_DisplayChar(x + 40, y, ':');
        OLED_DisplayChar(x + 48, y, TimeString[4] + 0x30);
        OLED_DisplayChar(x + 56, y, TimeString[5] + 0x30);
    }
}

/// @brief 此函数为各种页面的项目提供一个可移动的选择指示箭头→，属于阻塞语句
/// @param item 可选项的数量,2-4
/// @return 用户选择的选项序号,返回0表示退出
unsigned char Show_SelectBar(unsigned char item)
{
    unsigned char i = 1;

    while (1)
    {
        OLED_DisplayCN(0, 0, 19); // 先清除所有的箭头显示
        OLED_DisplayCN(0, 2, 19);
        OLED_DisplayCN(0, 4, 19);
        OLED_DisplayCN(0, 6, 19);

        OLED_DisplayCN(0, i * 2 - 2, 18); // 在当前用户选择的位置显示箭头

        switch (Key_Scan_Blocked()) // 判断按下什么按键
        {
        case 1: // 按下ok，则返回当前的所选项
            return i;
            break;
        case 2: // 按下切换，则当前选项+1，也就是下移一位
            i++;
            break;
        case 3: // 按下取消，则返回的选项是0，表示返回。
            return 0;
            break;
        }
        if (i > item)
            i = 1; // 当项目数到最大，则归1
    }
}

/// @brief 打开或关闭闹钟响铃
/// @param statue 0为关，1为开
void AlarmToggle(unsigned char statue)
{
    Alarm_Pin = 1 - statue;
}