#ifndef __GM_PID_H_
#define __GM_PID_H_

#include "stm32f10x.h"
#include "main.h"

#define SHOOT_SPEED_KP_DEFAULTS  60  
#define SHOOT_SPEED_KI_DEFAULTS  0
#define SHOOT_SPEED_KD_DEFAULTS  0
#define SHOOT_SPEED_LIMIT_DEFAULTS  3000

#define SHOOT_POSITION_KP_DEFAULTS  0.1  
#define SHOOT_POSITION_KI_DEFAULTS  0
#define SHOOT_POSITION_KD_DEFAULTS  0
#define SHOOT_POSITION_LIMIT_DEFAULTS  20

#define SHOOT_SPEED_ERROR_MAX     1000
#define SHOOT_SPEED_ERROR_MIN     -1000
#define SHOOT_CURRENT_LIMIT       1200
#define PID_SHOOT_MOTOR_SPEED     -50







void CalPID_Increment (ST_PID *stpid);
void CalPID(ST_PID *stpid);

#endif 
