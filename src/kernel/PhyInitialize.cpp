#include "common.h"

/*
 * PhyInitialize
 *
 * Import Number:      253
 * Calling Convention: stdcall
 * Parameter 0:        BOOLEAN forceReset
 * Parameter 1:        PVOID   param
 * Return Type:        NTSTATUS
 */
int Xbox::PhyInitialize()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(BOOLEAN, forceReset);
	K_INIT_ARG_PTR(VOID,    param);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
