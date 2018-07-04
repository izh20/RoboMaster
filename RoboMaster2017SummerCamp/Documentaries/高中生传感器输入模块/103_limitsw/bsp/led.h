#ifndef __LED_H__
#define __LED_H__

#define LED_GREEN_OFF()  GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define LED_GREEN_ON()   GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define LED_GREEN_TOGGLE()  GPIOB->ODR ^= GPIO_Pin_12

#define LED_RED_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define LED_RED_ON()   GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define LED_RED_TOGGLE()  GPIOB->ODR ^= GPIO_Pin_13

void LED_Configuration(void);

#endif 
