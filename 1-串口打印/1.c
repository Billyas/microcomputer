#include<AT89X51.H>
#include<stdio.h>

void main(void)
{
	SCON = 0X50;
	TMOD = 0X20;
	TCON = 0X40;
	TH1 = 0XE8;
	TL1 = 0XE8;
	TI = 1;
	TR1 = 1; 
	while(1){
		printf("HELLO WORLD 201921121073 ¡Ë…Ÿ∆Ω\n");
	}
}