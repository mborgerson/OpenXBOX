#include "common.h"

/*
 * KeQueryBasePriorityThread
 *
 * Import Number:      124
 * Calling Convention: stdcall
 * Parameter 0:        PKTHREAD Thread
 * Return Type:        LONG
 */
int Xbox::KeQueryBasePriorityThread()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(KTHREAD, Thread);
	XboxTypes::LONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
