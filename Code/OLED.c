
#include "OLED.h"
#include "OLEDFont.h"
#include "ds1302.h"

/***OLED12864/0.96'/SSD1306***/

/**
 * 51延时函数 @24MHz频率下
 * @param n 最大为65535，单位毫秒
 * @return: void
 */
void Delay_ms(unsigned int n)
{
    unsigned int i=0,j=0;
    for(i=0;i<n;i++)
        for(j=0;j<357;j++); 
}

/**
 * @function: void OLED_Write_Byte(uchar WCommand,uchar cmdORdata)
 * @description: 
 * @param {uchar WData待写入的内容，uchar cmdORdata指明写入指令还是数据} 
 * @return: void
 */
void OLED_Write_Byte(uchar WData, uchar cmdORdata)
{
    I2C_Start();
    I2C_Write_Byte(OLED_Addr + 0); //写入器件地址(bit0=1读，bit0=0写)
    I2C_Wait_Ack();
    I2C_Write_Byte(cmdORdata); //指明写入指令还是数据
    I2C_Wait_Ack();
    I2C_Write_Byte(WData); //写入数据内容
    I2C_Wait_Ack();
    I2C_Stop();
}


/// @brief 全屏显示相同的数据：用于清屏、亮屏
/// @param Fill_Data 清屏：0x00  全亮屏：0xff
void OLED_Fill(uchar Fill_Data)
{
    uchar i, j;
    for (i = 0; i < 8; i++) //8页，共需要循环显示8次
    {
        OLED_Write_Byte(0xb0 + i, isCmd); //页地址0~7
        OLED_Write_Byte(0x10, isCmd);     //设置高位列地址
        OLED_Write_Byte(0x00, isCmd);     //设置低位列地址
        for (j = 0; j < OLED_Width; j++)  //每页有128列需要显示
            OLED_Write_Byte(Fill_Data, isData);
    }
}


/// @brief 设置起始显示位置
/// @param x 起始列，取值范围0-127
/// @param y 起始行，取值范围0-7
void OLED_Set_Pos(uchar x, uchar y)
{
    OLED_Write_Byte(0xb0 + y, isCmd);               //起始显示的页
    OLED_Write_Byte((x & 0xf0) >> 4 | 0x10, isCmd); //设置高位列地址
    OLED_Write_Byte(x & 0x0f, isCmd);               //设置低位列地址
}

/**
 * @function: void OLED_Init()
 * @description: OLED初始化函数
 */
void OLED_Init()
{
    Delay_ms(200);
    OLED_Write_Byte(0xae, isCmd); //关闭显示器 display off
    OLED_Write_Byte(0x00, isCmd); //设置低位列地址 set low column address
    OLED_Write_Byte(0x10, isCmd); //设置高位列地址 set high column address
    OLED_Write_Byte(0x40, isCmd); //设置起始行地址 set start line address
    OLED_Write_Byte(0xb0, isCmd); //设置页地址 set page address
	
    OLED_Write_Byte(0x81, isCmd); //设置对比度
    OLED_Write_Byte(0x10, isCmd); //对比度,数值越大屏幕越亮，0x00-0xff
	
    OLED_Write_Byte(0xa1, isCmd); //设置段重新映射 不左右翻转set segment remap
    OLED_Write_Byte(0xa6, isCmd); //设置正常/反向 normal / reverse
    OLED_Write_Byte(0xa8, isCmd); //设置多路复用比(1-64) set multiplex ratio(1 to 64)
    OLED_Write_Byte(0x3f, isCmd); //设定值1/32  1/32 duty
    OLED_Write_Byte(0xc8, isCmd); //扫描方向 不上下翻转Com scan direction

    OLED_Write_Byte(0xd3, isCmd); //设置显示偏移 set display offset
    OLED_Write_Byte(0x00, isCmd); //

    OLED_Write_Byte(0xd5, isCmd); //设置osc分区 set osc division
    OLED_Write_Byte(0x80, isCmd); //

    OLED_Write_Byte(0xd8, isCmd); //关闭区域颜色模式 set area color mode off
    OLED_Write_Byte(0x05, isCmd); //

    OLED_Write_Byte(0xd9, isCmd); //设置预充电期 Set Pre-Charge Period
    OLED_Write_Byte(0xf1, isCmd); //

    OLED_Write_Byte(0xda, isCmd); //设置com引脚配置 set com pin configuartion
    OLED_Write_Byte(0x12, isCmd); //

    OLED_Write_Byte(0xdb, isCmd); //设置vcomh set Vcomh
    OLED_Write_Byte(0x30, isCmd); //

    OLED_Write_Byte(0x8d, isCmd); //设置电源泵启用 set charge pump enable
    OLED_Write_Byte(0x14, isCmd); //

    OLED_Write_Byte(0xa4, isCmd); //设置全局显示开启；bit0，1开启，0关闭(白屏/黑屏)
    OLED_Write_Byte(0xa6, isCmd); //设置显示方式，bit0，1反相显示，0正常显示

    OLED_Write_Byte(0xaf, isCmd); //打开oled面板 turn on oled panel

    OLED_Fill(0x00); //清屏
}


