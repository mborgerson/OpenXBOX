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
	K_INIT_ARG_RPT(KSEMAPHORE, Semaphore);
	K_INIT_ARG_VAL(LONG,       Count);
	K_INIT_ARG_VAL(LONG,       Limit);

	// FIXME: let the object manager initialize this
	pSemaphore->Header.Size = sizeof(XboxTypes::KSEMAPHORE) / sizeof(XboxTypes::LONG);
	pSemaphore->Header.Type = XboxTypes::SemaphoreObject;
	pSemaphore->Header.SignalState = Count;
	pSemaphore->Limit = Limit;
	InitializeListHead(&pSemaphore->Header.WaitListHead);

	K_EXIT();
	return KF_OK;
}
