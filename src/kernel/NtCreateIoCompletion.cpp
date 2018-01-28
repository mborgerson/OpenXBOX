#include "common.h"

/*
 * NtCreateIoCompletion
 *
 * Import Number:      191
 * Calling Convention: stdcall
 * Parameter 0:        PHANDLE            IoCompletionHandle
 * Parameter 1:        ACCESS_MASK        DesiredAccess
 * Parameter 2:        POBJECT_ATTRIBUTES ObjectAttributes
 * Parameter 3:        ULONG              Count
 * Return Type:        NTSTATUS
 */
int Xbox::NtCreateIoCompletion()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(HANDLE,            IoCompletionHandle);
	K_INIT_ARG_VAL(ACCESS_MASK,       DesiredAccess);
	K_INIT_ARG_PTR(OBJECT_ATTRIBUTES, ObjectAttributes);
	K_INIT_ARG_VAL(ULONG,             Count);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
