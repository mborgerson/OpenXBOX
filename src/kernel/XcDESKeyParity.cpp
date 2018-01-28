#include "common.h"

/*
 * XcDESKeyParity
 *
 * Import Number:      346
 * Calling Convention: stdcall
 * Parameter 0:        PUCHAR pbKey
 * Parameter 1:        ULONG  dwKeyLength
 * Return Type:        VOID
 */
int Xbox::XcDESKeyParity()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(UCHAR, pbKey);
	K_INIT_ARG_VAL(ULONG, dwKeyLength);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
