#include "common.h"

/*
 * NtQueryMutant
 *
 * Import Number:      213
 * Calling Convention: stdcall
 * Parameter 0:        HANDLE                    MutantHandle
 * Parameter 1:        PMUTANT_BASIC_INFORMATION MutantInformation
 * Return Type:        NTSTATUS
 */
int Xbox::NtQueryMutant()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(HANDLE,                    MutantHandle);
	K_INIT_ARG(PMUTANT_BASIC_INFORMATION, MutantInformation);
	XboxTypes::NTSTATUS rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
