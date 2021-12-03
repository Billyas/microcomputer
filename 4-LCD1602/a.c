#include"reg51.h"
#define uchar unsigned char
#define uint unsigned int
uchar p[]="i love you";
sbit E=P2^7;//ʹ��E����������ʽ�����ź�
sbit RS=P2^6;//0��ָ��      1���ַ�
sbit RW=P2^5;//0��д        1����

void dy(uint x)
{
	uint i;
	i=x*100;
	while(i--);	
}

void LCD_1602_Cmd(uchar cmd)//дָ��
{
	RS=0;	 //ʱ��ΪRS��RW��E
	RW=0;
	P0=cmd;
	E=1;		 //һ�����塢һ��ʹ���ź�
	dy(2);
	E=0;
}

void LCD_1602_Data(uchar dat)//д�ַ�
{
	RS=1;
	RW=0;
	P0=dat;
	E=1;
	dy(2);
	E=0;
}
void LCD_1602_init()//��ʼ��
{
	LCD_1602_Cmd(0X38);//����ʾ��8λһ�С�5x7��ʾ����	
	LCD_1602_Cmd(0X06);//ÿд��һ���ַ���ָ���һ�����������һλ
	LCD_1602_Cmd(0X0c);//����ʾ������ʾ���
	LCD_1602_Cmd(0X01);//����
	//LCD_1602_Cmd(0X18)��������ʾ���ַ�������һλ��
	//LCD_1602_Cmd(0X80)����������ָ��������LCD_1602_Cmd(k2+0x80)��
}

void LCD_1602_Display(uchar x,uchar y,uchar dat)//��x�С�y�н�����ʾ�ַ�dat
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

void display(uint l,uchar *p)//��ʾ�ַ���
{
	uint i;
	for(i=0;i<l;i++)
		LCD_1602_Data(p[i]);
}

void main()
{
	uchar i;
	LCD_1602_init();
	//LCD_1602_Display(2,0,6+48);//��ʾ����ʱ��48����Ϊ0��ascii��Ϊ48
	for(i=0;i<sizeof(p)-1;i++)
	{
		LCD_1602_Display(1,i,p[i]);
	}
	LCD_1602_Cmd(0X80+0X40);	 
	display(sizeof(p)-1,p);
}
