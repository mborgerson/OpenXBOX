#include "common.h"

/*
 * AvGetSavedDataAddress
 *
 * Import Number:      1
 * Calling Convention: stdcall
 * Return Type:        PVOID
 */
int Xbox::AvGetSavedDataAddress()
{
	K_ENTER_STDCALL();
	XboxTypes::PVOID rval;

	rval = m_kernel->AvGetSavedDataAddress();

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
