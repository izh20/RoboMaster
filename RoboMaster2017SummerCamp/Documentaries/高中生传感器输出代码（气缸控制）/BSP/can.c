#include "main.h"

//HVD232---CAN_TX---PA12
//HVD232---CAN_RX---PA11

void CAN_Configuration(void)
{
	CAN_InitTypeDef        can;
	CAN_FilterInitTypeDef  can_filter;
	GPIO_InitTypeDef 	   gpio;
	NVIC_InitTypeDef   	   nvic;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);	//CAN_RX

	gpio.GPIO_Pin = GPIO_Pin_12;	   
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);    //CAN_TX

	nvic.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
	
	nvic.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;  
	nvic.NVIC_IRQChannelPreemptionPriority = 1;
	nvic.NVIC_IRQChannelSubPriority = 1;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	CAN_DeInit(CAN1);

	can.CAN_TTCM = DISABLE;
	can.CAN_ABOM = DISABLE;
	can.CAN_AWUM = DISABLE;
	can.CAN_NART = DISABLE;	  
	can.CAN_RFLM = DISABLE;																
	can.CAN_TXFP = ENABLE;		
	can.CAN_Mode = CAN_Mode_Normal;
	can.CAN_SJW = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_5tq;
	can.CAN_BS2 = CAN_BS2_3tq;
	can.CAN_Prescaler = 4;     //CAN BaudRate 36/(1+5+3)/4=1Mbps
	CAN_Init(CAN1, &can);

	can_filter.CAN_FilterNumber = 0; 
	can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
	can_filter.CAN_FilterIdHigh = 0;
	can_filter.CAN_FilterIdLow = 0;
	can_filter.CAN_FilterMaskIdHigh = 0;
	can_filter.CAN_FilterMaskIdLow = 0;
	can_filter.CAN_FilterFIFOAssignment = 0;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}
CanRxMsg rx_message;
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	  {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
				CanReceiveMsgProcess(&rx_message);
    }
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	  {
	      CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
    }
}

uint32_t ID;
int can_state_change=0;
int CanData;
void CanReceiveMsgProcess(CanRxMsg * msg)
{
	switch(msg->StdId)
	{
		case CAN_BUS_SOLENOID_VALVE_MSG_ID:
		{
			LED_0_TOGGLE();
			CanData=msg->Data[0]|msg->Data[1]<<1|msg->Data[2]<<2|msg->Data[3]<<3;
			can_state_change=1;
		}break;
		default:
		{
		}
	}
}

void SendSolenoidState(void)
{
    CanTxMsg tx_message;    
    tx_message.StdId =0x502;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = sv1_state;
    tx_message.Data[1] = sv2_state;
    tx_message.Data[2] = sv3_state;
    tx_message.Data[3] = sv4_state;
    tx_message.Data[4] = 0x00;
    tx_message.Data[5] = 0x00;
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;
    CAN_Transmit(CAN1,&tx_message);
}

