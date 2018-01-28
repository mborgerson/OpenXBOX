#include "common.h"

/*
 * RtlUnwind
 *
 * Import Number:      312
 * Calling Convention: stdcall
 * Parameter 0:        PVOID             TargetFrame
 * Parameter 1:        PVOID             TargetIp
 * Parameter 2:        PEXCEPTION_RECORD ExceptionRecord
 * Parameter 3:        PVOID             ReturnValue
 * Return Type:        VOID
 */
int Xbox::RtlUnwind()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,             TargetFrame);
	K_INIT_ARG_PTR(VOID,             TargetIp);
	K_INIT_ARG_PTR(EXCEPTION_RECORD, ExceptionRecord);
	K_INIT_ARG_PTR(VOID,             ReturnValue);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
