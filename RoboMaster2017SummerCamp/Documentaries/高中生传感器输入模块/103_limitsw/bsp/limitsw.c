#include "main.h"


//PA4  SW16 M8
//PA5  SW15 M7
//PA6  SW14 M6
//PA7  SW13 M5
//PB0  SW8  M4
//PB1  SW7  M3
//PB5  SW12 M13
//PB6  SW1  M12
//PB7  SW2  M11
//PB8  SW3  M10
//PB9  SW4  M9
//PB10 SW6  M2
//PB11 SW5  M1
//PB3  SW10 M15    不能用，与JTAG接口冲突
//PB4  SW11 M14    不能用，与JTAG接口冲突
//PA15 SW9  M16    不能用，与JTAG接口冲突

void LimitSW_Configuration(void)
{
	GPIO_InitTypeDef gpio;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
	gpio.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5| GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9| GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB, &gpio);
}
