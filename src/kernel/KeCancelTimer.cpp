#include "common.h"

/*
 * KeCancelTimer
 *
 * Import Number:      97
 * Calling Convention: stdcall
 * Parameter 0:        PKTIMER Timer
 * Return Type:        BOOLEAN
 */
int Xbox::KeCancelTimer()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PKTIMER, Timer);
	XboxTypes::BOOLEAN rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
