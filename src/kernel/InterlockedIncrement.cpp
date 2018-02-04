#include "common.h"

/*
 * InterlockedIncrement
 *
 * Import Number:      53
 * Calling Convention: fastcall
 * Parameter 0:        PLONG Addend
 * Return Type:        LONG
 */
int Xbox::InterlockedIncrement()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(PLONG, Addend);
	XboxTypes::LONG rval;

	rval = m_kernel->InterlockedIncrement(Addend);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
