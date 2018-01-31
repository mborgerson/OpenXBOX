#include "common.h"

/*
 * KeReleaseSemaphore
 *
 * Import Number:      132
 * Calling Convention: stdcall
 * Parameter 0:        PRKSEMAPHORE Semaphore
 * Parameter 1:        KPRIORITY    Increment
 * Parameter 2:        LONG         Adjustment
 * Parameter 3:        BOOLEAN      Wait
 * Return Type:        LONG
 */
int Xbox::KeReleaseSemaphore()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRKSEMAPHORE, Semaphore);
	K_INIT_ARG(KPRIORITY,    Increment);
	K_INIT_ARG(LONG,         Adjustment);
	K_INIT_ARG(BOOLEAN,      Wait);
	XboxTypes::LONG rval;

	K_EXIT_WITH_VALUE(rval);
	return KF_ERROR_NOT_IMPLEMENTED;
}
