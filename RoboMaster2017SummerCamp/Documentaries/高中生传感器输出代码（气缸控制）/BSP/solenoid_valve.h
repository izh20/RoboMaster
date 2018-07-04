#ifndef __SOLENOID_VALVE_H__
#define __SOLENOID_VALVE_H__

extern int sv1_state,sv2_state,sv3_state,sv4_state;
#define SV_1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_11)  //Can
#define SV_1_OFF()   GPIO_SetBits(GPIOB,GPIO_Pin_11)

#define SV_2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_10)  //Usart
#define SV_2_OFF()   GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define SV_3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_1)  //Usart
#define SV_3_OFF()   GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define SV_4_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_0)  //Usart
#define SV_4_OFF()   GPIO_SetBits(GPIOB,GPIO_Pin_0)

void SV_Configuration(void);

#endif 
