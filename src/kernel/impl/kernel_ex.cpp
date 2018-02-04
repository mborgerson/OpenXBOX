#include "kernel/impl/kernel.h"

XboxTypes::PVOID XboxKernel::ExAllocatePool(XboxTypes::SIZE_T NumberOfBytes) {
	return ExAllocatePoolWithTag(NumberOfBytes, 'enoN');
}

XboxTypes::PVOID XboxKernel::ExAllocatePoolWithTag(XboxTypes::SIZE_T NumberOfBytes, XboxTypes::ULONG Tag) {
	// TODO: implement properly
	return MmAllocateContiguousMemory(NumberOfBytes);
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

XboxTypes::NTSTATUS XboxKernel::ExQueryNonVolatileSetting(XboxTypes::ULONG ValueIndex, XboxTypes::PULONG Type, XboxTypes::PVOID Value, XboxTypes::ULONG ValueLength, XboxTypes::PULONG ResultLength) {
	// TODO: read from EEPROM
	return STATUS_SUCCESS;
}
