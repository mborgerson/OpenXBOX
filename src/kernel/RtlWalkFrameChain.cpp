#include "common.h"

/*
 * RtlWalkFrameChain
 *
 * Import Number:      319
 * Calling Convention: stdcall
 * Parameter 0:        PPVOID Callers
 * Parameter 1:        ULONG  Count
 * Parameter 2:        ULONG  Flags
 * Return Type:        ULONG
 */
int Xbox::RtlWalkFrameChain()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(PVOID, Callers);
	K_INIT_ARG_VAL(ULONG, Count);
	K_INIT_ARG_VAL(ULONG, Flags);
	XboxTypes::ULONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
