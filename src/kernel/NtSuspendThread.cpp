#include "common.h"

/*
 * NtSuspendThread
 *
 * Import Number:      231
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE ThreadHandle
 * Parameter 1:        PULONG PreviousSuspendCount
 * Return Type:        NTSTATUS
 */
int Xbox::NtSuspendThread()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(HANDLE, ThreadHandle);
	K_INIT_ARG(PULONG, PreviousSuspendCount);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
