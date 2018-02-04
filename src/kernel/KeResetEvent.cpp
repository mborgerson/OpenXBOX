#include "common.h"

/*
 * KeResetEvent
 *
 * Import Number:      138
 * Calling Convention: stdcall
 * Parameter 0:        PRKEVENT Event
 * Return Type:        LONG
 */
int Xbox::KeResetEvent()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRKEVENT, Event);
	XboxTypes::LONG rval;

	rval = m_kernel->KeResetEvent(Event);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
