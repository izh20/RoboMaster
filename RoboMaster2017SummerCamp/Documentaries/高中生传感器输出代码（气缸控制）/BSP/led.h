#ifndef __LED_H__
#define __LED_H__

#define LED_0_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_12)  //Can
#define LED_0_ON()   GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define LED_0_TOGGLE()  GPIOB->ODR ^= GPIO_Pin_12

#define LED_1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_13)  //Usart
#define LED_1_ON()   GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define LED_1_TOGGLE()  GPIOB->ODR ^= GPIO_Pin_13

void LED_Configuration(void);

#endif 
