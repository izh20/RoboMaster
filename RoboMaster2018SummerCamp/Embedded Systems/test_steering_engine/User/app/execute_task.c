#include "execute_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "cmsis_os.h"
#include "calibrate.h"
#include "pid.h"
#include "sys.h"



//uint8_t test_key_value;
extern uint8_t test_servo;

//uint16_t servo_angle = 1000;



void execute_task(const void* argu)
{
	//舵机控制函数周期设定
   set_pwm_group_param(PWM_GROUP1,20000);
	
   //开启控制端口
   start_pwm_output(PWM_IO1);
	set_pwm_param(PWM_IO1,1500);
    osDelay(2000);
   while(1)
  { 
		set_pwm_param(PWM_IO1,600);
		osDelay(2000);
		set_pwm_param(PWM_IO1,500);
		osDelay(2000);
		set_pwm_param(PWM_IO1,800);
    osDelay(2000);
		set_pwm_param(PWM_IO1,1000);
		osDelay(2000);
		set_pwm_param(PWM_IO1,1200);
    osDelay(2000);
		set_pwm_param(PWM_IO1,1400);
    osDelay(2000);
		set_pwm_param(PWM_IO1,1600);
		osDelay(2000);
		set_pwm_param(PWM_IO1,1800);
    osDelay(2000);
		set_pwm_param(PWM_IO1,2000);
		osDelay(2000);
		set_pwm_param(PWM_IO1,2200);
    osDelay(2000);
		set_pwm_param(PWM_IO1,2400);
    osDelay(2000);
		set_pwm_param(PWM_IO1,2500);
    osDelay(2000);
  }
}
