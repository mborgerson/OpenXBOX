#include "common.h"

/*
 * KeDisconnectInterrupt
 *
 * Import Number:      100
 * Calling Convention: stdcall
 * Parameter 0:        PKINTERRUPT Interrupt
 * Return Type:        BOOLEAN
 */
int Xbox::KeDisconnectInterrupt()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(KINTERRUPT, Interrupt);
	XboxTypes::BOOLEAN rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
