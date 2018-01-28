#include "common.h"

/*
 * NtSignalAndWaitForSingleObjectEx
 *
 * Import Number:      230
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE          SignalHandle
 * Parameter 1:        HANDLE          WaitHandle
 * Parameter 2:        KPROCESSOR_MODE WaitMode
 * Parameter 3:        BOOLEAN         Alertable
 * Parameter 4:        PLARGE_INTEGER  Timeout
 * Return Type:        NTSTATUS
 */
int Xbox::NtSignalAndWaitForSingleObjectEx()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE,          SignalHandle);
	K_INIT_ARG_VAL(HANDLE,          WaitHandle);
	K_INIT_ARG_VAL(KPROCESSOR_MODE, WaitMode);
	K_INIT_ARG_VAL(BOOLEAN,         Alertable);
	K_INIT_ARG_PTR(LARGE_INTEGER,   Timeout);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
