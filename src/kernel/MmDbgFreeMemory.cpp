#include "common.h"

/*
 * MmDbgFreeMemory
 *
 * Import Number:      375
 * Calling Convention: stdcall
 * Parameter 0:        PVOID  BaseAddress
 * Parameter 1:        SIZE_T NumberOfBytes
 * Return Type:        ULONG
 */
int Xbox::MmDbgFreeMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID,   BaseAddress);
	K_INIT_ARG_VAL(SIZE_T, NumberOfBytes);
	XboxTypes::ULONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
