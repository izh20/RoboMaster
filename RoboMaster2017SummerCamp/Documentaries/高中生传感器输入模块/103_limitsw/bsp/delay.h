#ifndef __DELAY_H__
#define __DELAY_H__
#include "main.h"

static uint32_t TimingDelay;


void SysTick_Init(void);
void delay_ms(unsigned int t);
//void delay_us(unsigned int t);

#endif
