#include "execute_task.h"
#include "can_device.h"
#include "uart_device.h"
#include "pid.h"
#include "sys.h"

int16_t test_moto_speed = 0;
int16_t test_moto_current[1];
uint8_t test_servo;
uint8_t key1;

void test_moto_control(void)
{
   //LED控制函数
	 write_led_io(LED_IO1,LED_ON );	
	
   //电机的速度给定
   test_moto_speed = rc.ch2 / RC_MAX_VALUE * MAX_WHEEL_RPM;
  
   //闭环计算电机电流
   test_moto_current[0] = pid_calc(&pid_test_moto, w_2006_moto_ID_5_plus[3].speed_rpm, test_moto_speed);
   
   //发送电机的电流
   set_test_motor_current(test_moto_current);
	
   //舵机控制函数周期设定
   set_pwm_group_param(PWM_GROUP1,20000);
	
   //开启控制端口
   start_pwm_output(PWM_IO1);
    
	//舵机控制命令
		if(test_servo == 0 )
		  {
			set_pwm_param(PWM_IO1,2200);
		  }
		else
		  {
		  set_pwm_param(PWM_IO1,1500);
		  }


//  扩展板的按键控制电机
//      read_key_io(KEY_IO1,&key1);  
//			if(key1 ==1)
//		{
//			 set_test_motor_current(test_moto_current);
//			}
			
			
}
