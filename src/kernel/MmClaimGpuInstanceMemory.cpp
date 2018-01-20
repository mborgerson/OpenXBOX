#include "common.h"

/*
 * MmClaimGpuInstanceMemory
 *
 * Import Number:      168
 * Calling Convention: stdcall
 * Parameter 0:        SIZE_T  NumberOfBytes
 * Parameter 1:        PSIZE_T NumberOfPaddingBytes
 * Return Type:        PVOID
 */
int Xbox::MmClaimGpuInstanceMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(XboxTypes::SIZE_T,  NumberOfBytes);
	K_INIT_ARG(XboxTypes::PSIZE_T, NumberOfPaddingBytes);
	XboxTypes::PVOID rval;

	SIZE_T val = 0x10000;
	m_cpu->MemWrite(NumberOfPaddingBytes, 4, &val);
	rval = 0x03FF0000;

	K_EXIT_WITH_VALUE(rval);
	return 0;
}
