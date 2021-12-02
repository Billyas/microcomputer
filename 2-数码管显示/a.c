#include "reg51.h"
//#include "intrins.h"
#define uchar unsigned char
#define uint unsigned int
#define out P0
//uchar code seg [] = {0xc0, 0xa4, 0x99, 0x82, 0x80, 0xf9, 0xb0, 0x92, 0xf8, 0x90, 0x01};
uchar code seg [] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void delayms(uint);

void main(void)
{
	uchar i;
	while(1){
		out = seg[i];
		delayms(900);
		i++;
		if(i == 10)
			i = 0;
	}
}

void delayms(uint j)
{								      
	uchar i=0;
	for (;j>0;j--)
	{
		i = 250;
		while(--i);
		i = 249;
		while(--i);
	}
}