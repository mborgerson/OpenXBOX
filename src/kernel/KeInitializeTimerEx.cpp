#include "common.h"

/*
 * KeInitializeTimerEx
 *
 * Import Number:      113
 * Calling Convention: stdcall
 * Parameter 0:        PKTIMER    Timer
 * Parameter 1:        TIMER_TYPE Type
 * Return Type:        VOID
 */
int Xbox::KeInitializeTimerEx()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(PKTIMER,    Timer);
	K_INIT_ARG_VAL(TIMER_TYPE, Type);

	m_kernel->KeInitializeTimerEx(Timer, Type);

	K_EXIT();
	return KF_OK;
}
