#include "common.h"

/*
 * KeSetTimerEx
 *
 * Import Number:      150
 * Calling Convention: stdcall
 * Parameter 0:        PKTIMER       Timer
 * Parameter 1:        LARGE_INTEGER DueTime
 * Parameter 2:        LONG          Period
 * Parameter 3:        PKDPC         Dpc
 * Return Type:        BOOLEAN
 */
int Xbox::KeSetTimerEx()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(PKTIMER,       Timer);
	K_INIT_ARG_VAL(LARGE_INTEGER, DueTime);
	K_INIT_ARG_VAL(LONG,          Period);
	K_INIT_ARG_VAL(PKDPC,         Dpc);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->KeSetTimerEx(Timer, DueTime, Period, Dpc);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
