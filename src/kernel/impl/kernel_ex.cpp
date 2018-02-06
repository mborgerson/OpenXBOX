#include "kernel/impl/kernel.h"

XboxTypes::PVOID XboxKernel::ExAllocatePool(XboxTypes::SIZE_T NumberOfBytes) {
	return ExAllocatePoolWithTag(NumberOfBytes, 'enoN');
}

XboxTypes::PVOID XboxKernel::ExAllocatePoolWithTag(XboxTypes::SIZE_T NumberOfBytes, XboxTypes::ULONG Tag) {
	// TODO: implement properly
	return MmAllocateContiguousMemory(NumberOfBytes);
}

XboxTypes::PLIST_ENTRY XboxKernel::ExfInterlockedInsertHeadList(XboxTypes::PLIST_ENTRY ListHead, XboxTypes::PLIST_ENTRY ListEntry) {
	XboxTypes::LIST_ENTRY *pListHead = ToPointer<XboxTypes::LIST_ENTRY>(ListHead);
	XboxTypes::LIST_ENTRY *pListEntry = ToPointer<XboxTypes::LIST_ENTRY>(ListEntry);
	XboxTypes::LIST_ENTRY *pNext = ToPointer<XboxTypes::LIST_ENTRY>(pListHead->Flink);

	pListEntry->Flink = pListHead->Flink;
	pListEntry->Blink = ListHead;
	pListHead->Flink = ListEntry;
	pNext->Blink = ListEntry;

	if (pListHead->Flink == ListHead) {
		return NULL;
	}
	return pListHead->Flink;
}

XboxTypes::PLIST_ENTRY XboxKernel::ExfInterlockedInsertTailList(XboxTypes::PLIST_ENTRY ListHead, XboxTypes::PLIST_ENTRY ListEntry) {
	XboxTypes::LIST_ENTRY *pListHead = ToPointer<XboxTypes::LIST_ENTRY>(ListHead);
	XboxTypes::LIST_ENTRY *pListEntry = ToPointer<XboxTypes::LIST_ENTRY>(ListEntry);
	XboxTypes::LIST_ENTRY *pPrev = ToPointer<XboxTypes::LIST_ENTRY>(pListHead->Blink);

	pListEntry->Flink = ListHead;
	pListEntry->Blink = pListHead->Blink;
	pListHead->Flink = ListEntry;
	pPrev->Blink = ListEntry;

	if (pListHead->Blink == ListHead) {
		return NULL;
	}
	return pListHead->Blink;
}

XboxTypes::PLIST_ENTRY XboxKernel::ExfInterlockedRemoveHeadList(XboxTypes::PLIST_ENTRY ListHead) {
	XboxTypes::LIST_ENTRY *pListHead = ToPointer<XboxTypes::LIST_ENTRY>(ListHead);
	XboxTypes::LIST_ENTRY *pNext = ToPointer<XboxTypes::LIST_ENTRY>(pListHead->Flink);

	if (pListHead->Flink == ListHead) {
		return NULL;
	}

	XboxTypes::LIST_ENTRY *pNextNext = ToPointer<XboxTypes::LIST_ENTRY>(pNext->Flink);
	pListHead->Flink = pNext->Flink;
	pNextNext->Blink = ListHead;

	return pListHead->Flink;
}

XboxTypes::VOID XboxKernel::ExFreePool(XboxTypes::PVOID P) {
	// TODO: implement properly
	MmFreeContiguousMemory(P);
}

XboxTypes::VOID XboxKernel::ExInitializeReadWriteLock(XboxTypes::PERWLOCK ReadWriteLock) {
	XboxTypes::ERWLOCK *pRWLock = ToPointer<XboxTypes::ERWLOCK>(ReadWriteLock);

	pRWLock->WritersWaitingCount = 0;
	pRWLock->ReadersWaitingCount = 0;
	pRWLock->ReadersEntryCount = 0;
	pRWLock->LockCount = -1;

	KeInitializeEvent(_PTR_TO_ADDR(KEVENT, &pRWLock->WriterEvent), XboxTypes::SynchronizationEvent, FALSE);
	KeInitializeSemaphore(_PTR_TO_ADDR(KSEMAPHORE, &pRWLock->ReaderSemaphore), 0, LONG_MAX);
}

XboxTypes::LARGE_INTEGER XboxKernel::ExInterlockedAddLargeInteger(XboxTypes::PLARGE_INTEGER Addend, XboxTypes::LARGE_INTEGER Increment) {
	XboxTypes::LARGE_INTEGER *pAddend = ToPointer<XboxTypes::LARGE_INTEGER>(Addend);

	XboxTypes::LARGE_INTEGER oldValue = *pAddend;
	pAddend->QuadPart += Increment.QuadPart;
	return oldValue;
}

XboxTypes::VOID XboxKernel::ExInterlockedAddLargeStatistic(XboxTypes::PLARGE_INTEGER Addend, XboxTypes::ULONG Increment) {
	XboxTypes::LARGE_INTEGER *pAddend = ToPointer<XboxTypes::LARGE_INTEGER>(Addend);
	
	pAddend->QuadPart += Increment;
}

XboxTypes::LONGLONG XboxKernel::ExInterlockedCompareExchange64(XboxTypes::PLONGLONG Destination, XboxTypes::PLONGLONG Exchange, XboxTypes::PLONGLONG Comparand) {
	XboxTypes::LONGLONG *pDestination = ToPointer<XboxTypes::LONGLONG>(Destination);

	XboxTypes::LONGLONG oldValue = *pDestination;
	if (*pDestination == Comparand) {
		*pDestination = Exchange;
	}

	return oldValue;
}

XboxTypes::NTSTATUS XboxKernel::ExQueryNonVolatileSetting(XboxTypes::ULONG ValueIndex, XboxTypes::PULONG Type, XboxTypes::PVOID Value, XboxTypes::ULONG ValueLength, XboxTypes::PULONG ResultLength) {
	// TODO: read from EEPROM
	return STATUS_SUCCESS;
}
