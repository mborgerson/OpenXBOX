#include "kernel/impl/kernel.h"
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
		InitializeListHead(&m_kernelData->KiDispatcherReadyListHead[i]);
	}

	// TODO: handle pointers to functions
	KeInitializeDpc(_PTR_TO_ADDR(KDPC, &m_kernelData->KiTimerExpireDpc), (XboxTypes::PKDEFERRED_ROUTINE)/*m_KiTimerExpiration*/NULL, NULL);

	for (XboxTypes::ULONG i = 0; i < TIMER_TABLE_SIZE; i++) {
		InitializeListHead(&m_kernelData->KiTimerTableListHead[i]);
	}

	InitializeListHead(&m_kernelData->KiWaitInListHead);
}

XboxTypes::PKTHREAD XboxKernel::KiFindReadyThread(XboxTypes::KPRIORITY LowPriority) {
	XboxTypes::ULONG highPriority;

	XboxTypes::ULONG prioritySet = (~((1 << LowPriority) - 1)) & m_kernelData->KiReadySummary;
	FindFirstSetLeftMember(prioritySet, &highPriority);
	prioritySet <<= (31 - highPriority);

	XboxTypes::LIST_ENTRY *listHead = &m_kernelData->KiDispatcherReadyListHead[highPriority];
	while (prioritySet != 0) {
		if ((XboxTypes::LONG)prioritySet < 0) {
			XboxTypes::PLIST_ENTRY nextEntry = listHead->Flink;

			XboxTypes::KTHREAD *thread = CONTAINING_RECORD(nextEntry, XboxTypes::KTHREAD, WaitListEntry);
			RemoveEntryList(&thread->WaitListEntry);
			if (IsListEmpty(listHead)) {
				ClearMember(highPriority, m_kernelData->KiReadySummary);
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
	pThread->WaitTime = m_kernelData->KeTickCount;

	XboxTypes::KPRCB *prcb = &m_pKPCR->PrcbData;

	if (m_kernelData->KiIdleSummary != 0) {
		m_kernelData->KiIdleSummary = 0;
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
		InsertHeadList(&m_kernelData->KiDispatcherReadyListHead[threadPriority], &pThread->WaitListEntry);
	}
	else {
		InsertTailList(&m_kernelData->KiDispatcherReadyListHead[threadPriority], &pThread->WaitListEntry);
	}

	SetMember(threadPriority, m_kernelData->KiReadySummary);

	// Tell the scheduler that the thread has been resumed
	m_sched->ResumeThread(Thread);
}

XboxTypes::VOID XboxKernel::KiUnlockDispatcherDatabase(XboxTypes::KIRQL OldIrql) {
	if (m_pKPCR->PrcbData.NextThread == NULL) {
		KfLowerIrql(OldIrql);
		return;
	}

	if (OldIrql >= DISPATCH_LEVEL) {
		if (m_pKPCR->PrcbData.DpcRoutineActive == NULL) {
			// TODO: HalRequestSoftwareInterrupt(DISPATCH_LEVEL);
		}

		KfLowerIrql(OldIrql);
		return;
	}

	XboxTypes::KTHREAD *pCurrThread = ToPointer<XboxTypes::KTHREAD>(m_pKPCR->PrcbData.CurrentThread);
	XboxTypes::KIRQL prevIrql = pCurrThread->WaitIrql;
	m_pKPCR->PrcbData.CurrentThread = m_pKPCR->PrcbData.NextThread;
	m_pKPCR->PrcbData.NextThread = NULL;
	// FIXME: this is incomplete
	// At this point, pending APCs (if any) should run at IRQL = APC_LEVEL

	KfLowerIrql(OldIrql);
	return;
}

XboxTypes::VOID XboxKernel::KiUnwaitThread(XboxTypes::PRKTHREAD Thread, XboxTypes::LONG_PTR WaitStatus, XboxTypes::KPRIORITY Increment) {
	XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(Thread);

	pThread->WaitStatus |= WaitStatus;
	XboxTypes::PKWAIT_BLOCK waitBlock = pThread->WaitBlockList;
	do {
		XboxTypes::KWAIT_BLOCK *pWaitBlock = ToPointer<XboxTypes::KWAIT_BLOCK>(waitBlock);
		RemoveEntryList(&pWaitBlock->WaitListEntry);
		waitBlock = pWaitBlock->NextWaitBlock;
	} while (waitBlock != pThread->WaitBlockList);
	RemoveEntryList(&pThread->WaitListEntry);

	XboxTypes::KTIMER *timer = &pThread->Timer;
	if (timer->Header.Inserted != FALSE) {
		KiRemoveTreeTimer(timer);
	}

	XboxTypes::KQUEUE *queue = ToPointer<XboxTypes::KQUEUE>(pThread->Queue);
	if (queue != NULL) {
		queue->CurrentCount += 1;
	}

	XboxTypes::KPROCESS *process = ToPointer<XboxTypes::KPROCESS>(pThread->ApcState.Process);
	if (pThread->Priority < LOW_REALTIME_PRIORITY) {
		if (pThread->PriorityDecrement == 0 && !pThread->DisableBoost) {
			XboxTypes::KPRIORITY newPriority = pThread->BasePriority + Increment;

			if (newPriority > pThread->Priority) {
				if (newPriority >= LOW_REALTIME_PRIORITY) {
					pThread->Priority = LOW_REALTIME_PRIORITY - 1;
				}
				else {
					pThread->Priority = newPriority;
				}
			}
		}

		if (pThread->BasePriority >= TIME_CRITICAL_PRIORITY_BOUND) {
			pThread->Quantum = process->ThreadQuantum;
		}
		else {
			pThread->Quantum -= WAIT_QUANTUM_DECREMENT;
			if (pThread->Quantum <= 0) {
				pThread->Quantum = process->ThreadQuantum;
				pThread->Priority -= (pThread->PriorityDecrement + 1);
				if (pThread->Priority < pThread->BasePriority) {
					pThread->Priority = pThread->BasePriority;
				}

				pThread->PriorityDecrement = 0;
			}
		}
	}
	else {
		pThread->Quantum = process->ThreadQuantum;
	}

	KiReadyThread(Thread);
}

XboxTypes::VOID XboxKernel::KiWaitSatisfyAll(XboxTypes::PKWAIT_BLOCK WaitBlock) {
	XboxTypes::KWAIT_BLOCK *pWaitBlock = ToPointer<XboxTypes::KWAIT_BLOCK>(WaitBlock);
	
	XboxTypes::KWAIT_BLOCK *pCurrentWaitBlock = pWaitBlock;
	XboxTypes::PKTHREAD thread = pWaitBlock->Thread;
	do {
		if (pCurrentWaitBlock->WaitKey != STATUS_TIMEOUT) {
			KiWaitSatisfyAny((XboxTypes::PKMUTANT)pCurrentWaitBlock->Object, thread);
		}
		pCurrentWaitBlock = ToPointer<XboxTypes::KWAIT_BLOCK>(pCurrentWaitBlock->NextWaitBlock);
	} while (pCurrentWaitBlock != pWaitBlock);
}

XboxTypes::VOID XboxKernel::KiWaitSatisfyAny(XboxTypes::PKMUTANT Object, XboxTypes::PKTHREAD Thread) {
	XboxTypes::KMUTANT *pObject = ToPointer<XboxTypes::KMUTANT>(Object);
	XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(Thread);

	if ((pObject->Header.Type & DISPATCHER_OBJECT_TYPE_MASK) == XboxTypes::EventSynchronizationObject) {
		pObject->Header.SignalState = 0;
	}
	else if (pObject->Header.Type == XboxTypes::SemaphoreObject) {
		pObject->Header.SignalState -= 1;
	}
	else if (pObject->Header.Type == XboxTypes::MutantObject) {
		pObject->Header.SignalState -= 1;
		if (pObject->Header.SignalState == 0) {
			pObject->OwnerThread = Thread;
			if (pObject->Abandoned == TRUE) {
				pObject->Abandoned = FALSE;
				pThread->WaitStatus = STATUS_ABANDONED;
			}
			InsertHeadList(pThread->MutantListHead.Blink, &pObject->MutantListEntry);
		}
	}
}

XboxTypes::VOID XboxKernel::KiWaitTest(XboxTypes::PVOID Object, XboxTypes::KPRIORITY Increment) {
	XboxTypes::KEVENT *pEvent = ToPointer<XboxTypes::KEVENT>(Object);
	
	XboxTypes::LIST_ENTRY *listHead = &pEvent->Header.WaitListHead;
	XboxTypes::LIST_ENTRY *waitEntry = ToPointer<XboxTypes::LIST_ENTRY>(listHead->Flink);
	while (pEvent->Header.SignalState > 0 && waitEntry != listHead) {
		XboxTypes::KWAIT_BLOCK *waitBlock = CONTAINING_RECORD(waitEntry, XboxTypes::KWAIT_BLOCK, WaitListEntry);

		XboxTypes::PRKTHREAD thread = waitBlock->Thread;
		if (waitBlock->WaitType != XboxTypes::WaitAny) {
			XboxTypes::BOOLEAN continueScan = FALSE;
			XboxTypes::KWAIT_BLOCK *nextWaitBlock = ToPointer<XboxTypes::KWAIT_BLOCK>(waitBlock->NextWaitBlock);
			while (nextWaitBlock != waitBlock) {
				if (nextWaitBlock->WaitKey != STATUS_TIMEOUT) {
					XboxTypes::KMUTANT *pMutant = ToPointer<XboxTypes::KMUTANT>(nextWaitBlock->Object);
					if (pMutant->Header.Type == XboxTypes::MutantObject && pMutant->Header.SignalState <= 0 && thread == pMutant->OwnerThread) {
						nextWaitBlock = ToPointer<XboxTypes::KWAIT_BLOCK>(nextWaitBlock->NextWaitBlock);
						continue;
					}

					if (pMutant->Header.SignalState <= 0) {
						continueScan = TRUE;
						break;
					}
				}
			}

			waitEntry = ToPointer<XboxTypes::LIST_ENTRY>(waitEntry->Blink);
			if (continueScan) {
				continue;
			}

			KiWaitSatisfyAll(_PTR_TO_ADDR(KWAIT_BLOCK, waitBlock));
		}
		else {
			waitEntry = ToPointer<XboxTypes::LIST_ENTRY>(waitEntry->Blink);
			KiWaitSatisfyAny((XboxTypes::PKMUTANT)Object, thread);
		}

		KiUnwaitThread(thread, waitBlock->WaitKey, Increment);
		waitEntry = ToPointer<XboxTypes::LIST_ENTRY>(waitEntry->Blink);
	}
}
