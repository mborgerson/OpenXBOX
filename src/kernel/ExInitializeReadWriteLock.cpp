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
	K_INIT_ARG(PERWLOCK, ReadWriteLock);

	m_kernel->ExInitializeReadWriteLock(ReadWriteLock);

	K_EXIT();
	return KF_OK;
}
