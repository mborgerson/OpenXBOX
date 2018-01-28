#include "common.h"

/*
 * DbgBreakPoint
 *
 * Import Number:      5
 * Calling Convention: stdcall
 * Return Type:        VOID
 */
int Xbox::DbgBreakPoint()
{
	K_ENTER_STDCALL();

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
