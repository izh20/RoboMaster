#include "main.h"

//LED_GREEN----PB13
//LED_RED------PB12

void LED_Configuration(void)
{
  GPIO_InitTypeDef gpio;   

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;	
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
    
	LED_GREEN_OFF();
	LED_RED_OFF();
}
