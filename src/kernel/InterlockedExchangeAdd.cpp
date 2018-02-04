#include "common.h"

/*
 * InterlockedExchangeAdd
 *
 * Import Number:      55
 * Calling Convention: fastcall
 * Parameter 0:        PLONG Addend
 * Parameter 1:        LONG  Increment
 * Return Type:        LONG
 */
int Xbox::InterlockedExchangeAdd()
{
	K_ENTER_FASTCALL();
	K_INIT_ARG(PLONG, Addend);
	K_INIT_ARG(LONG,  Increment);
	XboxTypes::LONG rval;

	rval = m_kernel->InterlockedExchangeAdd(Addend, Increment);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
