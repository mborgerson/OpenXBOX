#include "common.h"

/*
 * HalInitiateShutdown
 *
 * Import Number:      360
 * Calling Convention: stdcall
 * Return Type:        VOID
 */
int Xbox::HalInitiateShutdown()
{
	K_ENTER_STDCALL();

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
