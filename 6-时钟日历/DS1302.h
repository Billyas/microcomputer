#ifndef TIMER_DS1302
#define TIMER_DS1302
sbit DS1302_CLK = P3^6;//实时时钟时钟线引脚
sbit DS1302_IO = P3^4;//实时时钟数据线引脚
sbit DS1302_RST = P3^5;//实时时钟复位线引脚
sbit ACC0 = ACC^0;//定义ACC的最低位和最高位,在对ACC移位操作后,用于传输数据
sbit ACC7 = ACC^7;
typedef struct SYSTEM_TIME
{
unsigned char Second;
unsigned char Minute;
unsigned char Hour;
unsigned char Week;
unsigned char Day;
unsigned char Month;
unsigned char Year;
unsigned char DateString [9];//用这两个字符串来放置读取的时间
unsigned char TimeString[9];
}SYSTEMTIME; //定义的时间类型结构体
#define AM(X) X
#define PM(X) (X+12) //转成24小时制
#define DS1302_SECOND 0x80 //片内各位数据的地址
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 0x84
#define DS1302_WEEK 0x8A
#define DS1302_DAY 0x86
#define DS1302_MONTH 0x88
#define DS1302_YEAR 0x8C
#define DS1302_RAM(X)(0xC0+(X)*2)
//用于计算 DS1302_RAM 地址的宏
/******内部指令***★★***★*吉*★*/
void DS1302InputByte(unsigned char d)	//实时时钟写入1B(内部函数)
{
	unsigned char i;
	ACC=d;
	for(i=8;i>0;i--)
	{
		DS1302_IO=ACC0;
		DS1302_CLK=1; //写数据在上升沿,且先写低位再写高位
		DS1302_CLK=0; //因为在前面已定义ACCO=ACC^0;以便再次利用
		ACC=ACC>>1;
	}
}
//函数功能:实时时钟读取1B(内部函数)
unsigned char DS1302OutputByte (void)
{
	unsigned char i;
	for(i=8; i>0; i--)
	{
		ACC=ACC >>1;
		//相当于汇编中的 RRC
		ACC7=DS1302_IO;
		//由低位到高位传播AcC7中的信息
		DS1302_CLK=1;
		//读信息是在下降沿
		DS1302_CLK=0;
	}		  
	return(ACC);
}
void Write1302(unsigned char ucAddr, unsigned char ucDa) //ucAddr: DS1302地址,
{
	//ucData:要写的数据
	DS1302_RST =0;
	DS1302_CLK =0;
	DS1302_RST =1;
	DS1302InputByte(ucAddr) ;
	//地址,命令
	DS1302InputByte(ucDa);
	//写1B数据
	DS1302_CLK =1;
	DS1302_RST =0;
}
unsigned char Read1302(unsigned char ucAddr) //读取DS1302某地址的数据
{
	unsigned char ucData;
	DS1302_RST=0;
	DS1302_CLK = 0;
	DS1302_RST =1;
	DS1302InputByte(ucAddr|0x01);
	//上升沿,写地址,命令
	ucData = DS1302OutputByte();
	//下降沿,读1B数据
	DS1302_CLK =1;
	DS1302_RST =0;
	return(ucData) ;
	//在上升沿之后进行写操作,在下降沿之前进行读操作
}
void DS1302_SetProtect(bit flag) //!是否写保护
{
	if(flag)
		Write1302(0X8E,0x80);
	else
		Write1302(0X8E,0x00);
}

