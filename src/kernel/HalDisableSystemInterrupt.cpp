#include "common.h"

/*
 * HalDisableSystemInterrupt
 *
 * Import Number:      39
 * Calling Convention: stdcall
 * Parameter 0:        ULONG BusInterruptLevel
 * Return Type:        VOID
 */
int Xbox::HalDisableSystemInterrupt()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG, BusInterruptLevel);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
