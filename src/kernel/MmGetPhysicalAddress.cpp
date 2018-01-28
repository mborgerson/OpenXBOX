#include "common.h"

/*
 * MmGetPhysicalAddress
 *
 * Import Number:      173
 * Calling Convention: stdcall
 * Parameter 0:        PVOID BaseAddress
 * Return Type:        ULONG_PTR
 */
int Xbox::MmGetPhysicalAddress()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID, BaseAddress);
	XboxTypes::ULONG_PTR rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
