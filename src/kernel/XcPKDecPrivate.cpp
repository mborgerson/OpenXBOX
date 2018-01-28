#include "common.h"

/*
 * XcPKDecPrivate
 *
 * Import Number:      342
 * Calling Convention: stdcall
 * Parameter 0:        PUCHAR pbPrvKey
 * Parameter 1:        PUCHAR pbInput
 * Parameter 2:        PUCHAR pbOutput
 * Return Type:        ULONG
 */
int Xbox::XcPKDecPrivate()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(UCHAR, pbPrvKey);
	K_INIT_ARG_PTR(UCHAR, pbInput);
	K_INIT_ARG_PTR(UCHAR, pbOutput);
	XboxTypes::ULONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
