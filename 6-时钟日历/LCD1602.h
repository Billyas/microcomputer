#ifndef LCD_CHAR_1602_2005_4_9
#define LCD_CHAR_1602_2005_4_9
#define uchar unsigned char
#define uint unsigned int

sbit lcdrs = P2^6;
sbit lcdrw = P2^5;
sbit lcden = P2^7;

void delay (uint z)
{
	uint x,y;
	for(x=z;x>0 ; x--)
	for(y=110; y>0;y--);
}
void write_com(uchar com)
{
	lcdrw=0;
	lcdrs=0;
	P0=com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
void write_data(uchar date)
{
	lcdrw=0;
	lcdrs=1;
	P0=date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
}
void init1602()
{
	lcdrw=0;
	lcden=0;
	write_com(0x3C);   
	write_com(0x0C);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);
}

void write_string(uchar *pp, uint n)
{
	int i;
	for(i=0; i<n; i++)
	{
		write_data(pp[i]);
	}
}
#endif