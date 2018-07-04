#include "main.h"

void delay_ms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=10255;
 		while(a--);
	}
}

void delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=9;
		while(a--);
	}
}
