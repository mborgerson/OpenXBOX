#include "kernel/impl/kernel.h"
#include "log.h"

#define KiLockDispatcherDatabase(oldIRQL) \
	*(oldIRQL) = KeRaiseIrqlToDpcLevel();

XboxTypes::VOID XboxKernel::KiUnlockDispatcherDatabase(XboxTypes::KIRQL OldIrql) {
	if (m_pKPCR->PrcbData.NextThread == NULL) {
		// FIXME: this should be KfLowerIrql(oldIRQL);
		m_pKPCR->Irql = OldIrql;
		return;
	}

	if (OldIrql >= DISPATCH_LEVEL) {
		if (m_pKPCR->PrcbData.DpcRoutineActive == NULL) {
			// TODO: HalRequestSoftwareInterrupt(DISPATCH_LEVEL);
		}

		// FIXME: this should be KfLowerIrql(oldIRQL);
		m_pKPCR->Irql = OldIrql;
		return;
	}

	XboxTypes::KTHREAD *pCurrThread = ToPointer<XboxTypes::KTHREAD>(m_pKPCR->PrcbData.CurrentThread);
	XboxTypes::KIRQL prevIrql = pCurrThread->WaitIrql;
	m_pKPCR->PrcbData.CurrentThread = m_pKPCR->PrcbData.NextThread;
	m_pKPCR->PrcbData.NextThread = NULL;
	// TODO: tell scheduler that the current thread has changed
	// FIXME: this is incomplete
	// At this point, pending APCs (if any) should run at IRQL = APC_LEVEL

	// FIXME: this should be KfLowerIrql(oldIRQL);
	m_pKPCR->Irql = OldIrql;
	return;
}


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

XboxTypes::VOID XboxKernel::KeEnterCriticalRegion() {
	XboxTypes::PKTHREAD thread = KeGetCurrentThread();
	XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(thread);
	
	pThread->KernelApcDisable--;
}

XboxTypes::KIRQL XboxKernel::KeGetCurrentIrql() {
	return m_pKPCR->Irql;
}

