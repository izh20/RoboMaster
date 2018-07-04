#include "main.h"

//LED_GREEN----PB12, '0' is ON,'1' is OFF
//LED_RED------PB13, '0' is ON,'1' is OFF

void LED_Configuration(void)
{
  GPIO_InitTypeDef gpio;   

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;	
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
    
	LED_0_OFF(); //Can
	LED_1_OFF();  //Usart
}
