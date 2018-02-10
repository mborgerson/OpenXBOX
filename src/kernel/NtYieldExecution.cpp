#include "common.h"

/*
 * NtYieldExecution
 *
 * Import Number:      238
 * Calling Convention: stdcall
 * Return Type:        NTSTATUS
 */
int Xbox::NtYieldExecution()
{
	K_ENTER_STDCALL();
	XboxTypes::NTSTATUS rval;

	rval = m_kernel->NtYieldExecution();

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
