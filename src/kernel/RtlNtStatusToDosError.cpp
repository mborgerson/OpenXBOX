#include "common.h"

/*
 * RtlNtStatusToDosError
 *
 * Import Number:      301
 * Calling Convention: stdcall
 * Parameter 0:        NTSTATUS Status
 * Return Type:        ULONG
 */
int Xbox::RtlNtStatusToDosError()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(NTSTATUS, Status);
	XboxTypes::ULONG rval;

	rval = m_kernel->RtlNtStatusToDosError(Status);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
