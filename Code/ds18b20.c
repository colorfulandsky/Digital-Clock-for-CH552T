#include <reg52.h>
#include <intrins.h>
#define MAIN_Fosc		11059200UL	//宏定义主时钟HZ

/*====================================
 硬件接口位声明
====================================*/
sbit DS  = P2^2;   //DS18B20单总线


 
/*====================================
函数：void DS18B20_DelayMs(unsigned int ms)
参数：ms，毫秒延时形参
描述：12T 51单片机自适应主时钟毫秒级延时函数
====================================*/
void DS18B20_DelayMs(unsigned int ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 96000; 
		  while(--i);   //96T per loop
     }while(--ms);
}
/*us延时函数，执行一次US--所需6.5us进入一次函数需要11.95us*/
void DS18B20_DelayUs(unsigned char us)
{
	while(us--);	
}


/*单总线初始化时序*/
bit ds_init()
{
	bit i;
	DS = 1;
	_nop_();
	DS = 0;
	DS18B20_DelayUs(75); //拉低总线499.45us 挂接在总线上的18B20将会全部被复位
	DS = 1; //释放总线
	DS18B20_DelayUs(4); //延时37.95us 等待18B20发回存在信号
	i = DS;
	DS18B20_DelayUs(20); //141.95us
	DS = 1;
	_nop_();
	return (i);
}


/*写一个字节*/
void write_byte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DS = 0;
		_nop_();//产生些时序
		DS = dat & 0x01;
		DS18B20_DelayUs(10);//76.95us
		DS = 1; //释放总线准备下一次数据写入
		_nop_();
		dat >>= 1;
	}
}
 


unsigned char read_byte()
{
	unsigned char i, j, dat;
	for(i=0;i<8;i++)
	{
		DS = 0;
		_nop_();//产生读时序
		DS = 1;
		_nop_();//释放总线
		j = DS;
		DS18B20_DelayUs(10);//76.95us
		DS = 1;
		_nop_();
		dat = (j<<7)|(dat>>1);	
	}
	return (dat);
}



void DS18B20_StartMeasure()
{
	unsigned int i;
	unsigned char L, M;
	while(1)
	{
		ds_init();//初始化DS18B20
		write_byte(0xcc);//发送跳跃ROM指令
		write_byte(0x44);//发送温度转换指令
		ds_init();//初始化DS18B20
		write_byte(0xcc);//发送跳跃ROM指令
		write_byte(0xbe);//读取DS18B20暂存器值
		L = read_byte();
		M = read_byte();
		i = M;
		i <<= 8;
		i |= L;						
		i = i * 0.0625 * 10 + 0.5;
		
	}
}