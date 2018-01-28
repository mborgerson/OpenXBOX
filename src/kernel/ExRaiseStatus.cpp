#include "common.h"

/*
 * ExRaiseStatus
 *
 * Import Number:      27
 * Calling Convention: stdcall
 * Parameter 0:        NTSTATUS Status
 * Return Type:        VOID
 */
int Xbox::ExRaiseStatus()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_VAL(NTSTATUS, Status);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
