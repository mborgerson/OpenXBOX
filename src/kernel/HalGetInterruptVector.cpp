#include "common.h"

/*
 * HalGetInterruptVector
 *
 * Import Number:      44
 * Calling Convention: stdcall
 * Parameter 0:        ULONG  BusInterruptLevel
 * Parameter 1:        PKIRQL Irql
 * Return Type:        ULONG
 */
int Xbox::HalGetInterruptVector()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG, BusInterruptLevel);
	K_INIT_ARG_PTR(KIRQL, Irql);
	XboxTypes::ULONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
