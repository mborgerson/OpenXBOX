#include "common.h"

/*
 * HalEnableSystemInterrupt
 *
 * Import Number:      43
 * Calling Convention: stdcall
 * Parameter 0:        ULONG           BusInterruptLevel
 * Parameter 1:        KINTERRUPT_MODE InterruptMode
 * Return Type:        VOID
 */
int Xbox::HalEnableSystemInterrupt()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG,           BusInterruptLevel);
	K_INIT_ARG_VAL(KINTERRUPT_MODE, InterruptMode);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
