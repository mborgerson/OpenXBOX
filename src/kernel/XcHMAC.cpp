#include "common.h"

/*
 * XcHMAC
 *
 * Import Number:      340
 * Calling Convention: stdcall
 * Parameter 0:        PUCHAR pbKey
 * Parameter 1:        ULONG  dwKeyLength
 * Parameter 2:        PUCHAR pbInput
 * Parameter 3:        ULONG  dwInputLength
 * Parameter 4:        PUCHAR pbInput2
 * Parameter 5:        ULONG  dwInputLength2
 * Parameter 6:        PUCHAR pbDigest
 * Return Type:        VOID
 */
int Xbox::XcHMAC()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(UCHAR, pbKey);
	K_INIT_ARG_VAL(ULONG, dwKeyLength);
	K_INIT_ARG_PTR(UCHAR, pbInput);
	K_INIT_ARG_VAL(ULONG, dwInputLength);
	K_INIT_ARG_PTR(UCHAR, pbInput2);
	K_INIT_ARG_VAL(ULONG, dwInputLength2);
	K_INIT_ARG_PTR(UCHAR, pbDigest);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
