#include "common.h"

/*
 * XcBlockCryptCBC
 *
 * Import Number:      349
 * Calling Convention: stdcall
 * Parameter 0:        ULONG  dwCipher
 * Parameter 1:        ULONG  dwInputLength
 * Parameter 2:        PUCHAR pbOutput
 * Parameter 3:        PUCHAR pbInput
 * Parameter 4:        PUCHAR pbKeyTable
 * Parameter 5:        ULONG  dwOp
 * Parameter 6:        PUCHAR pbFeedback
 * Return Type:        VOID
 */
int Xbox::XcBlockCryptCBC()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(ULONG, dwCipher);
	K_INIT_ARG_VAL(ULONG, dwInputLength);
	K_INIT_ARG_PTR(UCHAR, pbOutput);
	K_INIT_ARG_PTR(UCHAR, pbInput);
	K_INIT_ARG_PTR(UCHAR, pbKeyTable);
	K_INIT_ARG_VAL(ULONG, dwOp);
	K_INIT_ARG_PTR(UCHAR, pbFeedback);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
