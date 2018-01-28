#include "common.h"

/*
 * KeSetEventBoostPriority
 *
 * Import Number:      146
 * Calling Convention: stdcall
 * Parameter 0:        PRKEVENT   Event
 * Parameter 1:        PPRKTHREAD Thread
 * Return Type:        VOID
 */
int Xbox::KeSetEventBoostPriority()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_RPT(KEVENT,    Event);
	K_INIT_ARG_PTR(PRKTHREAD, Thread);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
