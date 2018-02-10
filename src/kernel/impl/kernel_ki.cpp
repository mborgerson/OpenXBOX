#include "kernel/impl/kernel.h"
#include "kernel/impl/kernel_ki.h"
#include "log.h"

XboxTypes::VOID XboxKernel::KiInitializeContextThread(XboxTypes::PKTHREAD Thread, XboxTypes::SIZE_T TlsDataSize, XboxTypes::PKSYSTEM_ROUTINE SystemRoutine, XboxTypes::PKSTART_ROUTINE StartRoutine, XboxTypes::PVOID StartContext) {
	XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(Thread);

	XboxTypes::PFX_SAVE_AREA npxFrame = (XboxTypes::PFX_SAVE_AREA)(pThread->StackBase - sizeof(XboxTypes::FX_SAVE_AREA));
	RtlZeroMemory(npxFrame, sizeof(XboxTypes::FX_SAVE_AREA));

	XboxTypes::FX_SAVE_AREA *pNpxFrame = ToPointer<XboxTypes::FX_SAVE_AREA>(npxFrame);
	pNpxFrame->FloatSave.ControlWord = 0x27f;
	pNpxFrame->FloatSave.MXCsr = 0x1f80;

	pThread->NpxState = NPX_STATE_NOT_LOADED;

	TlsDataSize = ALIGN_UP(TlsDataSize, XboxTypes::ULONG);
	XboxTypes::PVOID tlsData = npxFrame - TlsDataSize;

	if (TlsDataSize != 0) {
		pThread->TlsData = tlsData;
		RtlZeroMemory(tlsData, TlsDataSize);
	}
	else {
		pThread->TlsData = NULL;
	}

	XboxTypes::ULONG *pStartContext = ToPointer<XboxTypes::ULONG>(tlsData - 1);
	XboxTypes::ULONG *pStartRoutine = pStartContext - 1;
	XboxTypes::ULONG *pSystemRoutine = pStartRoutine - 1;

	XboxTypes::PKSWITCHFRAME switchFrame = _PTR_TO_ADDR(ULONG, pSystemRoutine) - sizeof(XboxTypes::KSWITCHFRAME);
	XboxTypes::KSWITCHFRAME *pSwitchFrame = ToPointer<XboxTypes::KSWITCHFRAME>(switchFrame);

	*pStartContext = StartContext;
	*pStartRoutine = StartRoutine;
	*pSystemRoutine = SystemRoutine;

	// TODO: handle pointers to functions
	pSwitchFrame->RetAddr = (XboxTypes::ULONG)/*m_KiThreadStartup*/NULL;
	pSwitchFrame->Eflags = EFLAGS_INTERRUPT_MASK;
	pSwitchFrame->ExceptionList = EXCEPTION_CHAIN_END;

	pThread->KernelStack = switchFrame;
}

XboxTypes::VOID XboxKernel::KiInitSystem() {
	for (XboxTypes::ULONG i = 0; i < MAXIMUM_PRIORITY; i++) {
		InitializeListHead(&m_KiDispatcherReadyListHead[i]);
	}

	// TODO: handle pointers to functions
	KeInitializeDpc(_PTR_TO_ADDR(KDPC, &m_KiTimerExpireDpc), (XboxTypes::PKDEFERRED_ROUTINE)/*m_KiTimerExpiration*/NULL, NULL);

	for (XboxTypes::ULONG i = 0; i < TIMER_TABLE_SIZE; i++) {
		InitializeListHead(&m_KiTimerTableListHead[i]);
	}

	InitializeListHead(&m_KiWaitInListHead);
}

XboxTypes::PKTHREAD XboxKernel::KiFindReadyThread(XboxTypes::KPRIORITY LowPriority) {
	XboxTypes::ULONG highPriority;

	XboxTypes::ULONG prioritySet = (~((1 << LowPriority) - 1)) & m_KiReadySummary;
	FindFirstSetLeftMember(prioritySet, &highPriority);
	prioritySet <<= (31 - highPriority);

	XboxTypes::LIST_ENTRY *listHead = &m_KiDispatcherReadyListHead[highPriority];
	while (prioritySet != 0) {
		if ((XboxTypes::LONG)prioritySet < 0) {
			XboxTypes::PLIST_ENTRY nextEntry = listHead->Flink;

			XboxTypes::KTHREAD *thread = CONTAINING_RECORD(nextEntry, XboxTypes::KTHREAD, WaitListEntry);
			RemoveEntryList(&thread->WaitListEntry);
			if (IsListEmpty(listHead)) {
				ClearMember(highPriority, m_KiReadySummary);
			}

			return _PTR_TO_ADDR(KTHREAD, thread);
		}

		highPriority -= 1;
		listHead -= 1;
		prioritySet <<= 1;
	};

	return NULL;
}

XboxTypes::VOID XboxKernel::KiReadyThread(XboxTypes::PRKTHREAD Thread) {
	XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(Thread);
	
	XboxTypes::KPRIORITY threadPriority = pThread->Priority;
	XboxTypes::BOOLEAN wasPreempted = pThread->Preempted;
	pThread->Preempted = FALSE;
	pThread->WaitTime = m_KeTickCount;

	XboxTypes::KPRCB *prcb = &m_pKPCR->PrcbData;

	if (m_KiIdleSummary != 0) {
		m_KiIdleSummary = 0;
		prcb->NextThread = Thread;
		pThread->State = XboxTypes::Standby;

		return;
	}

	XboxTypes::PKTHREAD thread1;
	XboxTypes::KTHREAD *pThread1;
	if (prcb->NextThread != NULL) {
		thread1 = prcb->NextThread;
		pThread1 = ToPointer<XboxTypes::KTHREAD>(prcb->NextThread);
		if (threadPriority > pThread1->Priority) {
			pThread1->Preempted = TRUE;
			prcb->NextThread = thread1;
			pThread1->State = XboxTypes::Standby;
			KiReadyThread(thread1);
			return;
		}
	}
	else {
		thread1 = prcb->CurrentThread;
		pThread1 = ToPointer<XboxTypes::KTHREAD>(prcb->NextThread);
		if (threadPriority > pThread1->Priority) {
			pThread1->Preempted = TRUE;
			prcb->NextThread = thread1;
			pThread1->State = XboxTypes::Standby;
			return;
		}
	}

	pThread->State = XboxTypes::Ready;
	if (wasPreempted) {
		InsertHeadList(&m_KiDispatcherReadyListHead[threadPriority], &pThread->WaitListEntry);
	}
	else {
		InsertTailList(&m_KiDispatcherReadyListHead[threadPriority], &pThread->WaitListEntry);
	}

	SetMember(threadPriority, m_KiReadySummary);
}

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
