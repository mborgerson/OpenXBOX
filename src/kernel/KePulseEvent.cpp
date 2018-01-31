#include "common.h"

/*
 * KePulseEvent
 *
 * Import Number:      123
 * Calling Convention: stdcall
 * Parameter 0:        PRKEVENT  Event
 * Parameter 1:        KPRIORITY Increment
 * Parameter 2:        BOOLEAN   Wait
 * Return Type:        LONG
 */
int Xbox::KePulseEvent()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRKEVENT,  Event);
	K_INIT_ARG(KPRIORITY, Increment);
	K_INIT_ARG(BOOLEAN,   Wait);
	XboxTypes::LONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
