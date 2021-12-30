#include "reg51.h"
#include "LCD1602.h"
#include "DS1302.h"
#define uchar unsigned char
#define uint unsigned int
bit key_flag1=0, key_flag2=0;
SYSTEMTIME adjusted;

#define KEY_MATRIX_PORT	P1	//使用宏定义矩阵按键控制口		


uchar sec_add=0, min_add=0, hou_add=0, day_add=0, mon_add=0, yea_add=0;
uchar data_alarm[7]={0};
uchar test[6] = "     C";

int key_scan()
{
	int i=0;
	uint temp;
	P1=0xF0;
	temp=P1;
	if(temp!=0xF0)
	{
		i=1;
		test[5]='C';
	}
	else
	{
		i=0;
	}	

	return i;
}

uchar key_value()
{
	uint m=0, n=0, temp;
	uchar value;
	uchar v[3][4]={'0','1','2','6','3','4','5','7','8','9','A','B'};
	P1=0xfe;temp=P1; if(temp!=0xfe) m=3; 
	P1=0xfd;temp=P1; if(temp!=0xfd) m=2;
	P1=0xfb;temp=P1; if(temp!=0xfb) m=1;
	P1=0xf7;temp=P1; if(temp!=0xf7) m=0;
	P1=0x7f;temp=P1; if(temp!=0x7f) n=0;
	P1=0xbf;temp=P1; if(temp!=0xbf) n=1;
	P1=0xdf;temp=P1; if(temp!=0xdf) n=2;
	value=v[n][m];
	return value;
}

void adjust(void)
{
	/*if(key_scan()&&key_flag1)
	{
		switch(key_value())
		{	  
			case '5':sec_add++;test[5]='S';break;
			case '4':min_add++;test[5]='M';break;
			case '3':hou_add++;test[5]='H';break;
			case '2':day_add++;test[5]='D';break;
			case '1':mon_add++;test[5]='M';break;
			case '0':yea_add++;test[5]='Y';break;
			//case '7':Save_Time(&adjusted);break; 
			default:test[5]='C'; break;
		}
	}
	
	adjusted.Second+=sec_add;
	adjusted.Minute+=min_add;
	adjusted.Hour+=hou_add;
	adjusted.Day+=day_add;
	adjusted.Month+=mon_add;
	adjusted.Year+=yea_add;*/
	
	if(key_scan()&&key_flag1)
	{
		switch(key_value())
		{	  
			case '5':adjusted.Second++;test[5]='S';break;
			case '4':adjusted.Minute++;test[5]='M';break;
			case '3':adjusted.Hour++;test[5]='H';break;
			case '2':adjusted.Day++;test[5]='D';break;
			case '1':adjusted.Month++;test[5]='M';break;
			case '0':adjusted.Year++;test[5]='Y';break;
			//case '7':Save_Time(&adjusted);break; 
			default:test[5]='C'; break;
		}
		
	
		
		if(adjusted.Second>59)
		{
			adjusted.Second=adjusted.Second%60;
			adjusted.Minute++;
		}
		if(adjusted.Minute>59)
		{
			adjusted.Minute=adjusted.Minute%60;
	
	
			adjusted.Hour++;
		}
		if(adjusted.Hour>23)
		{
			adjusted.Hour=adjusted.Hour%24;
			adjusted.Day++;
		}
		if(adjusted.Day>31)
		{
			adjusted.Day=adjusted.Day%31;
			adjusted.Month++;
		}
		if(adjusted.Month>12)
		{
			adjusted.Month=adjusted.Month%12;
			adjusted.Year++;
			//test[5]='O';
		}	
		if(adjusted.Year>100)
		{
			adjusted.Year=adjusted.Year%100;
		}
		Save_Time(&adjusted);
	}
}

void changing(void) interrupt 0 using 0
{
	if(key_flag1)
	{			  
		key_flag1=0;
	}
	else
	{
		key_flag1=1;
	}
}

void main()
{
	uint i;
	uchar p1[]="D:", p2[]="T:";
	SYSTEMTIME T;

	EA=1;
	EX0=1;
	IT0=1;
	EA=1;
	EX1=1;
	IT1=1;
	init1602();
	Initial_DS1302();

	while(1)
	{
		write_com(0x80);
		write_string(p1, 2);
		write_com(0xc0);
		write_string(p2, 2);
		if(key_flag1==0)
		{
			DS1302_GetTime(&T);
			adjusted.Second=T.Second;
			adjusted.Minute=T.Minute;
			adjusted.Hour=T.Hour;
			adjusted.Day=T.Day;
			adjusted.Month=T.Month;
			adjusted.Year=T.Year;
			for(i=0; i<9; i++)
			{
				adjusted.DateString[i] = T.DateString[i];
				adjusted.TimeString[i] = T.TimeString[i];
			}
		}
		else
			adjust();
		DateToStr(&adjusted);
		TimeToStr(&adjusted);
		write_com(0x82);
		write_string(adjusted.DateString, 8);
		if(key_flag1)write_string(test, 6);
		else write_string("      ", 6);
		write_com(0xc2);			  
		write_string(adjusted.TimeString, 8);
		if(key_flag1)write_string(test, 6);
		else write_string("      ", 6);
		delay(10);
	}
}