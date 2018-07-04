#include "main.h"
#define test

int main(void)
{
	BSP_Init();
 	while(1)
	{
#ifdef test
    SV_1_ON();
		SV_2_ON();
		SV_3_ON();
		SV_4_ON();
		delay_ms(2000);
		SV_1_OFF();
		SV_2_OFF();
		SV_3_OFF();
		SV_4_OFF();
		delay_ms(2000);
#endif
	if(can_state_change==1)
	{
		LED_1_TOGGLE();
     if((CanData&0x01)==0x01)
			{
				SV_1_ON();
				sv1_state=1;
			}
			else
      {
				SV_1_OFF();
				sv1_state=0;
			}
			
			if((CanData&0x02)==0x02)
			{
				SV_2_ON();
				sv2_state=1;
			}
			else
      {
				SV_2_OFF();
				sv2_state=0;
			}
			
			if((CanData&0x04)==0x04)
			{
				SV_3_ON();
				sv3_state=1;
			}
			else
      {
				SV_3_OFF();
				sv3_state=0;
			}
			
			if((CanData&0x08)==0x08)
			{
				SV_4_ON();
				sv4_state=1;
			}
			else
      {
				SV_4_OFF();
				sv4_state=0;
			}
			SendSolenoidState();
			can_state_change=0;
		}
		
	}
}
