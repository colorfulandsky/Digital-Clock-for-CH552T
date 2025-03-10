#include "ds1302.h"

// 引脚定义；
sbit DS1302_SCLK = P1 ^ 4;
sbit DS1302_IO = P1 ^ 5;
sbit DS1302_RST = P1 ^ 6;

// 对应寄存器位置定义
#define DS1302_SECOND 0X80
#define DS1302_MINUTE 0X82
#define DS1302_HOUR 0X84
#define DS1302_DATE 0X86
#define DS1302_MONTH 0X88
#define DS1302_DAY 0X8A
#define DS1302_YEAR 0X8C
#define DS1302_WP 0X8E


/*数据（全局变量 数组）存放的方式：
例如：日期表示2024年12月30日
位数：	0	1	2	3	4	5	6	7
数据：	2	0	2	4	1	2	3	0	

时间表示：12时30分15秒
位数：	0	1	2	3	4	5
数据：	1	2	3	0	1	5

*/

unsigned char DS1302_Time[] = {1,2,3,0,0,0};	//默认时间
unsigned char DS1302_Date[] = {2,0,2,4,0,1,0,1};	//默认日期
unsigned char DS1302_Week = 1;	//默认星期一

/**
 *@brief DS1302初始化,同时设置为默认的时间/日期/星期
 *@param 无
 *@retval 无
 */
void DS1302_Init()
{
	DS1302_RST = 0;	 // 将使能位置0，低电平；
	DS1302_SCLK = 0; // 将时钟位置0，低电平；
}

/**
 *@breaf DS1302单字节写入函数
 *@param command：写入控制指令的指令，包含要写入寄存器的地址,8bits；
 *@param Data：将要写入的数据内容 8bits；
 *@retval 无
 */
void DS1302_WriteByte(unsigned char command, Data)
{
	unsigned char i;
	DS1302_RST = 1;			// 使能位置高电平；
	for (i = 0; i < 8; i++) // 控制寄存器数据需要通过IO线一个一个写入控制寄存器;低位先写入
	{
		DS1302_IO = command & (0x01 << i); // 相当于把第1--7位置0，只留第0位，如果第0位是0，则为0；反之则为1；
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}

	for (i = 0; i < 8; i++) // 数据写入
	{
		DS1302_IO = Data & (0x01 << i); // 相当于把第1--7位置0，只留第0位，如果第0位是0，则为0；反之则为1；
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
	}
	DS1302_RST = 0;
}

/**
 *@breaf  DS1302单字节读出函数
 *@param command：写入控制指令的指令，包含要读出寄存器的地址；
 *@retval Data：读出的数据；
 */
unsigned char DS1302_ReadByte(unsigned char command)
{
	unsigned i, Data = 0X00;
	command |= 0X01;		// 写入指令与读出指令只在最后一位相差1，故在此利用或运算消除；
	DS1302_RST = 1;			// 使能位置高电平；
	for (i = 0; i < 8; i++) // 控制寄存器数据需要通过IO线一个一个写入控制寄存器;低位先写入
	{
		DS1302_IO = command & (0x01 << i); // 相当于把第1--7位置0，只留第0位，如果第0位是0，则为0；反之则为1；
		DS1302_SCLK = 0;
		DS1302_SCLK = 1;
	}
	
	for (i = 0; i < 8; i++) // 数据读出
	{
		DS1302_SCLK = 1;
		DS1302_SCLK = 0;
		if (DS1302_IO)
		{
			Data |= (0X01 << i);
		}
	}
	DS1302_IO = 0;
	DS1302_RST = 0;
	return Data;
}


/**
 *@breaf ds1302设置时间
 *@param 无
 *@retval 无
 */
void DS1302_SetTime()
{
	DS1302_WriteByte(DS1302_WP, 0x00);										// 操作 DS1302 之前，关闭写保护，不然指令无法进入控制寄存器；
	DS1302_WriteByte(DS1302_HOUR, (DS1302_Time[0]<<4) | DS1302_Time[1]);	 // 写入时，
	DS1302_WriteByte(DS1302_MINUTE, (DS1302_Time[2]<<4) | DS1302_Time[3]); 	// 写入分，
	DS1302_WriteByte(DS1302_SECOND, (DS1302_Time[4]<<4) | DS1302_Time[5]);	// 写入秒，
	DS1302_WriteByte(DS1302_WP, 0x80);												 // 写入结束，开启写保护；
}

//ds1302设置日期程序
void DS1302_SetDate()
{
	DS1302_WriteByte(DS1302_WP, 0x00);										// 操作 DS1302 之前，关闭写保护，不然指令无法进入控制寄存器；
	DS1302_WriteByte(DS1302_YEAR, (DS1302_Date[2]<<4) | DS1302_Date[3]);	 // 写入年，直接将年份十位左移4位，再和个位并起来；下同
	DS1302_WriteByte(DS1302_MONTH, (DS1302_Date[4]<<4) | DS1302_Date[5]);	 // 写入月，
	DS1302_WriteByte(DS1302_DATE, (DS1302_Date[6]<<4) | DS1302_Date[7]);	 // 写入日，
	DS1302_WriteByte(DS1302_WP, 0x80);												 // 写入结束，开启写保护；
}

//ds1302设置星期
void DS1302_SetWeek()
{
	DS1302_WriteByte(DS1302_WP, 0x00);										// 操作 DS1302 之前，关闭写保护，不然指令无法进入控制寄存器
	DS1302_WriteByte(DS1302_DAY, DS1302_Week);	 							// 写入星期，
	DS1302_WriteByte(DS1302_WP, 0x80);												 // 写入结束，开启写保护；
}

/**
 *@brief 读取DS1302内时间，读取到的时间存入全局变量DS1302_Time[0]中
 *@param None
 *@retval None
 */
void DS1302_ReadTime()
{
	unsigned char Temp;							 // 定义变量，用于暂时存储BCD码

	Temp = DS1302_ReadByte(DS1302_YEAR);		 // 读取年BCD码；
	DS1302_Date[2] = (Temp >> 4) & 0x0f;		//年份十位，取后四位（先右移4位，高位变低位；再和00001111与一下）
	DS1302_Date[3] = Temp & 0x0f;				// 年份个位，取前四位bcd（直接与00001111与）下同
	
	Temp = DS1302_ReadByte(DS1302_MONTH);		 // 读取月BCD码；
	DS1302_Date[4] = (Temp >> 4) & 0x0f;
	DS1302_Date[5] = Temp & 0x0f;

	Temp = DS1302_ReadByte(DS1302_DATE);		 // 读取日BCD码；
	DS1302_Date[6] = (Temp >> 4) & 0x0f;
	DS1302_Date[7] = Temp & 0x0f;

	Temp = DS1302_ReadByte(DS1302_HOUR);		 // 读取小时BCD码；
	DS1302_Time[0] = (Temp >> 4) & 0x0f;
	DS1302_Time[1] = Temp & 0x0f;

	Temp = DS1302_ReadByte(DS1302_MINUTE);		 // 读取分钟BCD码；
	DS1302_Time[2] = (Temp >> 4) & 0x0f;
	DS1302_Time[3] = Temp & 0x0f;

	Temp = DS1302_ReadByte(DS1302_SECOND);		 // 读取秒BCD码；
	DS1302_Time[4] = (Temp >> 4) & 0x0f;
	DS1302_Time[5] = Temp & 0x0f;

	Temp = DS1302_ReadByte(DS1302_DAY);			 // 读取星期BCD码；
	DS1302_Week=Temp;
}