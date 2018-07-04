#include "main.h"

void delay_ms(uint32_t nTime)
{
  TimingDelay = nTime;

  while (TimingDelay != 0);
}
void SysTick_Init(void)
{
	if (SysTick_Config(SystemCoreClock / 1000))
  { 
    while (1);
  }
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


