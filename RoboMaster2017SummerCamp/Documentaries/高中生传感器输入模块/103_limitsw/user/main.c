#include "main.h"

//PA4  SW16 M8
//PA5  SW15 M7
//PA6  SW14 M6
//PA7  SW13 M5
//PB0  SW8  M4
//PB1  SW7  M3
//PB5  SW12 M13
//PB6  SW1  M12
//PB7  SW2  M11
//PB8  SW3  M10
//PB9  SW4  M9
//PB10 SW6  M2
//PB11 SW5  M1
//PB3  SW10 M15    不能用，与JTAG接口冲突
//PB4  SW11 M14    不能用，与JTAG接口冲突
//PA15 SW9  M16    不能用，与JTAG接口冲突

CanTxMsg tx_message;


int main(void)
{
	BSP_Init();
  
	tx_message.StdId = 0x401;
	tx_message.IDE = CAN_ID_STD;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.DLC = 0x08;	
	
	while(1)
	{
		tx_message.Data[0] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);  //SW1
		tx_message.Data[1] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);  //SW2
		tx_message.Data[2] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);  //SW3
		tx_message.Data[3] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);  //SW4
		tx_message.Data[4] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11); //SW5
		tx_message.Data[5] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10); //SW6
		tx_message.Data[6] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);  //SW7			
		tx_message.Data[7] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);  //SW8    
		CAN_Transmit(CAN1, &tx_message);
		//LED_RED_TOGGLE();
		//LED_GREEN_TOGGLE();
		delay_ms(5);

	}
}
