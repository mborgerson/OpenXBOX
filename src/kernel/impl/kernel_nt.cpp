#include "kernel/impl/kernel.h"

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
