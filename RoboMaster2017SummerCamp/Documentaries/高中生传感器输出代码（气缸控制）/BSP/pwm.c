#include "main.h"

//PA8----PWM---TIM1_CH1
//PA9----PWM---TIM1_CH2
//PA10---PWM---TIM1_CH3
//PA11---PWM---TIM1_CH4

//PA0----PWM---TIM2_CH1
//PA1----PWM---TIM2_CH2
//PB10---PWM---TIM2_CH3
//PB11---PWM---TIM2_CH4

//PA6----PWM---TIM3_CH1
//PA7----PWM---TIM3_CH2
//PB0----PWM---TIM3_CH3
//PB1----PWM---TIM3_CH4
/*************************************************************************
                              PWM≥ı ºªØ      20ms-50Hz 
*************************************************************************/
void PWM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	TIM_OCInitTypeDef        oc;
  GPIO_InitTypeDef         gpio;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_TIM1, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | 
                         RCC_APB1Periph_TIM3, ENABLE);	
	
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);
	
  //TIM1
  gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);  
  //TIM2
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio); 
  gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);  
	//TIM3
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio); 
  gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);  

	tim.TIM_Period=20000-1;
	tim.TIM_Prescaler=72-1;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &tim);
	TIM_TimeBaseInit(TIM2, &tim);
	TIM_TimeBaseInit(TIM3, &tim);
	
	oc.TIM_OCMode = TIM_OCMode_PWM2;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_OutputNState = TIM_OutputNState_Disable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	oc.TIM_OCNPolarity = TIM_OCPolarity_High;
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &oc);
	TIM_OC2Init(TIM1, &oc);
	TIM_OC3Init(TIM1, &oc);
	TIM_OC4Init(TIM1, &oc);
	
	TIM_OC1Init(TIM2, &oc);
	TIM_OC2Init(TIM2, &oc);
	TIM_OC3Init(TIM2, &oc);
	TIM_OC4Init(TIM2, &oc);
	
	TIM_OC1Init(TIM3, &oc);
	TIM_OC2Init(TIM3, &oc);
	TIM_OC3Init(TIM3, &oc);
	TIM_OC4Init(TIM3, &oc);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	
	TIM_Cmd(TIM1, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}
