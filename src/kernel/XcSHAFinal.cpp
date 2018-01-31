#include "common.h"

/*
 * XcSHAFinal
 *
 * Import Number:      337
 * Calling Convention: stdcall
 * Parameter 0:        PUCHAR pbSHAContext
 * Parameter 1:        PUCHAR pbDigest
 * Return Type:        VOID
 */
int Xbox::XcSHAFinal()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PUCHAR, pbSHAContext);
	K_INIT_ARG(PUCHAR, pbDigest);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
