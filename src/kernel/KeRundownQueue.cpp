#include "common.h"

/*
 * KeRundownQueue
 *
 * Import Number:      141
 * Calling Convention: stdcall
 * Parameter 0:        PRKQUEUE Queue
 * Return Type:        PLIST_ENTRY
 */
int Xbox::KeRundownQueue()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_RPT(KQUEUE, Queue);
	XboxTypes::PLIST_ENTRY rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
