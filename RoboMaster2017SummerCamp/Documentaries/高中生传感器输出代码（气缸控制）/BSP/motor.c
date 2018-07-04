#include "main.h"

//-------------------|ON|---|ONR|--|OFF|--|OFF|
//MOTOR1_IN1----PA6  |'0'|--|'1'|--|'0'|--|'1'|
//MOTOR1_IN2----PA7, |'1'|--|'0'|--|'0'|--|'1'|

//MOTOR2_IN1----PB0, |'0'|--|'1'|--|'0'|--|'1'|
//MOTOR2_IN2----PB1, |'1'|--|'0'|--|'0'|--|'1'|

Shoot_Ctrl ShootCtrl;

void Motor_Init(void)
{
	TIM_TimeBaseInitTypeDef  tim;
	TIM_OCInitTypeDef        oc;
  GPIO_InitTypeDef         gpio;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	//TIM3
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio); 
  gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio);  

	tim.TIM_Period=1000-1;       //1ms
	tim.TIM_Prescaler=72-1;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &tim);
	
	oc.TIM_OCMode = TIM_OCMode_PWM2;
	oc.TIM_OutputState = TIM_OutputState_Enable;
	oc.TIM_OutputNState = TIM_OutputNState_Disable;
	oc.TIM_Pulse = 0;
	oc.TIM_OCPolarity = TIM_OCPolarity_Low;
	oc.TIM_OCNPolarity = TIM_OCPolarity_High;
	oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
	oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	
	TIM_OC1Init(TIM3, &oc);
	TIM_OC2Init(TIM3, &oc);	
	TIM_OC3Init(TIM3, &oc);
	TIM_OC4Init(TIM3, &oc);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	M1_PWM1 = 0;
	M1_PWM2 = 0;
	M2_PWM1 = 0;
	M2_PWM2 = 0;
	
	ShootCtrl.SpeedPid.P = SHOOT_SPEED_KP_DEFAULTS;  
	ShootCtrl.SpeedPid.I = SHOOT_SPEED_KI_DEFAULTS;
	ShootCtrl.SpeedPid.D = SHOOT_SPEED_KD_DEFAULTS;	
	ShootCtrl.SpeedPid.out_limit = SHOOT_SPEED_LIMIT_DEFAULTS;
	
	ShootCtrl.PositionPid.P = SHOOT_POSITION_KP_DEFAULTS;
	ShootCtrl.PositionPid.I = SHOOT_POSITION_KI_DEFAULTS;
	ShootCtrl.PositionPid.D = SHOOT_POSITION_KD_DEFAULTS;
	ShootCtrl.PositionPid.out_limit = 20;
	
}

void ShootMotorControl_Speed(u8 dir,float motor_target_speed)
{
	if(dir)
	{
		ShootCtrl.SpeedPid.fdb = GetQuadEncoderDiff();   	
    ShootCtrl.SpeedPid.ref = motor_target_speed;	
		CalPID(&ShootCtrl.SpeedPid);			
	}
	else
	{
		ShootCtrl.SpeedPid.fdb = GetQuadEncoderDiff();   
    if(motor_target_speed>0)		
      ShootCtrl.SpeedPid.ref = -motor_target_speed;	
		else
			ShootCtrl.SpeedPid.ref = motor_target_speed;
		CalPID(&ShootCtrl.SpeedPid);		
	}
	
  if(ShootCtrl.SpeedPid.ref == 0)
		ShootCtrl.SpeedPid.out = 0;
	
	if(ShootCtrl.SpeedPid.out < 0)
	{
		M1_PWM1 = 0;
		M1_PWM2 = -ShootCtrl.SpeedPid.out;	
	}
  else
	{
		M1_PWM1 = ShootCtrl.SpeedPid.out;
		M1_PWM2 = 0;	
	}		

}

void ShootMotorControl_Position(u8 dir,uint16_t motor_target_angle)
{
	
  if(dir)
	{
	  ShootCtrl.PositionPid.ref = motor_target_angle;
		ShootCtrl.PositionPid.fdb = ShootCtrl.ShootEncoder.ecd_cnt - ShootCtrl.ShootEncoder.ecd_bios;
		if(ShootCtrl.PositionPid.fdb < -(0x7fff))
			ShootCtrl.PositionPid.fdb = ShootCtrl.PositionPid.fdb + 0xffff;	
	
		CalPID(&ShootCtrl.PositionPid);
	}
	else
	{
	  ShootCtrl.PositionPid.ref = -motor_target_angle;
		ShootCtrl.PositionPid.fdb = ShootCtrl.ShootEncoder.ecd_cnt - ShootCtrl.ShootEncoder.ecd_bios;
		if(ShootCtrl.PositionPid.fdb > 0x7fff)
			ShootCtrl.PositionPid.fdb = ShootCtrl.PositionPid.fdb - 0xffff;	
		
		CalPID(&ShootCtrl.PositionPid);
	}
	
  ShootCtrl.CtrlData.ctrl_counter++;
	if(ShootCtrl.CtrlData.ctrl_counter >= DIR_CHANGE_NUM)
	{
	  ShootCtrl.CtrlData.DirChange_Flag = 1;
		ShootCtrl.CtrlData.dir = !ShootCtrl.CtrlData.dir;
		ShootCtrl.ShootEncoder.ecd_bios = ShootCtrl.ShootEncoder.ecd_cnt;
		ShootCtrl.CtrlData.ctrl_counter = 0;
	}
}

