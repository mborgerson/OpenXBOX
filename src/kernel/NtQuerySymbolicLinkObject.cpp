#include "common.h"

/*
 * NtQuerySymbolicLinkObject
 *
 * Import Number:      215
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE         LinkHandle
 * Parameter 1:        POBJECT_STRING LinkTarget
 * Parameter 2:        PULONG         ReturnedLength
 * Return Type:        NTSTATUS
 */
int Xbox::NtQuerySymbolicLinkObject()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE,        LinkHandle);
	K_INIT_ARG_PTR(OBJECT_STRING, LinkTarget);
	K_INIT_ARG_PTR(ULONG,         ReturnedLength);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
