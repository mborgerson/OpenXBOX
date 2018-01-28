#include "common.h"

/*
 * KeInitializeInterrupt
 *
 * Import Number:      109
 * Calling Convention: stdcall
 * Parameter 0:        PKINTERRUPT       Interrupt
 * Parameter 1:        PKSERVICE_ROUTINE ServiceRoutine
 * Parameter 2:        PVOID             ServiceContext
 * Parameter 3:        ULONG             Vector
 * Parameter 4:        KIRQL             Irql
 * Parameter 5:        KINTERRUPT_MODE   InterruptMode
 * Parameter 6:        BOOLEAN           ShareVector
 * Return Type:        VOID
 */
int Xbox::KeInitializeInterrupt()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(KINTERRUPT,        Interrupt);
	K_INIT_ARG_VAL(PKSERVICE_ROUTINE, ServiceRoutine);
	K_INIT_ARG_PTR(VOID,              ServiceContext);
	K_INIT_ARG_VAL(ULONG,             Vector);
	K_INIT_ARG_VAL(KIRQL,             Irql);
	K_INIT_ARG_VAL(KINTERRUPT_MODE,   InterruptMode);
	K_INIT_ARG_VAL(BOOLEAN,           ShareVector);

	// FIXME: let the object manager initialize this


	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
