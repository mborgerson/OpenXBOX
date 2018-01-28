#include "common.h"

/*
 * XcSHAUpdate
 *
 * Import Number:      336
 * Calling Convention: stdcall
 * Parameter 0:        PUCHAR pbSHAContext
 * Parameter 1:        PUCHAR pbInput
 * Parameter 2:        ULONG  dwInputLength
 * Return Type:        VOID
 */
int Xbox::XcSHAUpdate()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(UCHAR, pbSHAContext);
	K_INIT_ARG_PTR(UCHAR, pbInput);
	K_INIT_ARG_VAL(ULONG, dwInputLength);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
