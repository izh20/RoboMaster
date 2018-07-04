#include "main.h"

//USART2_TX----PA2
//USART2_RX------PA3

uint8_t rx_buffer[JUDGE_RX_BUFFER_LEN];
//eBuffType 
tGpsData gRobotGps;
tGameInfo gRobotGameInfo;
tRealBloodChangedData gRobotBlood;
tRealShootData gRobotShoot;

void USART2_Configuration(void)
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_9;  
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio);

    USART_DeInit(USART1);
    usart.USART_BaudRate = 115200;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No ;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART1,&usart);
    USART_Cmd(USART1,ENABLE);
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
    
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
    dma.DMA_MemoryBaseAddr = (uint32_t)rx_buffer;   
    dma.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma.DMA_BufferSize = JUDGE_RX_BUFFER_LEN;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5,&dma);

    DMA_Cmd(DMA1_Channel5, ENABLE);
 
    nvic.NVIC_IRQChannel = USART1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);        //usart rx idle interrupt  enabled
    USART_Cmd(USART1, ENABLE);
		
		gRobotGameInfo.realChassisRemainJ = 60; //
}

void USART1_SendChar(char b)
{
  while( USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	USART_SendData(USART1,b);
}
int flags=0;
void USART1_IRQHandler() 
{
	uint16_t stream_length = 0;
	uint16_t judge_data_length = 0;
	uint16_t judge_cmd_id = 0;
	uint8_t judge_data_crc8 = 0;
	uint16_t judge_data_crc16 = 0;
	if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
	{
		(void)USART1->SR;
		(void)USART1->DR;
		DMA_Cmd(DMA1_Channel6, DISABLE);
		stream_length = JUDGE_RX_BUFFER_LEN - DMA_GetCurrDataCounter(DMA1_Channel6);
		DMA1_Channel6->CNDTR = (uint16_t)JUDGE_RX_BUFFER_LEN;     //relocate the dma memory pointer to the beginning position
		if(0xA5 == rx_buffer[0])
		{
			/* frame header */
			judge_data_length = (rx_buffer[2]<<8) | rx_buffer[1];
			judge_data_crc8 = rx_buffer[3];
			
			/* cmdID */
			judge_cmd_id = (rx_buffer[5]<<8) | rx_buffer[4];
			
			/* Judge Data */
			if (0x0001 == judge_cmd_id)
			{
				gRobotGameInfo.remainTime = rx_buffer[6] | (rx_buffer[7]<<8) | (rx_buffer[8]<<16) | (rx_buffer[9]<<24);
				gRobotGameInfo.remainLifeValue = rx_buffer[10] | (rx_buffer[11]<<8);
				gRobotGameInfo.realChassisOutV = *((float*)&rx_buffer[12]);//(float)(rx_buffer[15] | (rx_buffer[14]<<8) | (rx_buffer[13]<<16) | (rx_buffer[12]<<24));
				gRobotGameInfo.realChassisOutA = *((float*)&rx_buffer[16]);//(float)(rx_buffer[19] | (rx_buffer[18]<<8) | (rx_buffer[17]<<16) | (rx_buffer[16]<<24));
				
				gRobotGameInfo.realChassisOutP = gRobotGameInfo.realChassisOutA * gRobotGameInfo.realChassisOutV;
				gRobotGameInfo.realChassisRemainJ += (80 - gRobotGameInfo.realChassisOutP) * 0.02;
				if(gRobotGameInfo.realChassisRemainJ > 60) gRobotGameInfo.realChassisRemainJ = 60;
				if(gRobotGameInfo.realChassisRemainJ < 0) gRobotGameInfo.realChassisRemainJ = 0;
				
				LED_1_TOGGLE();
				flags=1;
				//SendPowerLimitData();
				
				gRobotGameInfo.runeStatus[0] = rx_buffer[20];
				gRobotGameInfo.runeStatus[1] = rx_buffer[21];
				gRobotGameInfo.runeStatus[2] = rx_buffer[22];
				gRobotGameInfo.runeStatus[3] = rx_buffer[23];
				gRobotGameInfo.bigRune0Status = rx_buffer[24];
				gRobotGameInfo.bigRune1Status = rx_buffer[25];
				gRobotGameInfo.conveyorBelts0 = rx_buffer[26] & 0x03;
				gRobotGameInfo.conveyorBelts1 = rx_buffer[26] & 0x0c;
				gRobotGameInfo.parkingApron0 = rx_buffer[26] & 0x10;
				gRobotGameInfo.parkingApron1 = rx_buffer[26] & 0x20;
				gRobotGameInfo.parkingApron2 = rx_buffer[26] & 0x40;
				gRobotGameInfo.parkingApron3 = rx_buffer[26] & 0x80;
				
				gRobotGameInfo.gpsData.flag = rx_buffer[27];
				gRobotGameInfo.gpsData.x = rx_buffer[28] | (rx_buffer[29]<<8) | (rx_buffer[30]<<16) | (rx_buffer[31]<<24);
				gRobotGameInfo.gpsData.y = rx_buffer[32] | (rx_buffer[33]<<8) | (rx_buffer[34]<<16) | (rx_buffer[35]<<24);
				gRobotGameInfo.gpsData.z = rx_buffer[36] | (rx_buffer[37]<<8) | (rx_buffer[38]<<16) | (rx_buffer[39]<<24);
				gRobotGameInfo.gpsData.compass = rx_buffer[40] | (rx_buffer[41]<<8) | (rx_buffer[42]<<16) | (rx_buffer[43]<<24);

				/* frame tail */
				judge_data_crc16 = rx_buffer[44] | (rx_buffer[45]<<8);
			}
			else if (0x0002 == judge_cmd_id)
			{
				gRobotBlood.weakId = rx_buffer[6] & 0x0f;
				gRobotBlood.way = rx_buffer[6] & 0xf0;
				gRobotBlood.value = rx_buffer[7] | (rx_buffer[8]<<8);

				judge_data_crc16 = rx_buffer[9] | (rx_buffer[10]<<8);
			}
			else if(0x0003 == judge_cmd_id)
			{
				gRobotShoot.realBulletShootSpeed = (float)(rx_buffer[9] | (rx_buffer[8]<<8) | (rx_buffer[7]<<16) | (rx_buffer[6]<<24));
				gRobotShoot.realBulletShootFreq = (float)(rx_buffer[13] | (rx_buffer[12]<<8) | (rx_buffer[11]<<16) | (rx_buffer[10]<<24));
				gRobotShoot.realGolfShootSpeed = (float)(rx_buffer[17] | (rx_buffer[16]<<8) | (rx_buffer[15]<<16) | (rx_buffer[14]<<24));
				gRobotShoot.realGolfShootFreq = (float)(rx_buffer[21] | (rx_buffer[20]<<8) | (rx_buffer[19]<<16) | (rx_buffer[18]<<24));

				judge_data_crc16 = rx_buffer[23] | (rx_buffer[24]<<8);
			}
			
			
		}
		DMA_Cmd(DMA1_Channel6, ENABLE);
	}
}
