#include "common.h"

/*
 * NtWaitForSingleObject
 *
 * Import Number:      233
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE         Handle
 * Parameter 1:        BOOLEAN        Alertable
 * Parameter 2:        PLARGE_INTEGER Timeout
 * Return Type:        NTSTATUS
 */
int Xbox::NtWaitForSingleObject()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE,        Handle);
	K_INIT_ARG_VAL(BOOLEAN,       Alertable);
	K_INIT_ARG_PTR(LARGE_INTEGER, Timeout);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
