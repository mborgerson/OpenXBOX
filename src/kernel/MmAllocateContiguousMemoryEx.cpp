#include "common.h"

/*
 * MmAllocateContiguousMemoryEx
 *
 * Import Number:      166
 * Calling Convention: stdcall
 * Parameter 0:        SIZE_T    NumberOfBytes
 * Parameter 1:        ULONG_PTR LowestAcceptableAddress
 * Parameter 2:        ULONG_PTR HighestAcceptableAddress
 * Parameter 3:        ULONG_PTR Alignment
 * Parameter 4:        ULONG     Protect
 * Return Type:        PVOID
 */
int Xbox::MmAllocateContiguousMemoryEx()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(XboxTypes::SIZE_T,    NumberOfBytes);
	K_INIT_ARG(XboxTypes::ULONG_PTR, LowestAcceptableAddress);
	K_INIT_ARG(XboxTypes::ULONG_PTR, HighestAcceptableAddress);
	K_INIT_ARG(XboxTypes::ULONG_PTR, Alignment);
	K_INIT_ARG(XboxTypes::ULONG,     Protect);
	XboxTypes::PVOID rval;

	printf("NumberOfBytes            = %x\n", NumberOfBytes);
	printf("LowestAcceptableAddress  = %x\n", LowestAcceptableAddress);
	printf("HighestAcceptableAddress = %x\n", HighestAcceptableAddress);
	printf("Alignment                = %x\n", Alignment);
	printf("Protect                  = %x\n", Protect);

	ContiguousMemoryBlock *block = m_memmgr->AllocateContiguous(NumberOfBytes, LowestAcceptableAddress, HighestAcceptableAddress, Alignment, Protect);
	if (block != nullptr) {
		rval = block->BaseAddress();
		printf("...allocated at %x\n", rval);
	}
	else {
		rval = 0;
		printf("...failed to allocate\n");
	}

	K_EXIT_WITH_VALUE(rval);
	return 0;
}
