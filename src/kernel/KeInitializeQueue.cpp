#include "common.h"

/*
 * KeInitializeQueue
 *
 * Import Number:      111
 * Calling Convention: stdcall
 * Parameter 0:        PRKQUEUE Queue
 * Parameter 1:        ULONG    Count
 * Return Type:        VOID
 */
int Xbox::KeInitializeQueue()
{
	K_ENTER_STDCALL();
	K_INIT_ARG_RPT(KQUEUE, Queue);
	K_INIT_ARG_VAL(ULONG,  Count);

	// FIXME: let the object manager initialize this
	pQueue->Header.Size = sizeof(XboxTypes::KQUEUE) / sizeof(XboxTypes::LONG);
	pQueue->Header.Type = XboxTypes::QueueObject;
	pQueue->Header.SignalState = 0;
	pQueue->CurrentCount = 0;
	pQueue->MaximumCount = (Count == 0) ? 1 : Count;
	InitializeListHead(&pQueue->Header.WaitListHead);
	InitializeListHead(&pQueue->EntryListHead);
	InitializeListHead(&pQueue->ThreadListHead);

	K_EXIT();
	return KF_OK;
}
