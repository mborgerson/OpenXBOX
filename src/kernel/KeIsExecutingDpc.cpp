#include "common.h"

/*
 * KeIsExecutingDpc
 *
 * Import Number:      121
 * Calling Convention: stdcall
 * Return Type:        ULONG
 */
int Xbox::KeIsExecutingDpc()
{
	K_ENTER_STDCALL();
	XboxTypes::ULONG rval;

	rval = m_kernel->KeIsExecutingDpc();

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
