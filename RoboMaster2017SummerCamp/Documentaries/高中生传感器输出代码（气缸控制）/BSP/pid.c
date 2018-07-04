#include "main.h"

//普通位置式的PID运算
void CalPID(ST_PID *stpid)
{
	stpid->E = stpid->ref - stpid->fdb;
	stpid->ESum += stpid->E;
	
	stpid->out = stpid->E * stpid->P + stpid->ESum * stpid->I + (stpid->E - stpid->PreE) * stpid->D;
	
	if(stpid->out >= stpid->out_limit)
		  stpid->out=stpid->out_limit;
	else if(stpid->out <= -stpid->out_limit)
		  stpid->out=-stpid->out_limit;
	
	stpid->PrePreE = stpid->PreE;
	stpid->PreE = stpid->E;
}

//速度增量式PID
void CalPID_Increment (ST_PID *stpid)
{
  
	stpid->PrePreE = stpid->PreE;
	stpid->PreE = stpid->E;
	stpid->E = stpid->ref - stpid->fdb;
	
	stpid->out += (stpid->E-stpid->PreE) * stpid->P + stpid->E * stpid->I + (stpid->E - 2*stpid->PreE + stpid->PrePreE) * stpid->D;
	
	if(stpid->out >= stpid->out_limit)
		  stpid->out=stpid->out_limit;
	else if(stpid->out <= -stpid->out_limit)
		  stpid->out=-stpid->out_limit;
	
}
