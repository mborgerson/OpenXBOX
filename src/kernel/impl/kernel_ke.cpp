#include "kernel/impl/kernel.h"

XboxTypes::VOID XboxKernel::KeInitializeApc(XboxTypes::PRKAPC Apc, XboxTypes::PRKTHREAD Thread, XboxTypes::PKKERNEL_ROUTINE KernelRoutine, XboxTypes::PKRUNDOWN_ROUTINE RundownRoutine, XboxTypes::PKNORMAL_ROUTINE NormalRoutine, XboxTypes::KPROCESSOR_MODE ProcessorMode, XboxTypes::PVOID NormalContext) {
	XboxTypes::KAPC *pApc = ToPointer<XboxTypes::KAPC>(Apc);

	// FIXME: let the object manager initialize this
	pApc->Type = XboxTypes::ApcObject;
	pApc->Inserted = FALSE;
	pApc->Thread = Thread;

	pApc->KernelRoutine = KernelRoutine;
	pApc->RundownRoutine = RundownRoutine;
	pApc->NormalRoutine = NormalRoutine;
	if (NULL != NormalRoutine) {
		pApc->ApcMode = ProcessorMode;
		pApc->NormalContext = NormalContext;
	}
	else {
		pApc->ApcMode = XboxTypes::KernelMode;
		pApc->NormalContext = NULL;
	}
}

XboxTypes::VOID XboxKernel::KeInitializeDeviceQueue(XboxTypes::PKDEVICE_QUEUE DeviceQueue) {
	XboxTypes::KDEVICE_QUEUE *pDeviceQueue = ToPointer<XboxTypes::KDEVICE_QUEUE>(DeviceQueue);
	
	// FIXME: let the object manager initialize this
	pDeviceQueue->Size = sizeof(XboxTypes::KDEVICE_QUEUE);
	pDeviceQueue->Type = XboxTypes::DeviceQueueObject;
	pDeviceQueue->Busy = FALSE;
	InitializeListHead(&pDeviceQueue->DeviceListHead);
}

XboxTypes::VOID XboxKernel::KeInitializeDpc(XboxTypes::PKDPC Dpc, XboxTypes::PKDEFERRED_ROUTINE DeferredRoutine, XboxTypes::PVOID DeferredContext) {
	XboxTypes::KDPC *pDpc = ToPointer<XboxTypes::KDPC>(Dpc);

	// FIXME: let the object manager initialize this
	pDpc->Type = XboxTypes::DpcObject;
	pDpc->DeferredRoutine = DeferredRoutine;
	pDpc->DeferredContext = DeferredContext;
	pDpc->Inserted = FALSE;
}

XboxTypes::VOID XboxKernel::KeInitializeEvent(XboxTypes::PRKEVENT Event, XboxTypes::EVENT_TYPE Type, XboxTypes::BOOLEAN State) {
	XboxTypes::KEVENT *pEvent = ToPointer<XboxTypes::KEVENT>(Event);

	// FIXME: let the object manager initialize this
	pEvent->Header.Size = sizeof(XboxTypes::KEVENT) / sizeof(XboxTypes::LONG);
	pEvent->Header.Type = (XboxTypes::UCHAR)Type;
	pEvent->Header.SignalState = State;
	// FIXME: what can we do about pointers to structs within structs?
	InitializeListHead(&pEvent->Header.WaitListHead);
}

XboxTypes::VOID XboxKernel::KeInitializeQueue(XboxTypes::PRKQUEUE Queue, XboxTypes::ULONG Count) {
	XboxTypes::KQUEUE *pQueue = ToPointer<XboxTypes::KQUEUE>(Queue);

	// FIXME: let the object manager initialize this
	pQueue->Header.Size = sizeof(XboxTypes::KQUEUE) / sizeof(XboxTypes::LONG);
	pQueue->Header.Type = XboxTypes::QueueObject;
	pQueue->Header.SignalState = 0;
	pQueue->CurrentCount = 0;
	pQueue->MaximumCount = (Count == 0) ? 1 : Count;
	InitializeListHead(&pQueue->Header.WaitListHead);
	InitializeListHead(&pQueue->EntryListHead);
	InitializeListHead(&pQueue->ThreadListHead);
}

XboxTypes::VOID XboxKernel::KeInitializeSemaphore(XboxTypes::PRKSEMAPHORE Semaphore, XboxTypes::LONG Count, XboxTypes::LONG Limit) {
	XboxTypes::KSEMAPHORE *pSemaphore = ToPointer<XboxTypes::KSEMAPHORE>(Semaphore);

	// FIXME: let the object manager initialize this
	pSemaphore->Header.Size = sizeof(XboxTypes::KSEMAPHORE) / sizeof(XboxTypes::LONG);
	pSemaphore->Header.Type = XboxTypes::SemaphoreObject;
	pSemaphore->Header.SignalState = Count;
	pSemaphore->Limit = Limit;
	InitializeListHead(&pSemaphore->Header.WaitListHead);
}

XboxTypes::VOID XboxKernel::KeInitializeTimerEx(XboxTypes::PKTIMER Timer, XboxTypes::TIMER_TYPE Type) {
	XboxTypes::KTIMER *pTimer = ToPointer<XboxTypes::KTIMER>(Timer);

	// FIXME: let the object manager initialize this
	pTimer->Header.Size = sizeof(XboxTypes::KTIMER) / sizeof(XboxTypes::LONG);
	pTimer->Header.Type = XboxTypes::TimerNotificationObject + Type;
	pTimer->Header.Inserted = FALSE;
	pTimer->Header.SignalState = FALSE;
	pTimer->DueTime.QuadPart = 0;
	pTimer->Period = 0;
	InitializeListHead(&pTimer->TimerListEntry);
}

XboxTypes::BOOLEAN XboxKernel::KeSetTimer(XboxTypes::PKTIMER Timer, XboxTypes::LARGE_INTEGER DueTime, XboxTypes::PKDPC Dpc) {
	return KeSetTimerEx(Timer, DueTime, 0, Dpc);
}

XboxTypes::BOOLEAN XboxKernel::KeSetTimerEx(XboxTypes::PKTIMER Timer, XboxTypes::LARGE_INTEGER DueTime, XboxTypes::LONG Period, XboxTypes::PKDPC Dpc) {
	XboxTypes::KTIMER *pTimer = ToPointer<XboxTypes::KTIMER>(Timer);

	// TODO: let the object manager initialize this
	pTimer->Header.SignalState = FALSE;
	pTimer->Header.Inserted = TRUE;
	pTimer->Header.Absolute = FALSE;
	pTimer->Dpc = Dpc;
	pTimer->Period = Period;
	pTimer->DueTime.QuadPart = DueTime.QuadPart;
	return TRUE;
}