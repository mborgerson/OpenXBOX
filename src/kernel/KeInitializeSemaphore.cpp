#include "common.h"

/*
 * KeInitializeSemaphore
 *
 * Import Number:      112
 * Calling Convention: stdcall
 * Parameter 0:        PRKSEMAPHORE Semaphore
 * Parameter 1:        LONG         Count
 * Parameter 2:        LONG         Limit
 * Return Type:        VOID
 */
int Xbox::KeInitializeSemaphore()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(PRKSEMAPHORE, Semaphore);
	K_INIT_ARG(LONG,         Count);
	K_INIT_ARG(LONG,         Limit);

	m_kernel->KeInitializeSemaphore(Semaphore, Count, Limit);

	K_EXIT();
	return KF_OK;
}
