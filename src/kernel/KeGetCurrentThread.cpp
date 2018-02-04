#include "common.h"

/*
 * KeGetCurrentThread
 *
 * Import Number:      104
 * Calling Convention: stdcall
 * Return Type:        PKTHREAD
 */
int Xbox::KeGetCurrentThread()
{
	K_ENTER_STDCALL();
	XboxTypes::PKTHREAD rval;

	rval = m_kernel->KeGetCurrentThread();

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
