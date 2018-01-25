#include "common.h"

/*
 * MmPersistContiguousMemory
 *
 * Import Number:      178
 * Calling Convention: stdcall
 * Parameter 0:        PVOID   BaseAddress
 * Parameter 1:        SIZE_T  NumberOfBytes
 * Parameter 2:        BOOLEAN Persist
 * Return Type:        VOID
 */
int Xbox::MmPersistContiguousMemory()
{
	K_ENTER_STDCALL();
	K_INIT_ARG(XboxTypes::PVOID,   BaseAddress);
	K_INIT_ARG(XboxTypes::SIZE_T,  NumberOfBytes);
	K_INIT_ARG(XboxTypes::BOOLEAN, Persist);

	m_memmgr->SetPersist(BaseAddress, NumberOfBytes, Persist);

	K_EXIT();
	return 0;
}
