#include "common.h"

/*
 * KeAlertResumeThread
 *
 * Import Number:      92
 * Calling Convention: stdcall
 * Parameter 0:        PKTHREAD Thread
 * Return Type:        ULONG
 */
int Xbox::KeAlertResumeThread()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PKTHREAD, Thread);
	XboxTypes::ULONG rval;

	rval = m_kernel->KeAlertResumeThread(Thread);

	K_EXIT_WITH_VALUE(rval);
	return KF_OK;
}
