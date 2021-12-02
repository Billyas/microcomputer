#include "reg51.h"
#include "intrins.h"
#define uchar	unsigned char
#define uint	unsigned int
#define out		P0
sbit 	smg1 = out^4;
sbit 	smg2 = out^5;
sbit 	DQ   = P3^7;
uchar a[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void delay5(uchar);
void init_ds18b20(void);
uchar readbyte(void);
void writebyte(uchar);
uchar retemp(void);

void main(void)
{
	uchar i, temp;
	delay5(1000);
	while(1)
	{
		temp = retemp();
		for(i=0; i<10; i++)
		{
			//out = (temp/10)&0x0f;
			out = a[(temp/10)&0x0f];
			smg1 = 0;
			smg2 = 1;
			delay5(1000);
			//out = (temp%10)&0x0f;
			out = a[(temp%10)&0x0f];
			smg1 = 1;
			smg2 = 0;
			delay5(1000);
		}
	}
	
}

void delay5(uchar n)  //5us
{
	do{
		_nop_();
		_nop_();
		_nop_();
		n--;
	}while(n);
}

void init_ds18b20(void)
{
	uchar x = 0; 
	DQ = 0;
	delay5(120);
	DQ = 1;
	delay5(16);
	delay5(80);
}

uchar readbyte(void)
{
	uchar i = 0;
	uchar date = 0;
	for(i = 8; i > 0; i--)
	{
		DQ = 0;
		delay5(1);
		DQ = 1;
		date >>= 1;
		if(DQ)  date |= 0x80;
		delay5(11);
	}
	return (date);
}

void writebyte(uchar dat)
{
	uchar i=0;
	for(i=8; i>0; i--)
	{
		DQ = 0;
		DQ = dat&0x01;
		delay5(12);
		DQ = 1;
		dat >>=1;
		delay5(5);
	}
}

uchar retemp(void)
{
	uchar a,b,tt;
	uint t;
	init_ds18b20();
	writebyte(0xCC);  //跳过读取序列，即直接单个设备
	writebyte(0x44);  //启动温度转换
	init_ds18b20();
	writebyte(0xCC);
	writebyte(0xBE);  //读取温度
	a = readbyte();
	b = readbyte();
	t = b;
	t <<= 8;
	t = t|a;
	tt = t*0.0625;
	return (tt); 
}