#include "common.h"

/*
 * XcVerifyPKCS1Signature
 *
 * Import Number:      344
 * Calling Convention: stdcall
 * Parameter 0:        PUCHAR pbSig
 * Parameter 1:        PUCHAR pbPubKey
 * Parameter 2:        PUCHAR pbDigest
 * Return Type:        BOOLEAN
 */
int Xbox::XcVerifyPKCS1Signature()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PUCHAR, pbSig);
	K_INIT_ARG(PUCHAR, pbPubKey);
	K_INIT_ARG(PUCHAR, pbDigest);
	XboxTypes::BOOLEAN rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
