#include "common.h"

/*
 * XcKeyTable
 *
 * Import Number:      347
 * Calling Convention: stdcall
 * Parameter 0:        ULONG  dwCipher
 * Parameter 1:        PUCHAR pbKeyTable
 * Parameter 2:        PUCHAR pbKey
 * Return Type:        VOID
 */
int Xbox::XcKeyTable()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(ULONG,  dwCipher);
	K_INIT_ARG(PUCHAR, pbKeyTable);
	K_INIT_ARG(PUCHAR, pbKey);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
