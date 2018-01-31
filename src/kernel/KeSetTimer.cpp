#include "common.h"

/*
 * KeSetTimer
 *
 * Import Number:      149
 * Calling Convention: stdcall
 * Parameter 0:        PKTIMER       Timer
 * Parameter 1:        LARGE_INTEGER DueTime
 * Parameter 2:        PKDPC         Dpc
 * Return Type:        BOOLEAN
 */
int Xbox::KeSetTimer()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PKTIMER,       Timer);
	K_INIT_ARG(LARGE_INTEGER, DueTime);
	K_INIT_ARG(PKDPC,         Dpc);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->KeSetTimer(Timer, DueTime, Dpc);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
