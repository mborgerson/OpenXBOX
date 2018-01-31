#include "common.h"

/*
 * ExQueryPoolBlockSize
 *
 * Import Number:      23
 * Calling Convention: stdcall
 * Parameter 0:        PVOID PoolBlock
 * Return Type:        ULONG
 */
int Xbox::ExQueryPoolBlockSize()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PVOID, PoolBlock);
	XboxTypes::ULONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
