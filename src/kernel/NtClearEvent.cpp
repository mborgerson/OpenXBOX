#include "common.h"

/*
 * NtClearEvent
 *
 * Import Number:      186
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE EventHandle
 * Return Type:        NTSTATUS
 */
int Xbox::NtClearEvent()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE, EventHandle);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
