#include "common.h"

/*
 * NtDuplicateObject
 *
 * Import Number:      197
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE  SourceHandle
 * Parameter 1:        PHANDLE TargetHandle
 * Parameter 2:        ULONG   Options
 * Return Type:        NTSTATUS
 */
int Xbox::NtDuplicateObject()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE, SourceHandle);
	K_INIT_ARG_PTR(HANDLE, TargetHandle);
	K_INIT_ARG_VAL(ULONG,  Options);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
