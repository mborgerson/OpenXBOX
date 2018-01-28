#include "common.h"

/*
 * RtlRip
 *
 * Import Number:      352
 * Calling Convention: stdcall
 * Parameter 0:        PVOID ApiName
 * Parameter 1:        PVOID Expression
 * Parameter 2:        PVOID Message
 * Return Type:        VOID
 */
int Xbox::RtlRip()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID, ApiName);
	K_INIT_ARG_PTR(VOID, Expression);
	K_INIT_ARG_PTR(VOID, Message);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
