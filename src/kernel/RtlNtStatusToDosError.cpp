#include "common.h"
#include "kernel/impl/error.h"

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
	K_INIT_ARG_VAL(NTSTATUS, Status);
	XboxTypes::ULONG rval;

	rval = NTSTATUStoDOSerror(Status);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
