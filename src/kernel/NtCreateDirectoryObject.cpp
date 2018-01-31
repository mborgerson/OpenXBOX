#include "common.h"

/*
 * NtCreateDirectoryObject
 *
 * Import Number:      188
 * Calling Convention: stdcall
 * Parameter 0:        PHANDLE            DirectoryHandle
 * Parameter 1:        POBJECT_ATTRIBUTES ObjectAttributes
 * Return Type:        NTSTATUS
 */
int Xbox::NtCreateDirectoryObject()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PHANDLE,            DirectoryHandle);
	K_INIT_ARG(POBJECT_ATTRIBUTES, ObjectAttributes);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