/// @brief 在oled特定位置显示一个字符
/// @param x 取值0-127
/// @param y 取值0-7
/// @param DisplayChar 需要显示的字符（用’x‘表示，或直接传递ascii码）
void OLED_DisplayChar(uchar x, uchar y, uchar DisplayChar)
{
    uchar c = 0, i;
    c = DisplayChar - 32; //等同于减去个空格c = DisplayChar - ' '; 得到显示字符在库中的偏移地址
    
        OLED_Set_Pos(x, y);                                 //开始显示位置
        for (i = 0; i < 8; i++)                             //显示一个8X16字符的上半部分
            OLED_Write_Byte(F8X16[c * 16 + i], isData);     //循环显示数组中前8个数据
        OLED_Set_Pos(x, y + 1);                             //换到下一页，显示字符下半部分
        for (i = 0; i < 8; i++)                             //显示8X16字符的下半部分
            OLED_Write_Byte(F8X16[c * 16 + i + 8], isData); //循环显示数组中后8个数据

}


/// @brief 显示大字符，尺寸为16x32
/// @param x 显示横坐标，取值范围为0-127
/// @param y 显示纵坐标，取值范围为0-7
/// @param no 自建符号库（F16X32）中符号相应的序号
/// @param FontStyle 需要显示字符的风格字体，0：黑体 1：数码时钟体 2：衬线体
void OLED_ShowLargeChar(unsigned char x, unsigned char y, unsigned char no, unsigned char FontStyle)
{
    unsigned char t,i;
    for(i=0;i<4;i++)    //字符高度，页扫描，16x32,32/8=4页
    {
        OLED_Set_Pos(x,y+i);
        for(t=0;t<16;t++)    //写内容，字符宽度16
        {
            if(FontStyle==0)   OLED_Write_Byte(Bold_F16X32[4*no+i][t],isData); 
            if(FontStyle==1)   OLED_Write_Byte(Digital_Clock_F16x32[4*no+i][t],isData);
            //if(FontStyle==1)   OLED_Write_Byte(Serif_F16x32[4*no+i][t],isData);
        }
    }
}



/// @brief 在OLED12864特定位置显示字符串
/// @param x 列:0-128
/// @param y 行：0-7
/// @param DisplayStr 需要显示的字符串
void OLED_DisplayStr(uchar x, uchar y, uchar *DisplayStr) //(uchar *DisplayStr)改成(uchar DisplayStr[])也可
{
    uchar j = 0;
    while (DisplayStr[j] !=  '\0') //判断字符串是否显示完成
    {
        OLED_DisplayChar(x, y, DisplayStr[j]);
        x += 8;
        if (x > 120 ) //如果一行不够显示了，从下一行继续显示
        {
            x = 0;
            y++;
        }
        j++;
    }
}


/// @brief 在OLED12864特定位置显示16X16汉字
/// @param x 0-127
/// @param y 0-7
/// @param Num 汉字库中汉字编号
void OLED_DisplayCN(uchar x, uchar y, uchar Num)
{
    uchar i;
    OLED_Set_Pos(x, y);
    for (i = 0; i < 16; i++)
        OLED_Write_Byte(FontCN[2 * Num][i], isData); //显示汉字的上半部分
    OLED_Set_Pos(x, y + 1);
    for (i = 0; i < 16; i++)
        OLED_Write_Byte(FontCN[2 * Num + 1][i], isData); //显示汉字的下半部分
}