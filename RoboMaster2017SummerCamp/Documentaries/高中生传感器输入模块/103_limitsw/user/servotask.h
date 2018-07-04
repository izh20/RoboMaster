#ifndef __SERVO_H__
#define __SERVO_H__

#include "stm32f10x.h"

typedef struct Servo
{
	uint8_t  channel;
	uint16_t frequency;
	uint16_t angel;
}Servo;

#endif 
