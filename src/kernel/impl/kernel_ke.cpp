#include "kernel/impl/kernel.h"
#include "log.h"

XboxTypes::VOID XboxKernel::KeBugCheck(XboxTypes::ULONG BugCheckCode) {
	KeBugCheckEx(BugCheckCode, 0, 0, 0, 0);
}

XboxTypes::VOID XboxKernel::KeBugCheckEx(XboxTypes::ULONG BugCheckCode, XboxTypes::ULONG_PTR BugCheckParameter1, XboxTypes::ULONG_PTR BugCheckParameter2, XboxTypes::ULONG_PTR BugCheckParameter3, XboxTypes::ULONG_PTR BugCheckParameter4) {
	log_fatal("System issued a BugCheck\n");
	log_fatal("  BugCheck code: 0x%x\n", BugCheckCode);
	log_fatal("  Parameter 1  : 0x%x\n", BugCheckParameter1);
	log_fatal("  Parameter 2  : 0x%x\n", BugCheckParameter2);
	log_fatal("  Parameter 3  : 0x%x\n", BugCheckParameter3);
	log_fatal("  Parameter 4  : 0x%x\n", BugCheckParameter4);
}

XboxTypes::BOOLEAN XboxKernel::KeConnectInterrupt(XboxTypes::PKINTERRUPT Interrupt) {
	XboxTypes::KINTERRUPT *pInterrupt = ToPointer<XboxTypes::KINTERRUPT>(Interrupt);
	
	// TODO: implement properly
	if (!pInterrupt->Connected) {
		pInterrupt->Connected = TRUE;
		return TRUE;
	}

	return FALSE;
}

XboxTypes::VOID XboxKernel::KeInitializeApc(XboxTypes::PRKAPC Apc, XboxTypes::PRKTHREAD Thread, XboxTypes::PKKERNEL_ROUTINE KernelRoutine, XboxTypes::PKRUNDOWN_ROUTINE RundownRoutine, XboxTypes::PKNORMAL_ROUTINE NormalRoutine, XboxTypes::KPROCESSOR_MODE ProcessorMode, XboxTypes::PVOID NormalContext) {
	XboxTypes::KAPC *pApc = ToPointer<XboxTypes::KAPC>(Apc);

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
	
	pDeviceQueue->Size = sizeof(XboxTypes::KDEVICE_QUEUE);
	pDeviceQueue->Type = XboxTypes::DeviceQueueObject;
	pDeviceQueue->Busy = FALSE;
	InitializeListHead(&pDeviceQueue->DeviceListHead);
}

XboxTypes::VOID XboxKernel::KeInitializeDpc(XboxTypes::PKDPC Dpc, XboxTypes::PKDEFERRED_ROUTINE DeferredRoutine, XboxTypes::PVOID DeferredContext) {
	XboxTypes::KDPC *pDpc = ToPointer<XboxTypes::KDPC>(Dpc);

	pDpc->Type = XboxTypes::DpcObject;
	pDpc->DeferredRoutine = DeferredRoutine;
	pDpc->DeferredContext = DeferredContext;
	pDpc->Inserted = FALSE;
}

XboxTypes::VOID XboxKernel::KeInitializeEvent(XboxTypes::PRKEVENT Event, XboxTypes::EVENT_TYPE Type, XboxTypes::BOOLEAN State) {
	XboxTypes::KEVENT *pEvent = ToPointer<XboxTypes::KEVENT>(Event);

	pEvent->Header.Size = sizeof(XboxTypes::KEVENT) / sizeof(XboxTypes::LONG);
	pEvent->Header.Type = (XboxTypes::UCHAR)Type;
	pEvent->Header.SignalState = State;
	// FIXME: what can we do about pointers to structs within structs?
	InitializeListHead(&pEvent->Header.WaitListHead);

	//m_objmgr->RegisterEvent(Event, pEvent, Type);
}

XboxTypes::VOID XboxKernel::KeInitializeInterrupt(XboxTypes::PKINTERRUPT Interrupt, XboxTypes::PKSERVICE_ROUTINE ServiceRoutine, XboxTypes::PVOID ServiceContext, XboxTypes::ULONG Vector, XboxTypes::KIRQL Irql, XboxTypes::KINTERRUPT_MODE InterruptMode, XboxTypes::BOOLEAN ShareVector) {
	XboxTypes::KINTERRUPT *pInterrupt = ToPointer<XboxTypes::KINTERRUPT>(Interrupt);

	pInterrupt->ServiceRoutine = ServiceRoutine;
	pInterrupt->ServiceContext = ServiceContext;
	pInterrupt->BusInterruptLevel = Vector - 0x30;
	pInterrupt->Irql = Irql;
	pInterrupt->Mode = InterruptMode;
	pInterrupt->Connected = FALSE;
	// TODO: this is incomplete
}

XboxTypes::VOID XboxKernel::KeInitializeQueue(XboxTypes::PRKQUEUE Queue, XboxTypes::ULONG Count) {
	XboxTypes::KQUEUE *pQueue = ToPointer<XboxTypes::KQUEUE>(Queue);

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

	pSemaphore->Header.Size = sizeof(XboxTypes::KSEMAPHORE) / sizeof(XboxTypes::LONG);
	pSemaphore->Header.Type = XboxTypes::SemaphoreObject;
	pSemaphore->Header.SignalState = Count;
	pSemaphore->Limit = Limit;
	InitializeListHead(&pSemaphore->Header.WaitListHead);

	//m_objmgr->RegisterSemaphore(Semaphore, pSemaphore);
}

