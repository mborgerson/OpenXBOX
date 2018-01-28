#include "common.h"

/*
 * NtClose
 *
 * Import Number:      187
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE Handle
 * Return Type:        NTSTATUS
 */
int Xbox::NtClose()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE, Handle);
	XboxTypes::NTSTATUS rval;

	// TODO: tell the object manager to close the handle
	rval = 0;

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
