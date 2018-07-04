#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f10x.h"

#define M1_PWM1      TIM3->CCR1
#define M1_PWM2			 TIM3->CCR2
#define M2_PWM1 		 TIM3->CCR3
#define M2_PWM2	     TIM3->CCR4

#define SINGLE_ANGLE  835   //715 
#define SINGLE_ANGLE_BACK  350

#define END         0
#define START       1
#define SINGLESTEP  2

#define DIR_CHANGE_NUM  84

typedef struct
{
	float ref;
	float fdb;
	float P;
	float I;
	float D;
	float E;
	float PreE;
	float PrePreE;
	float Edead;
	float ESum;
	float out;
	float out_limit;
}ST_PID;

typedef struct 
{
  int32_t ecd_bios;
	int32_t ecd_cnt;
	int32_t ecd_cnt_last;
	int32_t ecd_diff;
}Encoder;

typedef struct
{
  u8 dir;
  uint16_t speed;
  u8 ModeFlag[2];  //0 停止   1 启动    2 转动固定角度
	u8 SingleStepOverFlag;
	uint16_t SingleStep_Counter;
	
	uint16_t ctrl_counter;
	u8 DirChange_Flag;
}Ctrl_Data;

typedef struct 
{
  uint32_t win_ticks;
	uint8_t bullet_num;
	uint32_t bullet_time[5];
	uint8_t StopShootFlag;
}Shoot_Freq_Ctrl;

typedef struct
{
  ST_PID PositionPid;
	ST_PID SpeedPid;
	Encoder ShootEncoder;
	Ctrl_Data CtrlData;
	Shoot_Freq_Ctrl ShootFreqCtrl;
}Shoot_Ctrl;


void ShootCtrlTask(Shoot_Ctrl *shoot_ctrl);
void ShootMotorControl_Position(u8 dir,uint16_t motor_target_angle);
void ShootMotorControl_Speed(u8 dir,float motor_target_speed);
void Motor_Init(void);
extern void ShootFreq_Ctrl(void);

#endif