XboxTypes::VOID XboxKernel::KeInitializeThread(XboxTypes::PKTHREAD Thread, XboxTypes::PVOID KernelStack, XboxTypes::SIZE_T KernelStackSize, XboxTypes::SIZE_T TlsDataSize, XboxTypes::PKSYSTEM_ROUTINE SystemRoutine, XboxTypes::PKSTART_ROUTINE StartRoutine, XboxTypes::PVOID StartContext, XboxTypes::PKPROCESS Process) {
	XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(Thread);
	pThread->Header.Size = sizeof(XboxTypes::KTHREAD) / sizeof(XboxTypes::LONG);
	pThread->Header.Type = XboxTypes::ThreadObject;
	InitializeListHead(&pThread->Header.WaitListHead);

	InitializeListHead(&pThread->MutantListHead);

	InitializeListHead(&pThread->ApcState.ApcListHead[XboxTypes::KernelMode]);
	InitializeListHead(&pThread->ApcState.ApcListHead[XboxTypes::UserMode]);
	pThread->ApcState.Process = Process;
	pThread->ApcState.ApcQueueable = TRUE;
	// TODO: KeInitializeApc(_PTR_TO_ADDR(KAPC, &pThread->SuspendApc), Thread, <PKKERNEL_ROUTINE f1>, NULL, <PKNORMAL_ROUTINE f2>, XboxTypes::KernelMode, NULL);
	// f1: literally does nothing
	/* f2: does a bit more than nothing:
	PKTHREAD currentThread = pKPCR->PrcbData.CurrentThread;
	KeWaitForSingleObject(&currentThread->SuspendSemaphore, XboxTypes::Suspended, XboxTypes::KernelMode, FALSE, NULL);
	*/

	KeInitializeSemaphore(_PTR_TO_ADDR(KSEMAPHORE, &pThread->SuspendSemaphore), 0, 2);

	pThread->StackBase = KernelStack;
	pThread->StackLimit = KernelStackSize;

	pThread->TlsData = Thread + sizeof(XboxTypes::KTHREAD);
	RtlZeroMemory(pThread->TlsData, TlsDataSize);

	KeInitializeTimerEx(_PTR_TO_ADDR(KTIMER, &pThread->Timer), XboxTypes::NotificationTimer);

	pThread->TimerWaitBlock.Object = _PTR_TO_ADDR(KTIMER, &pThread->Timer);
	pThread->TimerWaitBlock.WaitKey = 0x102;
	pThread->TimerWaitBlock.WaitType = XboxTypes::WaitAny;
	pThread->TimerWaitBlock.Thread = Thread;
	pThread->TimerWaitBlock.WaitListEntry.Flink = _PTR_TO_ADDR(LIST_ENTRY, &pThread->Timer.Header.WaitListHead);
	pThread->TimerWaitBlock.WaitListEntry.Blink = _PTR_TO_ADDR(LIST_ENTRY, &pThread->Timer.Header.WaitListHead);
	// TODO: there are more fields to initialize

	//m_objmgr->RegisterThread(Thread, pThread, Type);
}

XboxTypes::VOID XboxKernel::KeInitializeTimerEx(XboxTypes::PKTIMER Timer, XboxTypes::TIMER_TYPE Type) {
	XboxTypes::KTIMER *pTimer = ToPointer<XboxTypes::KTIMER>(Timer);

	pTimer->Header.Size = sizeof(XboxTypes::KTIMER) / sizeof(XboxTypes::LONG);
	pTimer->Header.Type = XboxTypes::TimerNotificationObject + Type;
	pTimer->Header.Inserted = FALSE;
	pTimer->Header.SignalState = FALSE;
	pTimer->DueTime.QuadPart = 0;
	pTimer->Period = 0;
	InitializeListHead(&pTimer->TimerListEntry);

	//m_objmgr->RegisterTimer(Timer, pTimer, Type);
}

XboxTypes::BOOLEAN XboxKernel::KeSetTimer(XboxTypes::PKTIMER Timer, XboxTypes::LARGE_INTEGER DueTime, XboxTypes::PKDPC Dpc) {
	return KeSetTimerEx(Timer, DueTime, 0, Dpc);
}

XboxTypes::BOOLEAN XboxKernel::KeSetTimerEx(XboxTypes::PKTIMER Timer, XboxTypes::LARGE_INTEGER DueTime, XboxTypes::LONG Period, XboxTypes::PKDPC Dpc) {
	XboxTypes::KTIMER *pTimer = ToPointer<XboxTypes::KTIMER>(Timer);

	pTimer->Header.SignalState = FALSE;
	pTimer->Header.Inserted = TRUE;
	pTimer->Header.Absolute = FALSE;
	pTimer->Dpc = Dpc;
	pTimer->Period = Period;
	pTimer->DueTime.QuadPart = DueTime.QuadPart;
	return TRUE;
}
