#include "common.h"

/*
 * KeSetDisableBoostThread
 *
 * Import Number:      144
 * Calling Convention: stdcall
 * Parameter 0:        PKTHREAD Thread
 * Parameter 1:        LOGICAL  Disable
 * Return Type:        LOGICAL
 */
int Xbox::KeSetDisableBoostThread()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PKTHREAD, Thread);
	K_INIT_ARG(LOGICAL,  Disable);
	XboxTypes::LOGICAL rval;

	rval = m_kernel->KeSetDisableBoostThread(Thread, Disable);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_PARTIAL_IMPL;
}
