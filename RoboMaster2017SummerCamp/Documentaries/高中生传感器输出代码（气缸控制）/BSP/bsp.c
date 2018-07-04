#include "main.h"

void BSP_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);     
	LED_Configuration();
	TIM2_Configuration();
	TIM2_Start();
	CAN_Configuration();
	SV_Configuration();

}
