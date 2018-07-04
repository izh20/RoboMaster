#include "main.h"

void BSP_Init(void)
{  
	SysTick_Init();
	LED_Configuration();
	LimitSW_Configuration();
  CAN_Configuration();
}
