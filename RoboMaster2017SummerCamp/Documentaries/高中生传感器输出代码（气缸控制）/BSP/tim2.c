#include "main.h"

int16_t tim_tick;

void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 72-1;    //APB1 is 72MHz
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 1000-1;     //1ms
    TIM_TimeBaseInit(TIM2,&tim);
}

void TIM2_Start(void)
{
    TIM_Cmd(TIM2, ENABLE);	 
    TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
}

void TIM2_IRQHandler(void)
{
	tim_tick++;
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
	{
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
		
		if((tim_tick%4) == 0)
		{

		}
		if((tim_tick%1000) == 0)
		{
       
		}
		if((tim_tick%500) == 0)
		{

		}
		
  }
}


