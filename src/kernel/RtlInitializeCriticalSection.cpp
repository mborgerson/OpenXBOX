#include "common.h"

/*
 * RtlInitializeCriticalSection
 *
 * Import Number:      291
 * Calling Convention: stdcall
 * Parameter 0:        PRTL_CRITICAL_SECTION CriticalSection
 * Return Type:        VOID
 */
int Xbox::RtlInitializeCriticalSection()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_PTR(RTL_CRITICAL_SECTION, CriticalSection);

	// FIXME: let the object manager initialize this
	pCriticalSection->OwningThread = 0;
	pCriticalSection->LockCount = -1;
	pCriticalSection->RecursionCount = 0;
	// TODO: redirect to KeInitializeEvent
	pCriticalSection->Synchronization.Event.Size = sizeof(XboxTypes::KEVENT) / sizeof(XboxTypes::LONG);
	pCriticalSection->Synchronization.Event.Type = XboxTypes::SynchronizationEvent;
	pCriticalSection->Synchronization.Event.SignalState = FALSE;
	InitializeListHead(&pCriticalSection->Synchronization.Event.WaitListHead);

	K_EXIT();
	return KF_OK;
}
