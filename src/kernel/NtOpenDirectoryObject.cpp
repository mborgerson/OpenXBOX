#include "common.h"

/*
 * NtOpenDirectoryObject
 *
 * Import Number:      201
 * Calling Convention: stdcall
 * Parameter 0:        PHANDLE            DirectoryHandle
 * Parameter 1:        POBJECT_ATTRIBUTES ObjectAttributes
 * Return Type:        NTSTATUS
 */
int Xbox::NtOpenDirectoryObject()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(HANDLE,            DirectoryHandle);
	K_INIT_ARG_PTR(OBJECT_ATTRIBUTES, ObjectAttributes);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
