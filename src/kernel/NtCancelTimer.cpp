#include "common.h"

/*
 * NtCancelTimer
 *
 * Import Number:      185
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE   TimerHandle
 * Parameter 1:        PBOOLEAN CurrentState
 * Return Type:        NTSTATUS
 */
int Xbox::NtCancelTimer()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(HANDLE,   TimerHandle);
	K_INIT_ARG(PBOOLEAN, CurrentState);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
