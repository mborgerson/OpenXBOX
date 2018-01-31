#include "kernel/impl/kernel.h"
#include "log.h"

XboxTypes::PVOID XboxKernel::MmAllocateContiguousMemory(XboxTypes::SIZE_T NumberOfBytes) {
	return MmAllocateContiguousMemoryEx(NumberOfBytes, 0, ULONG_MAX, PAGE_SIZE, PAGE_READWRITE);
}

XboxTypes::PVOID XboxKernel::MmAllocateContiguousMemoryEx(XboxTypes::SIZE_T NumberOfBytes, XboxTypes::ULONG_PTR LowestAcceptableAddress, XboxTypes::ULONG_PTR HighestAcceptableAddress, XboxTypes::ULONG_PTR Alignment, XboxTypes::ULONG Protect) {
	log_spew("Requested allocation of 0x%x bytes\n", NumberOfBytes);
	log_spew("  LowestAcceptableAddress  = %x\n", LowestAcceptableAddress);
	log_spew("  HighestAcceptableAddress = %x\n", HighestAcceptableAddress);
	log_spew("  Alignment                = %x\n", Alignment);
	log_spew("  Protect                  = %x\n", Protect);

	PhysicalMemoryBlock *block = m_pmemmgr->AllocateContiguous(NumberOfBytes, LowestAcceptableAddress, HighestAcceptableAddress, Alignment, Protect);
	if (block != nullptr) {
		log_spew("... allocated at %x\n", block->BaseAddress());
		return block->BaseAddress();
	}
	else {
		log_spew("... failed to allocate\n");
		return NULL;
	}
}

XboxTypes::PVOID XboxKernel::MmClaimGpuInstanceMemory(XboxTypes::SIZE_T NumberOfBytes, XboxTypes::PSIZE_T NumberOfPaddingBytes) {
	// FIXME: fake implementation
	XboxTypes::SIZE_T val = 0x10000;
	m_cpu->MemWrite(NumberOfPaddingBytes, sizeof(XboxTypes::SIZE_T), &val);
	return 0x03FF0000;
}

XboxTypes::VOID XboxKernel::MmFreeContiguousMemory(XboxTypes::PVOID BaseAddress) {
	log_spew("Attempting to free memory block at 0x%x... ");
	if (m_pmemmgr->FreeContiguous(BaseAddress)) {
		log_spew("OK\n");
	}
	else {
		log_spew("address is not allocated\n");
		// TODO: should we stop execution here?
	}
}

XboxTypes::VOID XboxKernel::MmPersistContiguousMemory(XboxTypes::PVOID BaseAddress, XboxTypes::SIZE_T NumberOfBytes, XboxTypes::BOOLEAN Persist) {
	m_pmemmgr->SetPersist(BaseAddress, NumberOfBytes, Persist);
}

XboxTypes::ULONG XboxKernel::MmQueryAddressProtect(XboxTypes::PVOID VirtualAddress) {
	return m_pmemmgr->QueryProtect(VirtualAddress);
}

XboxTypes::SIZE_T XboxKernel::MmQueryAllocationSize(XboxTypes::PVOID BaseAddress) {
	return m_pmemmgr->QueryAllocationSize(BaseAddress);
}

XboxTypes::VOID XboxKernel::MmSetAddressProtect(XboxTypes::PVOID BaseAddress, XboxTypes::ULONG NumberOfBytes, XboxTypes::ULONG NewProtect) {
	return m_pmemmgr->SetProtect(BaseAddress, NumberOfBytes, NewProtect);
}
