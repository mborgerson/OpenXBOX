#include "common.h"

/*
 * MmIsAddressValid
 *
 * Import Number:      174
 * Calling Convention: stdcall
 * Parameter 0:        PVOID VirtualAddress
 * Return Type:        BOOLEAN
 */
int Xbox::MmIsAddressValid()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PVOID, VirtualAddress);
	XboxTypes::BOOLEAN rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
