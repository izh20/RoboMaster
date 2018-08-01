#include "execute_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "cmsis_os.h"
#include "calibrate.h"
#include "pid.h"
#include "sys.h"
#include "keyboard.h"

#define W_IN_SPEED 8000				//后面摩擦轮进的速度
#define W_OUT_SPEED 8000				//出
#define OPEN_IO1 850			//爪子张开状态1舵机的角度, 以下同理
#define OPEN_IO2 1100
#define CLOSE_IO1 1550
#define CLOSE_IO2 550
 
extern uint8_t b_in;
extern uint8_t b_out;
extern uint8_t b_open;
extern uint8_t b_close;

pid_t pid_w_moto[4]     = { 0 };
pid_t pid_w_3508_moto     = { 0 };
//uint8_t test_key_value;
extern uint8_t test_servo;

//uint16_t servo_angle = 1000;

int16_t w_2006_moto_speed[4];
int16_t w_3508_moto_speed;
int16_t w_2006_moto_current[4] = {0};
int16_t w_3508_moto_current;

void send_2006_moto_current(int16_t current[])
{
  static uint8_t data[8];
  //int16_t trigger_current = trigger_moto_current;
  
  data[0] = current[0] >> 8;
  data[1] = current[0];
  data[2] = current[1] >> 8;
  data[3] = current[1];
  data[4] = current[2] >> 8;
  data[5] = current[2];
  data[6] = current[3] >> 8;
  data[7] = current[3];
  
  write_can(GIMBAL_CAN, CAN_GIMBAL_ID, data);
}

void send_w_3508_moto_current(int16_t current)
{
  static uint8_t data[8];
  //int16_t trigger_current = trigger_moto_current;
  
  data[0] = current >> 8;
  data[1] = current;
  data[2] = 0;
  data[3] = 0;
  data[4] = 0;
  data[5] = 0;
  data[6] = 0;
  data[7] = 0;
  
  write_can(2, CAN_CHASSIS_ID, data);
}

//电机初始化参数设定
void w_moto_init(void)
{
   //PID参数初始化		 
	for (int i = 0; i <=1; i++)
		pid_init(&pid_w_moto[i], 7000, 0, 2, 0, 0);
	pid_init(&pid_w_moto[2], 12000, 0, 4, 0, 0);
}

void execute_task(const void* argu)
{
	osDelay(3000);	
  w_moto_init();
   set_pwm_group_param(PWM_GROUP1,20000);
   start_pwm_output(PWM_IO1);
	 start_pwm_output(PWM_IO2);
	
  while(1) {
		pc_kb_hook();
		//电机的速度给定]
		if (b_in == 1) {
			w_2006_moto_speed[0] = -W_IN_SPEED;
			w_2006_moto_speed[1] = W_IN_SPEED;
		} else if (b_out == 1) {
			w_2006_moto_speed[0] = W_IN_SPEED;
	  w_2006_moto_speed[1] = -W_IN_SPEED;
		} else {
			w_2006_moto_speed[0] = 0;
			w_2006_moto_speed[1] = 0;
		}
	  
	  w_2006_moto_speed[2] = rc.ch2 / RC_MAX_VALUE * MAX_WHEEL_RPM / 7;

		if (b_open == 1) {
			set_pwm_param(PWM_IO1,OPEN_IO1);
			set_pwm_param(PWM_IO2,OPEN_IO2);
		}
		else if (b_close == 1) {
			set_pwm_param(PWM_IO1,CLOSE_IO1);
			set_pwm_param(PWM_IO2,CLOSE_IO2);
		} else {
			b_open = 1;
		}
		
		
	  for (int i = 0; i <= 2; i++)
		  w_2006_moto_current[i] = pid_calc(&pid_w_moto[i], w_2006_moto_ID_5_plus[i].speed_rpm, w_2006_moto_speed[i]);
	  send_2006_moto_current(w_2006_moto_current);
		osDelay(5);
	}
}
