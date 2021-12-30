#ifndef TIMER_DS1302
#define TIMER_DS1302
sbit DS1302_CLK = P3^6;//ʵʱʱ��ʱ��������
sbit DS1302_IO = P3^4;//ʵʱʱ������������
sbit DS1302_RST = P3^5;//ʵʱʱ�Ӹ�λ������
sbit ACC0 = ACC^0;//����ACC�����λ�����λ,�ڶ�ACC��λ������,���ڴ�������
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
unsigned char DateString [9];//���������ַ��������ö�ȡ��ʱ��
unsigned char TimeString[9];
}SYSTEMTIME; //�����ʱ�����ͽṹ��
#define AM(X) X
#define PM(X) (X+12) //ת��24Сʱ��
#define DS1302_SECOND 0x80 //Ƭ�ڸ�λ���ݵĵ�ַ
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 0x84
#define DS1302_WEEK 0x8A
#define DS1302_DAY 0x86
#define DS1302_MONTH 0x88
#define DS1302_YEAR 0x8C
#define DS1302_RAM(X)(0xC0+(X)*2)
//���ڼ��� DS1302_RAM ��ַ�ĺ�
/******�ڲ�ָ��***���***��*��*��*/
void DS1302InputByte(unsigned char d)	//ʵʱʱ��д��1B(�ڲ�����)
{
	unsigned char i;
	ACC=d;
	for(i=8;i>0;i--)
	{
		DS1302_IO=ACC0;
		DS1302_CLK=1; //д������������,����д��λ��д��λ
		DS1302_CLK=0; //��Ϊ��ǰ���Ѷ���ACCO=ACC^0;�Ա��ٴ�����
		ACC=ACC>>1;
	}
}
//��������:ʵʱʱ�Ӷ�ȡ1B(�ڲ�����)
unsigned char DS1302OutputByte (void)
{
	unsigned char i;
	for(i=8; i>0; i--)
	{
		ACC=ACC >>1;
		//�൱�ڻ���е� RRC
		ACC7=DS1302_IO;
		//�ɵ�λ����λ����AcC7�е���Ϣ
		DS1302_CLK=1;
		//����Ϣ�����½���
		DS1302_CLK=0;
	}		  
	return(ACC);
}
void Write1302(unsigned char ucAddr, unsigned char ucDa) //ucAddr: DS1302��ַ,
{
	//ucData:Ҫд������
	DS1302_RST =0;
	DS1302_CLK =0;
	DS1302_RST =1;
	DS1302InputByte(ucAddr) ;
	//��ַ,����
	DS1302InputByte(ucDa);
	//д1B����
	DS1302_CLK =1;
	DS1302_RST =0;
}
unsigned char Read1302(unsigned char ucAddr) //��ȡDS1302ĳ��ַ������
{
	unsigned char ucData;
	DS1302_RST=0;
	DS1302_CLK = 0;
	DS1302_RST =1;
	DS1302InputByte(ucAddr|0x01);
	//������,д��ַ,����
	ucData = DS1302OutputByte();
	//�½���,��1B����
	DS1302_CLK =1;
	DS1302_RST =0;
	return(ucData) ;
	//��������֮�����д����,���½���֮ǰ���ж�����
}
void DS1302_SetProtect(bit flag) //!�Ƿ�д����
{
	if(flag)
		Write1302(0X8E,0x80);
	else
		Write1302(0X8E,0x00);
}

void DS1302_SetTime (unsigned char Address,unsigned char Value)//��������:����ʱ��
{
	DS1302_SetProtect(0);
	Write1302(Address, ((Value/10)<<4 | (Value%10)));//��ʮ������ת��ΪBCD��
}
//�� DS1302�е���������ʱ����صļĴ�����ŵ����ݱ���ΪBCD����ʽ
void DS1302_GetTime (SYSTEMTIME *Time)
{
	unsigned char ReadValue;
	ReadValue = Read1302(DS1302_SECOND);
	Time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);//��BCD��ת��Ϊʮ������,�˴�Ϊ�ṹ�����
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
	//��ʮ������ת��ΪҺ����ʾ��ASCII ֵ������Ϊ�ַ��ͣ��˺���Ϊ��������Ϣ
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
	//��ʮ������ת��ΪҺ����ʾ��ASCIIֵ���˴�Ϊʱ����Ϣ
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
//��δʵ�����ڵ���ʾת��,��Ϊʹ����ֵ��ʾ}
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
	if(Second&0x80)//��ʼ��ʱ��(
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
