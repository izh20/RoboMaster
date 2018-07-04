#include "main.h"

//LED_GREEN----PB12, '0' is ON,'1' is OFF
//LED_RED------PB13, '0' is ON,'1' is OFF
int sv1_state=1,sv2_state=0,sv3_state=1,sv4_state=0;
void SV_Configuration(void)
{
  GPIO_InitTypeDef gpio;   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO| RCC_APB2Periph_GPIOB, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
	
//	gpio.GPIO_Pin = GPIO_Pin_15;	
//	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
//	gpio.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &gpio);
    
	gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_1|GPIO_Pin_0;	
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);
	
	SV_1_ON(); 
	SV_2_ON(); 
	SV_3_ON(); 
	SV_4_ON(); 
}
