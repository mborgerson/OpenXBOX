#include "common.h"


// Xbox uses 4 KiB pages
#define TARGET_PAGE_MASK 0xfff
#define TARGET_PAGE_ALIGN(x) (((x) + 0xfff) & ~0xfff)

uint32_t start = MiB(16);
uint32_t cur = start;
uint32_t end = MiB(40);

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

	printf("NumberOfBytes            = %x,\n", NumberOfBytes);
	printf("LowestAcceptableAddress  = %x,\n", LowestAcceptableAddress);
	printf("HighestAcceptableAddress = %x,\n", HighestAcceptableAddress);
	printf("Alignment                = %x,\n", Alignment);
	printf("Protect                  = %x\n", Protect);
	rval = TARGET_PAGE_ALIGN(cur);
	cur  = TARGET_PAGE_ALIGN(rval + NumberOfBytes);
	printf("...allocated at %x\n", rval);

	K_EXIT_WITH_VALUE(rval);
	return 0;
}