void ShootCtrlTask(Shoot_Ctrl *shoot_ctrl)
{

	if(shoot_ctrl->CtrlData.SingleStep_Counter)
		shoot_ctrl->CtrlData.ModeFlag[0] = SINGLESTEP;
	
	switch(shoot_ctrl->CtrlData.ModeFlag[0])
	{
		case START:
		{
		  ShootMotorControl_Speed(shoot_ctrl->CtrlData.dir,20);
		}break;
		
		case END:
		{
		  ShootMotorControl_Speed(shoot_ctrl->CtrlData.dir,0);
			shoot_ctrl->ShootEncoder.ecd_bios = shoot_ctrl->ShootEncoder.ecd_cnt;
		}break;
		
	  default:
	  {
//			if(shoot_ctrl->CtrlData.ModeFlag[1]!=SINGLESTEP)
//				shoot_ctrl->ShootEncoder.ecd_bios = shoot_ctrl->ShootEncoder.ecd_cnt;
			if(!ShootCtrl.CtrlData.DirChange_Flag)
			{
				ShootMotorControl_Position(shoot_ctrl->CtrlData.dir,SINGLE_ANGLE+57);
				if(abs(shoot_ctrl->PositionPid.out)<6)
				{
					shoot_ctrl->CtrlData.SingleStepOverFlag = 1;
					shoot_ctrl->CtrlData.SingleStep_Counter--;
					shoot_ctrl->CtrlData.ModeFlag[0] = 0;
					shoot_ctrl->ShootEncoder.ecd_bios = shoot_ctrl->ShootEncoder.ecd_cnt;
					ShootCtrl.CtrlData.ctrl_counter = 0;
				}else
				{
					ShootMotorControl_Speed(shoot_ctrl->CtrlData.dir,shoot_ctrl->PositionPid.out);
				}
		  }else
			{
			  ShootMotorControl_Position(shoot_ctrl->CtrlData.dir,SINGLE_ANGLE_BACK+57);
				if(abs(shoot_ctrl->PositionPid.out)<6)
				{
					shoot_ctrl->CtrlData.SingleStepOverFlag = 1;
					shoot_ctrl->CtrlData.SingleStep_Counter--;
					shoot_ctrl->CtrlData.ModeFlag[0] = 0;
					shoot_ctrl->ShootEncoder.ecd_bios = shoot_ctrl->ShootEncoder.ecd_cnt;
					ShootCtrl.CtrlData.ctrl_counter = 0;
					shoot_ctrl->CtrlData.dir = !shoot_ctrl->CtrlData.dir;
					ShootCtrl.CtrlData.DirChange_Flag = 0;
				}else
				{
					ShootMotorControl_Speed(shoot_ctrl->CtrlData.dir,shoot_ctrl->PositionPid.out);
				}
			}

		}break;
	
	}


}

void ShootFreq_Ctrl(void)
{
	u8 index;
	if(ShootCtrl.CtrlData.ModeFlag[0]==SINGLESTEP)
  {				
	  ShootCtrl.ShootFreqCtrl.bullet_time[ShootCtrl.ShootFreqCtrl.bullet_num] = ShootCtrl.ShootFreqCtrl.win_ticks;
		if(ShootCtrl.ShootFreqCtrl.bullet_num<4)
		{
			ShootCtrl.ShootFreqCtrl.bullet_num++;
		  ShootCtrl.CtrlData.SingleStep_Counter = 1;
		}
		else if(ShootCtrl.ShootFreqCtrl.bullet_num==4)
		{
		  if(ShootCtrl.ShootFreqCtrl.bullet_time[4]-ShootCtrl.ShootFreqCtrl.bullet_time[0]<1100)
			{
			  ShootCtrl.CtrlData.ModeFlag[0] = 0;
				ShootCtrl.ShootFreqCtrl.StopShootFlag = 1;
			}
			else
			{			
				ShootCtrl.CtrlData.SingleStep_Counter = 1;
				ShootCtrl.ShootFreqCtrl.StopShootFlag = 0;
				for(index=0;index<4;index++)
				{
					ShootCtrl.ShootFreqCtrl.bullet_time[index] = ShootCtrl.ShootFreqCtrl.bullet_time[index+1];
				}
			}
		}
	}  


}
