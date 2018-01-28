#include "common.h"

/*
 * KeSetEvent
 *
 * Import Number:      145
 * Calling Convention: stdcall
 * Parameter 0:        PRKEVENT  Event
 * Parameter 1:        KPRIORITY Increment
 * Parameter 2:        BOOLEAN   Wait
 * Return Type:        LONG
 */
int Xbox::KeSetEvent()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_RPT(KEVENT,    Event);
	K_INIT_ARG_VAL(KPRIORITY, Increment);
	K_INIT_ARG_VAL(BOOLEAN,   Wait);
	XboxTypes::LONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
