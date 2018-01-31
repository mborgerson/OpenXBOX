#include "common.h"

/*
 * XcPKGetKeyLen
 *
 * Import Number:      343
 * Calling Convention: stdcall
 * Parameter 0:        PUCHAR pbPubKey
 * Return Type:        ULONG
 */
int Xbox::XcPKGetKeyLen()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PUCHAR, pbPubKey);
	XboxTypes::ULONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
