#include "common.h"

/*
 * NtReleaseMutant
 *
 * Import Number:      221
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE MutantHandle
 * Parameter 1:        PLONG  PreviousCount
 * Return Type:        NTSTATUS
 */
int Xbox::NtReleaseMutant()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(HANDLE, MutantHandle);
	K_INIT_ARG_PTR(LONG,   PreviousCount);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