void DS1302_SetTime (unsigned char Address,unsigned char Value)//函数功能:设置时间
{
	DS1302_SetProtect(0);
	Write1302(Address, ((Value/10)<<4 | (Value%10)));//将十进制数转换为BCD码
}
//在 DS1302中的与日历、时钟相关的寄存器存放的数据必须为BCD码形式
void DS1302_GetTime (SYSTEMTIME *Time)
{
	unsigned char ReadValue;
	ReadValue = Read1302(DS1302_SECOND);
	Time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//将BCD码转换为十进制数,此处为结构体操作
	ReadValue = Read1302(DS1302_MINUTE);
	Time->Minute =((ReadValue&0x70)>>4)*10 +(ReadValue&0x0F);
	ReadValue = Read1302(DS1302_HOUR);
	Time->Hour =((ReadValue&0x70)>>4)*10+(ReadValue&0x0F);
	ReadValue = Read1302(DS1302_DAY);
	Time->Day =((ReadValue&0x70)>>4)*10 +(ReadValue&0x0F);
	ReadValue = Read1302(DS1302_WEEK);
	Time->Week =((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_MONTH);
	Time->Month = ((ReadValue&0x70)>>4)*10 +(ReadValue&0x0F);
	ReadValue = Read1302(DS1302_YEAR);
	Time->Year = ((ReadValue&0x70)>>4)*10 +(ReadValue&0x0F);
}

/*unsigned char *DataToBCD(SYSTEMTIME *Time)
{
	unsigned char D[8];
	D[0]=Time->Second/10<<4+Time->Second%10;
	D[1]=Time->Minute/10<<4+Time->Minute%10;
	D[2]=Time->Hour/10<<4+Time->Hour%10;
	D[3]=Time->Day/10<<4+Time->Day%10;
	D[4]=Time->Month/10<<4+Time->Month%10;
	D[5]=Time->Week/10<<4+Time->Week%10;
	D[6]=Time->Year/10<<4+Time->Year%10;
	return D;
}*/

void DateToStr (SYSTEMTIME* Time)
{
	//将十进制数转换为液晶显示的ASCII 值，即变为字符型，此函数为年月日信息
	Time->DateString[0]=Time->Year/10 +'0';
	Time->DateString[1]=Time->Year%10 +'0';
	Time->DateString[2]='-';
	Time->DateString[3]=Time->Month/10 +'0';
	Time->DateString[4]=Time->Month%10 +'0';
	Time->DateString[5]='-';
	Time->DateString[6]=Time->Day/10 +'0';
	Time->DateString[7]=Time->Day%10 +'0';
	Time->DateString[8]='\0';
}

void TimeToStr(SYSTEMTIME* Time)
{
	//将十进制数转换为液晶显示的ASCII值，此处为时间信息
	Time->TimeString[0]=Time->Hour/10 +'0';		   
	Time->TimeString[1]=Time->Hour%10 +'0';
	Time->TimeString[2]=':';
	Time->TimeString[3]=Time->Minute/10 + '0';
	Time->TimeString[4]=Time->Minute%10+'0';
	Time->TimeString[5]=':';
	Time->TimeString[6]=Time->Second/10+ '0';
	Time->TimeString[7]=Time->Second%10 +'0';
	Time->DateString[8]='\0';
}
//还未实现星期的显示转换,改为使用数值显示}
/*uchar *WeekToStr(SYSTEMTIME Time)
{
	uint i;
	uchar *z;
	i=Time.Week;
	switch (i)
	{
	case 1:z="sun";break;
	case 2:z="mon";break;
	case 3:z="tue";break;
	case 4:z="wen";break;
	case 5:z="thu";break;
	case 6:z="fri";break;
	case 7:z="sat";break;
	}
	return z;
}*/
void Initial_DS1302 (void)
{
	unsigned char Second;
	Second=Read1302(DS1302_SECOND);
	if(Second&0x80)//初始化时间(
		DS1302_SetTime(DS1302_SECOND,0);
	DS1302_SetTime(DS1302_MINUTE,0);
	DS1302_SetTime(DS1302_YEAR,21);
	DS1302_SetTime(DS1302_MONTH,12);
	DS1302_SetTime(DS1302_DAY,17);
}
/*void DS1302_Timestop(bit flag)
{
	unsigned char Data;
	Data=Read1302(DS1302_SECOND);
	DS1302_SetProtect (0);
	if(flag)
		Write1302(DS1302_SECOND, Data|0x80);
	else
		Write1302(DS1302_SECOND,Data&0x7F);
}*/
void Save_Time(SYSTEMTIME* T)
{
	DS1302_SetTime(DS1302_SECOND,T->Second);
	DS1302_SetTime(DS1302_MINUTE,T->Minute);
	DS1302_SetTime(DS1302_HOUR,T->Hour);
	DS1302_SetTime(DS1302_YEAR,T->Year);
	DS1302_SetTime(DS1302_MONTH,T->Month);
	DS1302_SetTime(DS1302_DAY,T->Day);
}
#endif
