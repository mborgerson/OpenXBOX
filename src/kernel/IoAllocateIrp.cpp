#include "common.h"

/*
 * IoAllocateIrp
 *
 * Import Number:      59
 * Calling Convention: stdcall
 * Parameter 0:        CCHAR StackSize
 * Return Type:        PIRP
 */
int Xbox::IoAllocateIrp()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(CCHAR, StackSize);
	XboxTypes::PIRP rval;

	rval = m_kernel->IoAllocateIrp(StackSize);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