XboxTypes::PKTHREAD XboxKernel::KeGetCurrentThread() {
	return m_pKPCR->PrcbData.CurrentThread;
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

XboxTypes::VOID XboxKernel::KeInitializeMutant(XboxTypes::PRKMUTANT Mutant, XboxTypes::BOOLEAN InitialOwner) {
	XboxTypes::KMUTANT *pMutant = ToPointer<XboxTypes::KMUTANT>(Mutant);

	pMutant->Header.Size = sizeof(XboxTypes::KMUTANT) / sizeof(XboxTypes::LONG);
	pMutant->Header.Type = XboxTypes::MutantObject;
	pMutant->Abandoned = FALSE;
	InitializeListHead(&pMutant->Header.WaitListHead);

	if (InitialOwner == TRUE) {
		XboxTypes::PKTHREAD thread = KeGetCurrentThread();
		XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(thread);
		pMutant->Header.SignalState = 0;
		pMutant->OwnerThread = thread;

		XboxTypes::KIRQL oldIRQL;
		KiLockDispatcherDatabase(&oldIRQL);

		XboxTypes::PLIST_ENTRY mutantList = pThread->MutantListHead.Blink;
		XboxTypes::LIST_ENTRY *pMutantList = ToPointer<XboxTypes::LIST_ENTRY>(mutantList);
		InsertHeadList(pMutantList, &pMutant->MutantListEntry);

		KiUnlockDispatcherDatabase(oldIRQL);
	}
	else {
		pMutant->Header.SignalState = 1;
		pMutant->OwnerThread = NULL;
	}

	//m_objmgr->RegisterMutant(Mutant, pMutant);
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

XboxTypes::BOOLEAN XboxKernel::KeInsertByKeyDeviceQueue(XboxTypes::PKDEVICE_QUEUE DeviceQueue, XboxTypes::PKDEVICE_QUEUE_ENTRY DeviceQueueEntry, XboxTypes::ULONG SortKey) {
	XboxTypes::KDEVICE_QUEUE *pDeviceQueue = ToPointer<XboxTypes::KDEVICE_QUEUE>(DeviceQueue);
	XboxTypes::KDEVICE_QUEUE_ENTRY *pDeviceQueueEntry = ToPointer<XboxTypes::KDEVICE_QUEUE_ENTRY>(DeviceQueueEntry);
	XboxTypes::BOOLEAN inserted;

	pDeviceQueueEntry->SortKey = SortKey;
	if (pDeviceQueue->Busy) {
		inserted = TRUE;
		XboxTypes::LIST_ENTRY *pListEntry = ToPointer<XboxTypes::LIST_ENTRY>(pDeviceQueue->DeviceListHead.Flink);
		while (pListEntry != &pDeviceQueue->DeviceListHead) {
			XboxTypes::KDEVICE_QUEUE_ENTRY *pQueueEntry = CONTAINING_RECORD(pListEntry, XboxTypes::KDEVICE_QUEUE_ENTRY, DeviceListEntry);
			if (SortKey < pQueueEntry->SortKey) {
				break;
			}
			pListEntry = ToPointer<XboxTypes::LIST_ENTRY>(pListEntry->Flink);
		}
		pListEntry = ToPointer<XboxTypes::LIST_ENTRY>(pListEntry->Blink);
		InsertHeadList(pListEntry, &pDeviceQueueEntry->DeviceListEntry);
	}
	else {
		inserted = FALSE;
		pDeviceQueue->Busy = TRUE;
	}
	pDeviceQueueEntry->Inserted = inserted;

	return inserted;
}

XboxTypes::BOOLEAN XboxKernel::KeInsertDeviceQueue(XboxTypes::PKDEVICE_QUEUE DeviceQueue, XboxTypes::PKDEVICE_QUEUE_ENTRY DeviceQueueEntry) {
	XboxTypes::KDEVICE_QUEUE *pDeviceQueue = ToPointer<XboxTypes::KDEVICE_QUEUE>(DeviceQueue);
	XboxTypes::KDEVICE_QUEUE_ENTRY *pDeviceQueueEntry = ToPointer<XboxTypes::KDEVICE_QUEUE_ENTRY>(DeviceQueueEntry);
	XboxTypes::BOOLEAN inserted;

	if (pDeviceQueue->Busy) {
		inserted = TRUE;
		InsertTailList(&pDeviceQueue->DeviceListHead, &pDeviceQueueEntry->DeviceListEntry);
	}
	else {
		pDeviceQueue->Busy = TRUE;
		inserted = FALSE;
	}
	pDeviceQueueEntry->Inserted = inserted;

	return inserted;
}

XboxTypes::BOOLEAN XboxKernel::KeInsertQueueDpc(XboxTypes::PRKDPC Dpc, XboxTypes::PVOID SystemArgument1, XboxTypes::PVOID SystemArgument2) {
	XboxTypes::KDPC *pDpc = ToPointer<XboxTypes::KDPC>(Dpc);
	
	XboxTypes::KIRQL oldIrql = KfRaiseIrql(HIGH_LEVEL);

	XboxTypes::BOOLEAN inserted = pDpc->Inserted;
	if (!inserted) {
		pDpc->Inserted = TRUE;
		pDpc->SystemArgument1 = SystemArgument1;
		pDpc->SystemArgument2 = SystemArgument2;

		XboxTypes::KPRCB *pKPRCB = &m_pKPCR->PrcbData;
		InsertTailList(&pKPRCB->DpcListHead, &pDpc->DpcListEntry);
		if (!pKPRCB->DpcRoutineActive && !pKPRCB->DpcInterruptRequested) {
			pKPRCB->DpcInterruptRequested = TRUE;
			// TODO: HalRequestSoftwareInterrupt(DISPATCH_LEVEL);
		}
	}

	// TODO: KfLowerIrql(oldIrql);

	return !inserted;
}

XboxTypes::ULONG XboxKernel::KeIsExecutingDpc() {
	return m_pKPCR->PrcbData.DpcRoutineActive;
}

XboxTypes::VOID XboxKernel::KeLeaveCriticalRegion() {
	XboxTypes::PKTHREAD thread = KeGetCurrentThread();
	XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(thread);

	pThread->KernelApcDisable++;
	
	XboxTypes::KAPC_STATE *apcState = &pThread->ApcState;
	XboxTypes::LIST_ENTRY *apcListHead = &apcState->ApcListHead[XboxTypes::KernelMode];
	if (pThread->KernelApcDisable == 0 && apcListHead->Flink != _PTR_TO_ADDR(LIST_ENTRY, apcListHead)) {
		apcState->KernelApcPending = TRUE;
		// TODO: HalRequestSoftwareInterrupt(APC_LEVEL);
	}
}

XboxTypes::KIRQL XboxKernel::KeRaiseIrqlToDpcLevel() {
	XboxTypes::KIRQL oldIRQL = m_pKPCR->Irql;
	m_pKPCR->Irql = DISPATCH_LEVEL;
	return oldIRQL;
}

XboxTypes::KIRQL XboxKernel::KeRaiseIrqlToSynchLevel() {
	XboxTypes::KIRQL oldIRQL = m_pKPCR->Irql;
	m_pKPCR->Irql = SYNCH_LEVEL;
	return oldIRQL;
}

XboxTypes::PKDEVICE_QUEUE_ENTRY XboxKernel::KeRemoveByKeyDeviceQueue(XboxTypes::PKDEVICE_QUEUE DeviceQueue, XboxTypes::ULONG SortKey) {
	XboxTypes::KDEVICE_QUEUE *pDeviceQueue = ToPointer<XboxTypes::KDEVICE_QUEUE>(DeviceQueue);
	XboxTypes::KDEVICE_QUEUE_ENTRY *pEntry;

	if (IsListEmpty(&pDeviceQueue->DeviceListHead)) {
		pDeviceQueue->Busy = FALSE;
		pEntry = NULL;
	}
	else {
		XboxTypes::LIST_ENTRY *pListEntry = ToPointer<XboxTypes::LIST_ENTRY>(pDeviceQueue->DeviceListHead.Flink);
		while (pListEntry != &pDeviceQueue->DeviceListHead) {
			pEntry = CONTAINING_RECORD(pListEntry, XboxTypes::KDEVICE_QUEUE_ENTRY, DeviceListEntry);
			if (SortKey <= pEntry->SortKey) {
				break;
			}
			pListEntry = ToPointer<XboxTypes::LIST_ENTRY>(pListEntry->Flink);
		}

		if (pListEntry != &pDeviceQueue->DeviceListHead) {
			RemoveEntryList(&pEntry->DeviceListEntry);

		}
		else {
			pListEntry = RemoveHeadList(&pDeviceQueue->DeviceListHead);
			pEntry = CONTAINING_RECORD(pListEntry, XboxTypes::KDEVICE_QUEUE_ENTRY, DeviceListEntry);
		}

		pEntry->Inserted = FALSE;
	}

	return _PTR_TO_ADDR(KDEVICE_QUEUE_ENTRY, pEntry);
}

XboxTypes::PKDEVICE_QUEUE_ENTRY XboxKernel::KeRemoveDeviceQueue(XboxTypes::PKDEVICE_QUEUE DeviceQueue) {
	XboxTypes::KDEVICE_QUEUE *pDeviceQueue = ToPointer<XboxTypes::KDEVICE_QUEUE>(DeviceQueue);
	XboxTypes::KDEVICE_QUEUE_ENTRY *pEntry;

	if (IsListEmpty(&pDeviceQueue->DeviceListHead)) {
		pDeviceQueue->Busy = FALSE;
		pEntry = NULL;
	}
	else {
		XboxTypes::LIST_ENTRY *pListEntry = RemoveHeadList(&pDeviceQueue->DeviceListHead);
		pEntry = CONTAINING_RECORD(pListEntry, XboxTypes::KDEVICE_QUEUE_ENTRY, DeviceListEntry);
		pEntry->Inserted = FALSE;
	}

	return _PTR_TO_ADDR(KDEVICE_QUEUE_ENTRY, pEntry);
}

XboxTypes::BOOLEAN XboxKernel::KeRemoveEntryDeviceQueue(XboxTypes::PKDEVICE_QUEUE DeviceQueue, XboxTypes::PKDEVICE_QUEUE_ENTRY DeviceQueueEntry) {
	// NOTE: DeviceQueue is not used for some reason

	XboxTypes::KIRQL oldIRQL;
	KiLockDispatcherDatabase(&oldIRQL);

	XboxTypes::KDEVICE_QUEUE_ENTRY *pDeviceQueueEntry = ToPointer<XboxTypes::KDEVICE_QUEUE_ENTRY>(DeviceQueueEntry);

	XboxTypes::BOOLEAN currentlyInserted = pDeviceQueueEntry->Inserted;
	if (currentlyInserted) {
		pDeviceQueueEntry->Inserted = FALSE;
		RemoveEntryList(&pDeviceQueueEntry->DeviceListEntry);
	}

	KiUnlockDispatcherDatabase(oldIRQL);

	return currentlyInserted;
}

XboxTypes::BOOLEAN XboxKernel::KeRemoveQueueDpc(XboxTypes::PRKDPC Dpc) {
	XboxTypes::KDPC *pDpc = ToPointer<XboxTypes::KDPC>(Dpc);

	XboxTypes::BOOLEAN inserted = pDpc->Inserted;
	if (inserted) {
		RemoveEntryList(&pDpc->DpcListEntry);
		pDpc->Inserted = FALSE;
	}

	return inserted;
}

XboxTypes::LONG XboxKernel::KeResetEvent(XboxTypes::PRKEVENT Event) {
	XboxTypes::KEVENT *pEvent = ToPointer<XboxTypes::KEVENT>(Event);

	XboxTypes::KIRQL oldIRQL;
	KiLockDispatcherDatabase(&oldIRQL);

	XboxTypes::LONG prevState = pEvent->Header.SignalState;
	pEvent->Header.SignalState = 0;

	KiUnlockDispatcherDatabase(oldIRQL);

	return prevState;
}

XboxTypes::LOGICAL XboxKernel::KeSetDisableBoostThread(XboxTypes::PKTHREAD Thread, XboxTypes::LOGICAL Disable) {
	XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(Thread);

	XboxTypes::KIRQL oldIRQL;
	KiLockDispatcherDatabase(&oldIRQL);

	XboxTypes::LOGICAL prevDisableBoost = pThread->DisableBoost;
	pThread->DisableBoost = Disable;

	KiUnlockDispatcherDatabase(oldIRQL);

	return prevDisableBoost;
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

XboxTypes::VOID XboxKernel::KeStallExecutionProcessor(XboxTypes::ULONG MicroSeconds) {
	log_spew("Stalling execution for %ud microseconds\n", MicroSeconds);
}

XboxTypes::KIRQL XboxKernel::KfRaiseIrql(XboxTypes::KIRQL NewIrql) {
	XboxTypes::KIRQL oldIRQL = m_pKPCR->Irql;
	m_pKPCR->Irql = NewIrql;
	return oldIRQL;
}
