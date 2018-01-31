#include "common.h"

/*
 * NtQueryEvent
 *
 * Import Number:      209
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE                   EventHandle
 * Parameter 1:        PEVENT_BASIC_INFORMATION EventInformation
 * Return Type:        NTSTATUS
 */
int Xbox::NtQueryEvent()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(HANDLE,                   EventHandle);
	K_INIT_ARG(PEVENT_BASIC_INFORMATION, EventInformation);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
