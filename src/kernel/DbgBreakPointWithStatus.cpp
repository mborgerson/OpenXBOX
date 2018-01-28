#include "common.h"

/*
 * DbgBreakPointWithStatus
 *
 * Import Number:      6
 * Calling Convention: stdcall
 * Parameter 0:        ULONG Status
 * Return Type:        VOID
 */
int Xbox::DbgBreakPointWithStatus()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG, Status);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
