#include "common.h"

/*
 * RtlRaiseException
 *
 * Import Number:      302
 * Calling Convention: stdcall
 * Parameter 0:        PEXCEPTION_RECORD ExceptionRecord
 * Return Type:        VOID
 */
int Xbox::RtlRaiseException()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(EXCEPTION_RECORD, ExceptionRecord);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
