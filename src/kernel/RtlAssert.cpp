#include "common.h"

/*
 * RtlAssert
 *
 * Import Number:      264
 * Calling Convention: stdcall
 * Parameter 0:        PVOID FailedAssertion
 * Parameter 1:        PVOID FileName
 * Parameter 2:        ULONG LineNumber
 * Parameter 3:        PCHAR Message
 * Return Type:        VOID
 */
int Xbox::RtlAssert()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,  FailedAssertion);
	K_INIT_ARG_PTR(VOID,  FileName);
	K_INIT_ARG_VAL(ULONG, LineNumber);
	K_INIT_ARG_PTR(CHAR,  Message);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
