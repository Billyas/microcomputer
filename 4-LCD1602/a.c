#include"reg51.h"
#define uchar unsigned char
#define uint unsigned int
uchar p[]="i love you";
sbit E=P2^7;//使能E，以脉冲形式发送信号
sbit RS=P2^6;//0：指令      1：字符
sbit RW=P2^5;//0：写        1：读

void dy(uint x)
{
	uint i;
	i=x*100;
	while(i--);	
}

void LCD_1602_Cmd(uchar cmd)//写指令
{
	RS=0;	 //时序为RS、RW、E
	RW=0;
	P0=cmd;
	E=1;		 //一次脉冲、一次使能信号
	dy(2);
	E=0;
}

void LCD_1602_Data(uchar dat)//写字符
{
	RS=1;
	RW=0;
	P0=dat;
	E=1;
	dy(2);
	E=0;
}
void LCD_1602_init()//初始化
{
	LCD_1602_Cmd(0X38);//开显示：8位一行、5x7显示点阵	
	LCD_1602_Cmd(0X06);//每写入一个字符后指针加一，即光标右移一位
	LCD_1602_Cmd(0X0c);//开显示但不显示光标
	LCD_1602_Cmd(0X01);//清屏
	//LCD_1602_Cmd(0X18)：所有显示的字符向左移一位，
	//LCD_1602_Cmd(0X80)：设置数据指针的起点如LCD_1602_Cmd(k2+0x80)。
}

void LCD_1602_Display(uchar x,uchar y,uchar dat)//在x行、y列进行显示字符dat
{
	uchar k1=0x00,k2=0x40;
	if(x==1)
	{
		k1+=y;
		LCD_1602_Cmd(k1+0x80);
	}
	else
	{
		k2+=y;
		LCD_1602_Cmd(k2+0x80);	
	}
	LCD_1602_Data(dat);
}

void display(uint l,uchar *p)//显示字符串
{
	uint i;
	for(i=0;i<l;i++)
		LCD_1602_Data(p[i]);
}

void main()
{
	uchar i;
	LCD_1602_init();
	//LCD_1602_Display(2,0,6+48);//显示数字时加48，因为0的ascii码为48
	for(i=0;i<sizeof(p)-1;i++)
	{
		LCD_1602_Display(1,i,p[i]);
	}
	LCD_1602_Cmd(0X80+0X40);	 
	display(sizeof(p)-1,p);
}
