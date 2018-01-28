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
	K_INIT_ARG_PTR(KTIMER,        Timer);
	K_INIT_ARG_VAL(LARGE_INTEGER, DueTime);
	K_INIT_ARG_VAL(LONG,          Period);
	K_INIT_ARG_PTR(KDPC,          Dpc);
	XboxTypes::BOOLEAN rval;

	pTimer->Header.SignalState = FALSE;
	pTimer->Header.Inserted = TRUE;
	pTimer->Header.Absolute = FALSE;
	pTimer->Dpc = Dpc;
	pTimer->Period = Period;
	pTimer->DueTime.QuadPart = DueTime.QuadPart;
	rval = TRUE;

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
