#ifndef __CAN_H__
#define __CAN_H__

#include "stm32f10x.h"
#define CAN_BUS_SOLENOID_VALVE_MSG_ID   0x501

void CAN_Configuration(void);
void CanReceiveMsgProcess(CanRxMsg * msg);

extern void SendSolenoidState(void);
extern int can_state_change;
extern int CanData;
#endif
