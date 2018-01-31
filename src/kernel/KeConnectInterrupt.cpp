#include "common.h"

/*
 * KeConnectInterrupt
 *
 * Import Number:      98
 * Calling Convention: stdcall
 * Parameter 0:        PKINTERRUPT Interrupt
 * Return Type:        BOOLEAN
 */
int Xbox::KeConnectInterrupt()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PKINTERRUPT, Interrupt);
	XboxTypes::BOOLEAN rval;

	rval = m_kernel->KeConnectInterrupt(Interrupt);

	K_EXIT_WITH_VALUE(rval);
	return KF_WARN_FAKE_IMPL;
}
