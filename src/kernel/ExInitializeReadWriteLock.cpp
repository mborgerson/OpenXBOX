#include "common.h"

/*
 * ExInitializeReadWriteLock
 *
 * Import Number:      18
 * Calling Convention: stdcall
 * Parameter 0:        PERWLOCK ReadWriteLock
 * Return Type:        VOID
 */
int Xbox::ExInitializeReadWriteLock()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(ERWLOCK, ReadWriteLock);

	K_EXIT();
	return KF_ERROR_NOT_IMPLEMENTED;
}
