#include "reg51.h"
#include "intrins.h"

#define uchar unsigned char
#define uint unsigned int
#define out P0

sbit smg1=P2^2;
sbit smg2=P2^3;
sbit smg3=P2^4;
sbit DQ=P3^7;
sbit BEEP=P1^5;

uchar code seg[]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

void delay5(uchar);
void init_ds18b20(void);
uchar readbyte(void);
void writebyte(uchar);
uchar retemp(void);
void checktemp(uint);

void main(void)
{
	uchar i, temp;
	delay5(1000);
	while(1)
	{
		temp = retemp();
		for(i=0; i<10; i++)
		{
			out = seg[(temp/10)];
			smg1 = 1;
			smg2 = 0;	
			smg3 = 0;
			if(temp<25)
				delay5(1000);
			else
				checktemp(temp);
			out = seg[(temp%10)];	 
			smg1 = 0;
			smg2 = 0;  
			smg3 = 0; 
			//delay5(1000);
			checktemp(temp);
		}
	}
}

void delay5(uchar n)	 //延时5微秒
{
	do
	{
		_nop_();
		_nop_();
		_nop_();
		n--;
	}
	while(n);
}

void init_ds18b20(void) //对18B20初始化函数
{
	uchar x = 0;
	DQ = 0;
	delay5(120);
	DQ = 1;
	delay5(16);
	delay5(80);
}

uchar readbyte(void) //函数功能：读取1字节数据
{
	uchar i = 0;
	uchar date = 0;
	for(i=8; i>0; i--)
	{
		DQ = 0;
		delay5(1);		
		DQ = 1;
		date >>= 1;
		if(DQ)
			date |= 0x80;
		delay5(11);	
	}
	return(date);
}

void writebyte(uchar dat) //写1B函数
{
	uchar i = 0;
	for(i=8; i>0; i--)
	{
		DQ = 0;
		DQ = dat&0x01;	  //写“1” 在15微秒内拉低
		delay5(12);	      //写“0” 拉低60微秒
		DQ = 1;
		dat >>= 1;
		delay5(5);
	}
}

uchar retemp(void) //读取温度函数
{
	uchar a, b, tt;
	uint t;
	init_ds18b20();
	writebyte(0xCC);
	writebyte(0x44);
	init_ds18b20();
	writebyte(0xCC);
	writebyte(0xBE);
	a = readbyte();
	b = readbyte();
	t = b;
	t <<= 8;
	t = t | a;
	tt = t * 0.0625;
	return(tt);
}

void checktemp(uint t)
{
	uint i = 10;
	if(t>=25)
	{
		while(i--)//循环2000次
		{
			BEEP=!BEEP;//产生一定频率的脉冲信号
			delay5(100);
		}
		i=0;//清零
		BEEP=0;//关闭蜂鸣器
	}
	else
	{
		BEEP = 0;
	}
}