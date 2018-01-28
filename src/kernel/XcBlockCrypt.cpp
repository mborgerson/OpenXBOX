#include "common.h"

/*
 * XcBlockCrypt
 *
 * Import Number:      348
 * Calling Convention: stdcall
 * Parameter 0:        ULONG  dwCipher
 * Parameter 1:        PUCHAR pbOutput
 * Parameter 2:        PUCHAR pbInput
 * Parameter 3:        PUCHAR pbKeyTable
 * Parameter 4:        ULONG  dwOp
 * Return Type:        VOID
 */
int Xbox::XcBlockCrypt()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG, dwCipher);
	K_INIT_ARG_PTR(UCHAR, pbOutput);
	K_INIT_ARG_PTR(UCHAR, pbInput);
	K_INIT_ARG_PTR(UCHAR, pbKeyTable);
	K_INIT_ARG_VAL(ULONG, dwOp);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
