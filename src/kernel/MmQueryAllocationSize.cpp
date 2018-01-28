#include "common.h"

/*
 * MmQueryAllocationSize
 *
 * Import Number:      180
 * Calling Convention: stdcall
 * Parameter 0:        PVOID BaseAddress
 * Return Type:        SIZE_T
 */
int Xbox::MmQueryAllocationSize()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(VOID, BaseAddress);
	XboxTypes::SIZE_T rval;

	rval = m_memmgr->QueryAllocationSize(BaseAddress);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
