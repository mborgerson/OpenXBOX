#include "common.h"

/*
 * ExAllocatePool
 *
 * Import Number:      14
 * Calling Convention: stdcall
 * Parameter 0:        SIZE_T NumberOfBytes
 * Return Type:        PVOID
 */
int Xbox::ExAllocatePool()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(SIZE_T, NumberOfBytes);
	XboxTypes::PVOID rval;

	rval = m_kernel->ExAllocatePool(NumberOfBytes);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
