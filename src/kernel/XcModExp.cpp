#include "common.h"

/*
 * XcModExp
 *
 * Import Number:      345
 * Calling Convention: stdcall
 * Parameter 0:        PULONG pA
 * Parameter 1:        PULONG pB
 * Parameter 2:        PULONG pC
 * Parameter 3:        PULONG pD
 * Parameter 4:        ULONG  dwN
 * Return Type:        ULONG
 */
int Xbox::XcModExp()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(ULONG, pA);
	K_INIT_ARG_PTR(ULONG, pB);
	K_INIT_ARG_PTR(ULONG, pC);
	K_INIT_ARG_PTR(ULONG, pD);
	K_INIT_ARG_VAL(ULONG, dwN);
	XboxTypes::ULONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
