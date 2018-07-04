#ifndef __USART2_H_
#define __USART2_H_

#include "stm32f10x.h"

#define JUDGE_RX_BUFFER_LEN 85

/* for 0x0001 */

typedef enum{
	BUFF_TYPE_NONE,          //??
	BUFF_TYPE_ARMOR = 0x01, //???
	BUFF_TYPE_SUPPLY = 0x04, //???
	BUFF_TYPE_BULLFTS= 0x08, //???
}eBuffType;


typedef __packed struct
{
	uint8_t flag; //0 ??,1??
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t compass;
}tGpsData;


typedef __packed struct
{
	uint32_t remainTime;
	uint16_t remainLifeValue;
	float realChassisOutV;       //????
	float realChassisOutA;       //????
	float realChassisRemainJ;			//?????,??60J
	float realChassisOutP;				//????
	uint8_t runeStatus[4];       //?????
	uint8_t bigRune0Status;      //??
	uint8_t bigRune1Status;
	
	//????,????,?????????
	uint8_t conveyorBelts0:2;    //???
	uint8_t conveyorBelts1:2;
	uint8_t parkingApron0:1;     //???
	uint8_t parkingApron1:1;
	uint8_t parkingApron2:1;
	uint8_t parkingApron3:1;
	tGpsData gpsData;
}tGameInfo;

/* for 0x0002 */

typedef __packed struct
{
	uint8_t weakId:4;
	uint8_t way:4;
	uint16_t value;
}tRealBloodChangedData;

/* for 0x0003 */
typedef __packed struct
{
	float realBulletShootSpeed;
	float realBulletShootFreq;
	float realGolfShootSpeed;
	float realGolfShootFreq;
}tRealShootData;

/* function declaration */
void USART2_Configuration(void);

extern tGpsData gRobotGps;
extern tGameInfo gRobotGameInfo;
extern tRealBloodChangedData gRobotBlood;
extern tRealShootData gRobotShoot;
extern uint8_t rx_buffer[JUDGE_RX_BUFFER_LEN];
extern int flags;

#endif

