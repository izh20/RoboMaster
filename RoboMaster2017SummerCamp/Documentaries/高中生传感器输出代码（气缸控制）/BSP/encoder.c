#include "main.h"

void Encoder_Configuration(void)
{
    GPIO_InitTypeDef gpio;
    TIM_TimeBaseInitTypeDef  tim;
    TIM_ICInitTypeDef tim_ic;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
     
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
		gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB,&gpio);

    TIM_DeInit(TIM4);
    TIM_TimeBaseInit(TIM4, &tim);   
		tim.TIM_Period =0xffff;       
		tim.TIM_Prescaler =0;            
		tim.TIM_ClockDivision =TIM_CKD_DIV1 ;       
		tim.TIM_CounterMode = TIM_CounterMode_Up; 
		//tim.TIM_CounterMode = TIM_CounterMode_CenterAligned1; 
		TIM_TimeBaseInit(TIM4, &tim);
 

    TIM_EncoderInterfaceConfig(TIM4, 
	               TIM_EncoderMode_TI12,
							   TIM_ICPolarity_Falling,
							   TIM_ICPolarity_Falling);
								 
  TIM_ICStructInit(&tim_ic);
  tim_ic.TIM_ICFilter = 6;         
  TIM_ICInit(TIM4, &tim_ic);
  
//TIM_ARRPreloadConfig(TIM4, ENABLE);
// Clear all pending interrupts
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);   
  //Reset counter
  TIM4->CNT =0;
    
	TIM_Cmd(TIM4,ENABLE);
}

void Encoder_Start(void)
{
    TIM4->CNT = 0x7fff;
	  ShootCtrl.ShootEncoder.ecd_bios = TIM4->CNT;
	  ShootCtrl.ShootEncoder.ecd_cnt = (TIM4->CNT);
}





int32_t GetQuadEncoderDiff(void)
{   
	  ShootCtrl.ShootEncoder.ecd_cnt_last = ShootCtrl.ShootEncoder.ecd_cnt;
    ShootCtrl.ShootEncoder.ecd_cnt = (TIM4->CNT);
	  ShootCtrl.ShootEncoder.ecd_diff = ShootCtrl.ShootEncoder.ecd_cnt - ShootCtrl.ShootEncoder.ecd_cnt_last;
    if(ShootCtrl.ShootEncoder.ecd_diff>0x7fff)
		{
      ShootCtrl.ShootEncoder.ecd_diff = ShootCtrl.ShootEncoder.ecd_diff - 0xffff;		
		}
		else if(ShootCtrl.ShootEncoder.ecd_diff<-(0x7fff))
		{
		  ShootCtrl.ShootEncoder.ecd_diff = ShootCtrl.ShootEncoder.ecd_diff + 0xffff;	
		}else
		{
		  ShootCtrl.ShootEncoder.ecd_diff = ShootCtrl.ShootEncoder.ecd_diff;
		}

		return ShootCtrl.ShootEncoder.ecd_diff;
}
