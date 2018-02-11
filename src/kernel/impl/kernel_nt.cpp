#include "kernel/impl/kernel.h"
#include "kernel/impl/kernel_ki.h"

XboxTypes::NTSTATUS XboxKernel::NtAllocateVirtualMemory(XboxTypes::PPVOID BaseAddress, XboxTypes::ULONG_PTR ZeroBits, XboxTypes::PSIZE_T RegionSize, XboxTypes::ULONG AllocationType, XboxTypes::ULONG Protect) {
	XboxTypes::SIZE_T regionSize;
	m_cpu->MemRead(RegionSize, sizeof(XboxTypes::SIZE_T), &regionSize);

	// TODO: need a virtual memory manager on top of the existing memory manager
	PhysicalMemoryBlock *block = m_pmemmgr->AllocateContiguous(regionSize);
	if (nullptr == block) {
		return STATUS_NO_MEMORY;
	}

	uint32_t baseAddr = block->BaseAddress();
	uint32_t size = block->TotalSize();
	m_cpu->MemWrite(BaseAddress, sizeof(XboxTypes::PVOID), &baseAddr);
	m_cpu->MemWrite(RegionSize, sizeof(XboxTypes::SIZE_T), &size);

	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::NtClose(XboxTypes::HANDLE Handle) {
	// TODO: tell the object manager to close the handle
	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::NtCreateEvent(XboxTypes::PHANDLE EventHandle, XboxTypes::POBJECT_ATTRIBUTES ObjectAttributes, XboxTypes::EVENT_TYPE EventType, XboxTypes::BOOLEAN InitialState) {
	// Ensure the type is a valid event type
	if (XboxTypes::NotificationEvent != EventType && XboxTypes::SynchronizationEvent != EventType) {
		return STATUS_INVALID_PARAMETER;
	}

	// TODO: implement
	// FIXME: These events need to be handled in the scheduler.

	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::NtCreateFile(XboxTypes::PHANDLE FileHandle, XboxTypes::ACCESS_MASK DesiredAccess, XboxTypes::POBJECT_ATTRIBUTES ObjectAttributes, XboxTypes::PIO_STATUS_BLOCK IoStatusBlock, XboxTypes::PLARGE_INTEGER AllocationSize, XboxTypes::ULONG FileAttributes, XboxTypes::ULONG ShareAccess, XboxTypes::ULONG CreateDisposition, XboxTypes::ULONG CreateOptions) {
	return IoCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, 0);
}

XboxTypes::NTSTATUS XboxKernel::NtDeviceIoControlFile(XboxTypes::HANDLE FileHandle, XboxTypes::HANDLE Event, XboxTypes::PIO_APC_ROUTINE ApcRoutine, XboxTypes::PVOID ApcContext, XboxTypes::PIO_STATUS_BLOCK, XboxTypes::ULONG IoControlCode, XboxTypes::PVOID InputBuffer, XboxTypes::ULONG InputBufferLength, XboxTypes::PVOID OutputBuffer, XboxTypes::ULONG OutputBufferLength) {
	// FIXME: implement properly

	// Fake just enough to get past DVD authentication
	XboxTypes::CHAR *data = ToPointer<XboxTypes::CHAR>(InputBuffer);
	XboxTypes::CHAR *auth = ToPointer<XboxTypes::CHAR>(*(XboxTypes::DWORD *)(data + 0x14));
	auth[10] = 1;
	auth[11] = 1;
	auth[12] = 1;
	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::NtOpenFile(XboxTypes::PHANDLE FileHandle, XboxTypes::ACCESS_MASK DesiredAccess, XboxTypes::POBJECT_ATTRIBUTES ObjectAttributes, XboxTypes::PIO_STATUS_BLOCK IoStatusBlock, XboxTypes::ULONG ShareAccess, XboxTypes::ULONG OpenOptions) {
	return IoCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, NULL, 0L, ShareAccess, FILE_OPEN, OpenOptions, 0);
}

XboxTypes::NTSTATUS XboxKernel::NtQueryInformationFile(XboxTypes::HANDLE FileHandle, XboxTypes::PIO_STATUS_BLOCK IoStatusBlock, XboxTypes::PVOID FileInformation, XboxTypes::ULONG Length, XboxTypes::FILE_INFORMATION_CLASS FileInformationClass) {
	// FIXME: implement properly
	
	switch (FileInformationClass) {
	case XboxTypes::FileNetworkOpenInformation: {
		// FIXME: fake and incomplete data
		XboxTypes::FILE_NETWORK_OPEN_INFORMATION *fNetOpen = ToPointer<XboxTypes::FILE_NETWORK_OPEN_INFORMATION>(FileInformation);
		fNetOpen->EndOfFile.QuadPart = 0x1234full;
		break;
	}
	}

	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::NtQueryVolumeInformationFile(XboxTypes::HANDLE FileHandle, XboxTypes::PIO_STATUS_BLOCK IoStatusBlock, XboxTypes::PVOID FsInformation, XboxTypes::ULONG Length, XboxTypes::FS_INFORMATION_CLASS FsInformationClass) {
	// FIXME: implement properly

	switch (FsInformationClass) {
	case XboxTypes::FileFsSizeInformation: {
		// FIXME: fake data
		XboxTypes::FILE_FS_SIZE_INFORMATION *fsSizeInfo = ToPointer<XboxTypes::FILE_FS_SIZE_INFORMATION>(FsInformation);
		fsSizeInfo->BytesPerSector = 512;
		fsSizeInfo->SectorsPerAllocationUnit = 32;
		fsSizeInfo->TotalAllocationUnits.QuadPart = 0x00000000fffffffull;
		fsSizeInfo->AvailableAllocationUnits.QuadPart = 0x000000007ffffffull;

		break;
	}
		// TODO: implement the rest of the types
	}
	return STATUS_SUCCESS;
}

XboxTypes::NTSTATUS XboxKernel::NtYieldExecution() {
	XboxTypes::NTSTATUS status = STATUS_NO_YIELD_PERFORMED;

	if (m_kernelData->KiReadySummary != 0) {
		XboxTypes::PKTHREAD thread = KeGetCurrentThread();
		XboxTypes::KTHREAD *pThread = ToPointer<XboxTypes::KTHREAD>(thread);
		XboxTypes::KPRCB *prcb = &m_pKPCR->PrcbData;

		KiLockDispatcherDatabase(&pThread->WaitIrql);
		if (prcb->NextThread == NULL) {
			prcb->NextThread = KiFindReadyThread(1);
		}

		if (prcb->NextThread != NULL) {
			XboxTypes::KPROCESS *pProcess = ToPointer<XboxTypes::KPROCESS>(pThread->ApcState.Process);
			pThread->Quantum = pProcess->ThreadQuantum;
			pThread->State = XboxTypes::Ready;
			XboxTypes::KPRIORITY priority = pThread->Priority;
			if (priority < LOW_REALTIME_PRIORITY) {
				priority = priority - pThread->PriorityDecrement - 1;
				if (priority < pThread->BasePriority) {
					priority = pThread->BasePriority;
				}

				pThread->PriorityDecrement = 0;
			}
			pThread->Priority = priority;

			InsertTailList(&m_kernelData->KiDispatcherReadyListHead[priority], &pThread->WaitListEntry);

			SetMember(priority, m_kernelData->KiReadySummary);
			m_sched->SuspendCurrentThread(new AlwaysTrueTSCondition());
			status = STATUS_SUCCESS;

		}
		else {
			KiUnlockDispatcherDatabase(pThread->WaitIrql);
		}
	}

	return status;
}
